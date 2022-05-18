ifneq ("$(wildcard .env)","")
    include .env
endif

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

libDir := $(abspath lib)
buildDir := $(abspath bin)
vendorDir := $(abspath vendor)
scriptsDir := $(abspath scripts)
releaseDir := $(buildDir)/release
outputDir := $(abspath output)
executable := app
target := $(buildDir)/$(executable)
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

includes = -I $(vendorDir)/vulkan/include -I $(vendorDir)/glfw/include
linkFlags = -L $(libDir) -lglfw3
compileFlags := -std=c++17 $(includes)

glfwLib := $(libDir)/libglfw3.a

vertSources = $(call rwildcard,shaders/,*.vert)
vertObjFiles = $(patsubst %.vert,$(buildDir)/%.vert.spv,$(vertSources))
fragSources = $(call rwildcard,shaders/,*.frag)
fragObjFiles = $(patsubst %.frag,$(buildDir)/%.frag.spv,$(fragSources))

ifeq ($(OS),Windows_NT)
    platform := windows
    CXX ?= g++
	
    PATHSEP := \$(BLANK)
    MKDIR := $(call platformpth,$(CURDIR)/scripts/mkdir.bat)
    RM := rm -r -f

    volkDefines = VK_USE_PLATFORM_WIN32_KHR
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
    COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3

    glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator.exe
else 
    UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)

        platform := linux
        CXX ?= g++
        libSuffix = so
        volkDefines = VK_USE_PLATFORM_XLIB_KHR
        linkFlags += -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
	endif
	ifeq ($(UNAMEOS),Darwin)
		
        platform := macos
        CXX ?= clang++
        volkDefines = VK_USE_PLATFORM_MACOS_MVK
        linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif

    PATHSEP := /
    MKDIR := mkdir -p
    RM := rm -rf
    MV := mv
    COPY = $1$(PATHSEP)$3 $2

    glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator
endif

# Lists phony targets for Makefile
.PHONY: all app release clean

all: app release clean

app: $(target)

# Link the program and create the executable
$(target): $(objects) $(glfwLib) $(vertObjFiles) $(fragObjFiles)
	$(CXX) $(objects) -o $(target) $(linkFlags)

$(buildDir)/%.spv: % 
	$(MKDIR) $(call platformpth, $(@D))
	$(glslangValidator) $< -V -o $@

$(glfwLib):
	$(MKDIR) $(call platformpth, $(libDir))
	$(call COPY,$(vendorDir)/glfw/src,$(libDir),libglfw3.a)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth,$(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

release: app
	$(MKDIR) $(call platformpth, $(releaseDir))
	$(CP) $(buildDir)/app $(releaseDir)
	$(CP) $(buildDir)/shaders $(releaseDir)
	$(CP) $(scriptsDir)/startup.sh $(releaseDir)
	$(MKDIR) $(call platformpth, $(releaseDir)/lib)
	$(CP) $(vendorDir)/vulkan/lib/$(platform)/* $(releaseDir)/lib
	$(MKDIR) $(call platformpth, $(outputDir))
	$(scriptsDir)/makeapp.sh "Snek" $(releaseDir) startup.sh $(outputDir)

clean: 
	$(RM) $(call platformpth, $(buildDir))
	$(RM) $(call platformpth, $(outputDir))
	$(RM) $(call platformpth, $(libDir))

ifneq ("$(wildcard .env)","")
    include .env
endif

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	override CXXFLAGS += -g -DDEBUG
else
    override CXXFLAGS += -DNDEBUG
endif

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

libDir := $(abspath lib)
buildDir := $(abspath bin)
vendorDir := $(abspath vendor)
scriptsDir := $(abspath scripts)
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
    executableSuffix := .exe
    startupScript := startup.bat
    makeappScript := makeapp.bat
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
    COPY = cp -r $1$(PATHSEP)$3 $2

    glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator
    startupScript := $(scriptsDir)/startup.sh
    packageScript := $(scriptsDir)/packageapp.sh
endif

# Lists phony targets for Makefile
.PHONY: all app release clean

all: app release clean

app: $(target)

# Link the program and create the executable
$(target): $(objects) $(glfwLib) $(vertObjFiles) $(fragObjFiles) $(buildDir)/lib
	$(CXX) $(objects) -o $(target) $(linkFlags)

$(buildDir)/%.spv: % 
	$(MKDIR) $(call platformpth, $(@D))
	$(glslangValidator) $< -V -o $@

$(glfwLib):
	$(MKDIR) $(call platformpth, $(libDir))
	$(call COPY,$(vendorDir)/glfw/src,$(libDir),libglfw3.a)

$(buildDir)/lib:
	$(MKDIR) $(call platformpth, $(buildDir)/lib)
	$(call COPY,$(vendorDir)/vulkan/lib/$(platform)/,$(buildDir)/lib,*)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth,$(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

package: app
	$(packageScript) "Snek" $(outputDir) $(buildDir) $(DEBUG)

clean: 
	$(RM) $(call platformpth, $(buildDir))
	$(RM) $(call platformpth, $(outputDir))
	$(RM) $(call platformpth, $(libDir))

ifneq ("$(wildcard .env)","")
    include .env
endif

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

buildDir := bin
executable := app
target := $(buildDir)/$(executable)
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

includes = -I vendor/vulkan/include -I vendor/glfw/include
linkFlags = -L lib -lglfw3
compileFlags := -std=c++17 $(includes)

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
endif

# Lists phony targets for Makefile
.PHONY: all app clean

all: app clean

app: bin/app

# Link the program and create the executable
$(target): $(objects) $(vertObjFiles) $(fragObjFiles)
	$(CXX) $(objects) -o $(target) $(linkFlags)

$(buildDir)/%.spv: % 
	$(MKDIR) $(call platformpth, $(@D))
	${GLSLC} $< -V -o $@

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth,$(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

clean: 
	$(RM) $(call platformpth, $(buildDir)/*)

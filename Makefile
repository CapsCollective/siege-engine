rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

buildDir := bin
executable := app
target := $(buildDir)/$(executable)
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

includes = -I vendor/include -I vendor/glfw/include -I vendor/include/volk -I $(vulkanIncludes)
linkFlags = -L lib/$(platform) -lglfw3
compileFlags = -std=c++17 $(includes)

ifeq ($(OS),Windows_NT)

    platformpth = "$(subst /,$(PATHSEP),$1)"
    platform := Windows
    CXX ?= g++
	
    THEN := &&
    PATHSEP := \$(BLANK)
    MKDIR := $(call platformpth,$(CURDIR)/scripts/windows/mkdir.bat)
    RM := rm -r -f
    COPY = -robocopy $(call platformpth,$1) $(call platformpth,$2) $3

    volkDefines = VK_USE_PLATFORM_WIN32_KHR
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
else 
    UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)

        platform := Linux
        CXX ?= g++
        libSuffix = so
        volkDefines = VK_USE_PLATFORM_XLIB_KHR
        linkFlags += -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
	endif
	ifeq ($(UNAMEOS),Darwin)
		
        platform := macOS
        CXX ?= clang++
        volkDefines = VK_USE_PLATFORM_MACOS_MVK
        linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif

    PATHSEP := /
    MKDIR := mkdir -p
    COPY = cp $1$(PATHSEP)$3 $2
    THEN := ;
    RM := rm -rf
endif

# Lists phony targets for Makefile
.PHONY: all setup submodules execute clean

all: $(target) execute clean

ifndef VULKAN_SDK
    DYLD_LIBRARY_PATH=?$(call platformpth,$(CURDIR)/lib/$(platform))
    vulkanIncludes := vendor/include/vulkan
    ifeq ($(UNAMEOS),Darwin)
        VK_ICD_FILENAMES?=$(CURDIR)/lib/macOS/icd.d/MoltenVK_icd.json
    else
        ifdef DEBUG
            VK_LAYER_PATH ?= $(call platformpth,$(CURDIR)/lib/$(platform)/explicit_layer.d)
        endif
    endif
else
    vulkanIncludes := $(VULKAN_SDK)/include
endif

# Link the program and create the executable
$(target): $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth,$(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

clear: 
	clear;

execute: 
	$(target) $(ARGS)

clean: 
	$(RM) $(call platformpth, $(buildDir)/*)
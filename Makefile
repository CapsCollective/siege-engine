rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

buildDir := bin
executable := app
target := $(buildDir)/$(executable)
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

includes := -I include -I vendor/glfw/include -I include/volk -I $(VULKAN_SDK)/include
linkFlags = -L lib/$(platform) -lglfw3
compileFlags := -std=c++17 $(includes)

ifdef MACRO_DEFS
    macroDefines := -D $(MACRO_DEFS)
endif

ifndef DYLD_LIBRARY_PATH
    export DYLD_LIBRARY_PATH=$(CURDIR)/lib/$(platform)
endif 

ifeq ($(OS),Windows_NT)

	LIB_EXT = .lib
	CMAKE_CMD = cmake -G "MinGW Makefiles" .

	vulkanLibDir := Lib
	vulkanLib := vulkan-1

	platform := Windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := rm -r -f
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3

	volkDefines = VK_USE_PLATFORM_WIN32_KHR
else 
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		
		LIB_EXT :=
		
		vulkanLib := vulkan.so

		vulkanExports := export VK_LAYER_PATH=$(VULKAN_SDK)/etc/explicit_layer.d

		NUMBER_OF_PROCESSORS := $(shell nproc)

		platform := Linux
		volkDefines = VK_USE_PLATFORM_XLIB_KHR
		CXX ?= g++
		linkFlags += -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
	endif
	ifeq ($(UNAMEOS), Darwin)
		
		LIB_EXT := .dylib
		
		vulkanLib := vulkan.1
		vulkanLibVersion := $(patsubst %.0,%,$(VK_VERSION))

		vulkanExports := export export VK_ICD_FILENAMES=$(VULKAN_SDK)/share/vulkan/icd.d/MoltenVK_icd.json; \ 
						export VK_LAYER_PATH=$(VULKAN_SDK)/share/vulkan/explicit_layer.d
		NUMBER_OF_PROCESSORS := $(shell sysctl -n hw.ncpu)
		volkDefines = VK_USE_PLATFORM_MACOS_MVK

		platform := macOS
		CXX ?= clang++
		
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif
	
	vulkanLibDir := lib

	vulkanLibDir := lib
	vulkanLibPrefix := $(vulkanLibDir)
	CMAKE_CMD = cmake -G "Unix Makefiles" .

	PATHSEP := /
	MKDIR = mkdir -p
	COPY = cp $1$(PATHSEP)$3 $2
	THEN = ;
	RM := rm -rf
endif

# Lists phony targets for Makefile
.PHONY: all setup submodules execute clean

all: $(target) execute clean

submodules:
	git submodule update --init --recursive

setup: submodules lib

lib: setup-volk setup-glfw
	$(call COPY,$(VULKAN_SDK)/$(vulkanLibDir),lib/$(platform),$(vulkanLibPrefix)$(vulkanLib)$(LIB_EXT))

setup-glfw:
	$(call updateSubmodule,glfw)

	cd $(call platformpth,vendor/glfw) $(THEN) $(CMAKE_CMD) $(THEN) "$(MAKE)" -j$(NUMBER_OF_PROCESSORS)
	$(MKDIR) $(call platformpth,lib/$(platform))
	$(call COPY,vendor/glfw/src,lib/$(platform),libglfw3.a)

setup-volk:
	$(call updateSubmodule,volk)

	$(MKDIR) $(call platformpth,include/volk)

	$(call COPY,vendor/volk,include/volk,volk.h)
	$(call COPY,vendor/volk,include/volk,volk.c)

# Link the program and create the executable
$(target): $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ -D$(volkDefines) $(macroDefines)

clear: 
	clear;

execute: 
	$(target) $(ARGS)

clean: 
	$(RM) $(call platformpth, $(buildDir)/*)

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

buildDir := bin
executable := app
target := $(buildDir)/$(executable)
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

submoduleDir := vendor
updateSubmodule = git submodule update --init $(submoduleDir)/$1

includes = -I include -I vendor/glfw/include -I include/volk -I $(vulkanIncludes)
linkFlags = -L lib/$(platform) -lglfw3
compileFlags = -std=c++17 $(includes)

ifndef DYLD_LIBRARY_PATH
    export DYLD_LIBRARY_PATH=$(CURDIR)/lib/$(platform)
endif 

ifeq ($(OS),Windows_NT)

	LIB_EXT = .lib

	vulkanLibDir := Lib
	vulkanLib := vulkan-1

	platform := Windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := $(call platformpth,$(CURDIR)/scripts/windows/mkdir.bat)
	RM := rm -r -f
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3

	generator := "MinGW Makefiles"

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

		NUMBER_OF_PROCESSORS := $(shell sysctl -n hw.ncpu)
		volkDefines = VK_USE_PLATFORM_MACOS_MVK

		platform := macOS
		CXX ?= clang++
		
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif
	
	vulkanLibDir := lib

	vulkanLibDir := lib
	vulkanLibPrefix := $(vulkanLibDir)
	generator := "Unix Makefiles"

	PATHSEP := /
	MKDIR = mkdir -p
	COPY = cp $1$(PATHSEP)$3 $2
	THEN = ;
	RM := rm -rf
endif

# Lists phony targets for Makefile
.PHONY: all setup submodules execute clean

all: $(target) execute clean

ifndef VULKAN_SDK
    vulkanIncludes := include/vulkan

    setup-vulkan-headers:
		$(call updateSubmodule,Vulkan-Headers)
		cd $(call platformpth,vendor/Vulkan-Headers) $(THEN) git fetch --all --tags $(THEN) git checkout tags/v1.3.211
		$(MKDIR) $(call platformpth,$(CURDIR)/vendor/Vulkan-Headers/build)

		cd $(call platformpth,vendor/Vulkan-Headers/build) $(THEN) cmake -DCMAKE_INSTALL_PREFIX=install -G $(generator) ..
		cd $(call platformpth,vendor/Vulkan-Headers/build) $(THEN) cmake --build . --target install

		$(MKDIR) $(call platformpth,include/vulkan)
		$(call COPY,vendor/Vulkan-Headers/include/vulkan,include/vulkan,**.h)

    lib: setup-volk setup-glfw setup-vulkan-headers
else
    lib: setup-volk setup-glfw
    vulkanIncludes := $(VULKAN_SDK)/include
endif

setup: lib

setup-glfw:
	$(call updateSubmodule,glfw)

	cd $(call platformpth,vendor/glfw) $(THEN) cmake -G $(generator) . $(THEN) "$(MAKE)" -j$(NUMBER_OF_PROCESSORS)
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
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

clear: 
	clear;

execute: 
	$(target) $(ARGS)

clean: 
	$(RM) $(call platformpth, $(buildDir)/*)

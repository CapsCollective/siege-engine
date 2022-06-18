# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
renderSrcDir := .
renderBinDir := $(binDir)/engine/render
renderSources := $(call rwildcard,$(renderSrcDir)/,*.cpp)
renderObjects := $(call findobjs,$(renderSrcDir),$(renderBinDir),$(renderSources))
renderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(renderBinDir)/,*.o))
renderBuildDir := $(renderBinDir)/build

# Set shader build vars
vertSources := $(call rwildcard,assets/shaders,*.vert)
fragSources := $(call rwildcard,assets/shaders,*.frag)
vertObjects := $(patsubst %.vert,$(renderBuildDir)/%.vert.spv,$(vertSources))
fragObjects := $(patsubst %.frag,$(renderBuildDir)/%.frag.spv,$(fragSources))

# Set build vars
ifeq ($(platform), windows)
    volkDefines = VK_USE_PLATFORM_WIN32_KHR
    glslangValidator := $(vendorDir)\glslang\build\install\bin\glslangValidator.exe
else ifeq ($(platform), linux)
	volkDefines = VK_USE_PLATFORM_XLIB_KHR
	glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator
else ifeq ($(platform), macos)
	volkDefines = VK_USE_PLATFORM_MACOS_MVK
	glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator
endif
compileFlags += -I $(vendorDir)/vulkan/include -I $(vendorDir)/glfw/include -I $(vendorDir)/glm -I $(vendorDir)/tinyobjloader

.PHONY: all vulkan-libs

all: $(renderLib) $(vertObjects) $(fragObjects) vulkan-libs

# Build the static library
$(renderLib): $(renderObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(renderLib))
	$(call COPY,$(vendorDir)/glfw/src,$(libDir),libglfw3.a)
	mv $(call platformpth,$(libDir)/libglfw3.a) $(call platformpth,$(libDir)/librender.a)
	ar -rcs $(renderLib) $(renderObjects) 

# Add all rules from dependency files
-include $(renderDepends)

# Compile object files to the bin directory
$(renderBinDir)/%.o: $(renderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

# Compile shaders to the build directory
$(renderBuildDir)/assets/shaders/%.spv: $(renderSrcDir)/assets/shaders/%
	$(MKDIR) $(call platformpth, $(@D))
	$(glslangValidator) $< -V -o $@

# Copy Vulkan libraries to the build directory
vulkan-libs:
	$(MKDIR) $(call platformpth, $(renderBuildDir)/lib)
	$(call COPY_DIR,$(vendorDir)/vulkan/lib,$(renderBuildDir)/lib)
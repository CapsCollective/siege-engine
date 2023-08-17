# Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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
renderLibs := $(vendorDir)/zlib/build/lib/libz.a $(vendorDir)/libpng/build/libpng.a $(vendorDir)/freetype/build/libfreetype.a

# Set shader build vars
vertSources := $(call rwildcard,assets/shaders,*.vert)
fragSources := $(call rwildcard,assets/shaders,*.frag)
vertObjects := $(patsubst %.vert,$(renderBuildDir)/%.vert.spv,$(vertSources))
fragObjects := $(patsubst %.frag,$(renderBuildDir)/%.frag.spv,$(fragSources))

linkFlags += -l utils -l window

compileFlags += -I $(vendorDir)/vulkan/include -I $(vendorDir)/glfw/include -I $(vendorDir)/glm \
				-I $(vendorDir)/tinyobjloader -I $(vendorDir)/stb_image -I $(vendorDir)/zlib/build/include \
				-I $(vendorDir)/libpng -I $(vendorDir)/include/freetype

.PHONY: all vulkan-libs

all: $(renderLib) $(vertObjects) $(fragObjects) vulkan-libs

# Build the static library
$(renderLib): $(renderObjects)
	$(RM) $(call platformpth, $(renderLib))
	$(call COMBINE_LIBS, $(renderLibs), $(renderObjects), $(libDir), render)

# Add all rules from dependency files
-include $(renderDepends)

# Compile object files to the bin directory
$(renderBinDir)/%.o: $(renderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS) -DVK_USE_PLATFORM_$(VOLK_OS)

# Compile shaders to the build directory
$(renderBuildDir)/assets/shaders/%.spv: $(renderSrcDir)/assets/shaders/%
	$(MKDIR) $(call platformpth, $(@D))
	$(call platformpth,$(vendorDir)/glslang/build/StandAlone/glslangValidator$(EXE_NAME)) $< -V -o $@

# Copy Vulkan libraries to the build directory
vulkan-libs:
	$(MKDIR) $(call platformpth, $(renderBuildDir)/lib)
	$(call COPY,$(vendorDir)/vulkan/lib,$(renderBuildDir)/lib,**)
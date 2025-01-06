# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
exampleRenderSrcDir := .
exampleRenderBinDir := $(binDir)/examples/render
exampleRenderSources := $(call rwildcard,$(exampleRenderSrcDir)/,*.cpp)
exampleRenderObjects := $(call findobjs,$(exampleRenderSrcDir),$(exampleRenderBinDir),$(exampleRenderSources))
exampleRenderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleRenderBinDir)/,*.o))
exampleRenderBuildDir := $(exampleRenderBinDir)/build
exampleRenderOutputName := "Render Example"

# Set packaging vars
ifeq ($(platform), linux)
    packagingEnvVars := \
    	export DYLD_LIBRARY_PATH='./lib'; \
    	export VK_LAYER_PATH='./lib/explicit_layer.d';
else ifeq ($(platform), macos)
    packagingEnvVars := \
		export DYLD_LIBRARY_PATH='./lib'; \
		export VK_LAYER_PATH='./lib/explicit_layer.d'; \
		export VK_ICD_FILENAMES='./lib/icd.d/MoltenVK_icd.json'
endif
engineRenderBuildDir := $(binDir)/engine/render/build
exampleRenderAssets := $(patsubst ./%,%, $(call rwildcard,$(exampleRenderSrcDir),*.obj))
exampleRenderAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleRenderSrcDir),*.png))
exampleRenderAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleRenderSrcDir),*.jpg))
exampleRenderAssets += $(patsubst $(engineRenderBuildDir)/%,%, $(call rwildcard,$(engineRenderBuildDir),*.spv))
exampleRenderAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleRenderSrcDir),*.ttf))

ifneq ($(ENABLE_VALIDATION_LAYERS), 1)
	packagingExcludes := $(VALIDATION_LAYERS_INSTALL_DIR)
endif

ifneq ("$(packagingEnvVars)","")
	ENV_VARS = --env-vars "$(packagingEnvVars)"
endif

packagingFlags = $(ENV_VARS) --excludes "$(packagingExcludes)"

# Set build vars
compileFlags += -I $(vendorDir)/glfw/include -I $(vendorDir)/glm -I $(vendorDir)/freetype/include
linkFlags += -l render -l window -l resources -l utils

.PHONY: all render-assets

all: $(exampleRenderApp) render-assets

# Link the object files and create an executable
$(exampleRenderApp): $(renderLib) $(exampleRenderObjects)
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) $(exampleRenderObjects) -o $(exampleRenderApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleRenderDepends)

# Compile object files to the bin directory
$(exampleRenderBinDir)/%.o: $(exampleRenderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy assets (and other required build files) directory to the build directory
render-assets:
	$(MKDIR) $(call platformpth, $(exampleRenderBuildDir)/assets)
	$(call COPY,$(exampleRenderSrcDir)/assets,$(exampleRenderBuildDir)/assets,**)
	$(MKDIR) $(call platformpth,$(exampleRenderBuildDir)/assets/shaders)
	$(call COPY,$(binDir)/engine/render/build/assets/shaders,$(exampleRenderBuildDir)/assets/shaders,**)
	$(packerApp) $(exampleRenderBuildDir)/app.pck $(exampleRenderBuildDir) $(exampleRenderAssets)
	$(RM) $(call platformpth,$(exampleRenderBuildDir)/assets)

# Package the built application and all its assets to the output directory
package:
	$(RM) "$(outputDir)/$(exampleRenderOutputName)"
	$(call PACKAGE_SCRIPT, $(exampleRenderOutputName), $(call BASENAME, $(exampleRenderApp)), $(outputDir), $(exampleRenderBuildDir), $(packagingFlags))

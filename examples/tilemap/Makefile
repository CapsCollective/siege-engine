# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
exampleTilemapSrcDir := .
exampleTilemapBinDir := $(binDir)/examples/tilemap
exampleTilemapSources := $(call rwildcard,$(exampleTilemapSrcDir)/,*.cpp)
exampleTilemapObjects := $(call findobjs,$(exampleTilemapSrcDir),$(exampleTilemapBinDir),$(exampleTilemapSources))
exampleTilemapDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleTilemapBinDir)/,*.o))
exampleTilemapBuildDir := $(exampleTilemapBinDir)/build
exampleTilemapOutputName := "Tilemap Example"

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

ifneq ($(ENABLE_VALIDATION_LAYERS), 1)
	packagingExcludes := $(VALIDATION_LAYERS_INSTALL_DIR)
endif

ifneq ("$(packagingEnvVars)","")
	ENV_VARS = --env-vars "$(packagingEnvVars)"
endif

packagingFlags = $(ENV_VARS) --excludes "$(packagingExcludes)"

# Set build vars
compileFlags += -I $(vendorDir)/glfw/include -I $(vendorDir)/glm \
				-I $(vendorDir)/tinyobjloader -I $(vendorDir)/freetype/include
linkFlags += -l render -l window -l utils

.PHONY: all render-assets tilemap-assets

all: $(exampleTilemapApp) tilemap-assets render-assets

# Link the object files and create an executable
$(exampleTilemapApp): $(renderLib) $(exampleTilemapObjects)
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) $(exampleTilemapObjects) -o $(exampleTilemapApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleTilemapDepends)

# Compile object files to the bin directory
$(exampleTilemapBinDir)/%.o: $(exampleTilemapSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy assets directory to the build directory
tilemap-assets:
	$(MKDIR) $(call platformpth, $(exampleTilemapBuildDir)/assets)
	$(call COPY,$(exampleTilemapSrcDir)/assets,$(exampleTilemapBuildDir)/assets,**)

# Copy assets (and other required build files) directory to the build directory
render-assets:
	$(MKDIR) $(call platformpth,$(exampleTilemapBuildDir))
	$(call COPY,$(binDir)/engine/render/build,$(exampleTilemapBuildDir),**)
	$(MKDIR) $(call platformpth,$(exampleTilemapBuildDir)/assets)
	$(call COPY,$(exampleTilemapSrcDir)/assets,$(exampleTilemapBuildDir)/assets,**)

# Package the built application and all its assets to the output directory
package:
	$(RM) "$(outputDir)/$(exampleTilemapOutputName)"
	$(call PACKAGE_SCRIPT, $(exampleTilemapOutputName), $(shell basename $(exampleTilemapApp)), $(outputDir), $(exampleTilemapBuildDir), $(packagingFlags))
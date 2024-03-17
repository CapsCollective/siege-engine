# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
exampleGameSrcDir := .
exampleGameBinDir := $(binDir)/examples/game
exampleGameSources := $(call rwildcard,$(exampleGameSrcDir)/,*.cpp)
exampleGameObjects := $(call findobjs,$(exampleGameSrcDir),$(exampleGameBinDir),$(exampleGameSources))
exampleGameDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleGameBinDir)/,*.o))
exampleGameBuildDir := $(exampleGameBinDir)/build
exampleGameOutputName := "Game Example"

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
exampleGameAssets := $(patsubst ./%,%, $(call rwildcard,$(exampleGameSrcDir),*.obj))
exampleGameAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleGameSrcDir),*.png))
exampleGameAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleGameSrcDir),*.jpg))

ifneq ($(ENABLE_VALIDATION_LAYERS), 1)
	packagingExcludes := $(VALIDATION_LAYERS_INSTALL_DIR)
endif

ifneq ("$(packagingEnvVars)","")
	ENV_VARS = --env-vars "$(packagingEnvVars)"
endif

packagingFlags = $(ENV_VARS) --excludes "$(packagingExcludes)"

# Set build vars
linkFlags += -l core -l window -l render -l resources -l utils

.PHONY: all game-assets render-assets

all: $(exampleGameApp) game-assets render-assets

# Link the object files and create an executable
$(exampleGameApp): $(utilsLib) $(windowLib) $(renderLib) $(coreLib) $(exampleGameObjects)
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) $(exampleGameObjects) -o $(exampleGameApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleGameDepends)

# Compile object files to the bin directory
$(exampleGameBinDir)/%.o: $(exampleGameSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy assets directory to the build directory
game-assets:
	$(MKDIR) $(call platformpth, $(exampleGameBuildDir)/assets)
	$(call COPY,$(exampleGameSrcDir)/assets,$(exampleGameBuildDir)/assets,**)

# Copy assets (and other required build files) directory to the build directory
render-assets:
	$(MKDIR) $(call platformpth,$(exampleGameBuildDir))
	$(call COPY,$(binDir)/engine/render/build,$(exampleGameBuildDir),**)
	$(MKDIR) $(call platformpth,$(exampleGameBuildDir)/assets)
	$(call COPY,$(exampleGameSrcDir)/assets,$(exampleGameBuildDir)/assets,**)
	$(packerApp) $(exampleGameBuildDir)/app.pck $(exampleGameBuildDir) $(exampleGameAssets)
	$(RM) $(call platformpth,$(exampleGameBuildDir)/assets/models)
	$(RM) $(call platformpth,$(exampleGameBuildDir)/assets/textures)

# Package the built application and all its assets to the output directory
package: all
	$(RM) "$(outputDir)/$(exampleGameOutputName)"
	$(call PACKAGE_SCRIPT, $(exampleGameOutputName), $(shell basename $(exampleGameApp)), $(outputDir), $(exampleGameBuildDir), $(packagingFlags))
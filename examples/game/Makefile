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
exampleGameAssets += $(patsubst $(engineRenderBuildDir)/%,%, $(call rwildcard,$(engineRenderBuildDir),*.spv))
exampleGameAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleGameSrcDir),*.ttf))
exampleGameAssets += $(patsubst ./%,%, $(call rwildcard,$(exampleGameSrcDir),*.scene))

ifneq ($(ENABLE_VALIDATION_LAYERS), 1)
	packagingExcludes := $(VALIDATION_LAYERS_INSTALL_DIR)
endif

ifneq ("$(packagingEnvVars)","")
	ENV_VARS = --env-vars "$(packagingEnvVars)"
endif

packagingFlags = $(ENV_VARS) --excludes "$(packagingExcludes)"

# Set build vars
compileFlags += -I $(vendorDir)/glfw/include -I $(vendorDir)/glm -I $(vendorDir)/freetype/include
linkFlags += -l core -l window -l render -l resources -l utils

.PHONY: all pack-assets

all: $(exampleGameApp) pack-assets

# Link the object files and create an executable
$(exampleGameApp): $(utilsLib) $(windowLib) $(renderLib) $(coreLib) $(exampleGameObjects)
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) $(exampleGameObjects) -o $(exampleGameApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleGameDepends)

# Compile object files to the bin directory
$(exampleGameBinDir)/%.o: $(exampleGameSrcDir)/%.cpp
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy assets directory to the build directory and pack them
pack-assets:
	$(call MKDIR,$(call platformpth,$(exampleGameBuildDir)/assets))
	$(call COPY,$(exampleGameSrcDir)/assets,$(exampleGameBuildDir)/assets,$(RWCARDGLOB))
	$(call MKDIR,$(call platformpth,$(exampleGameBuildDir)/assets/shaders))
	$(call COPY,$(binDir)/engine/render/build/assets/shaders,$(exampleGameBuildDir)/assets/shaders,$(RWCARDGLOB))
	$(packerApp) $(exampleGameBuildDir)/app.pck $(exampleGameBuildDir) $(exampleGameAssets)
	$(RM) "$(exampleGameBuildDir)/assets"
	$(call PACK_LIBS_SCRIPT,$(vendorDir)/vulkan/lib,$(exampleGameBuildDir))


# Package the built application and all its assets to the output directory
package: all
	$(RM) "$(outputDir)/$(exampleGameOutputName)"
	$(call PACKAGE_SCRIPT,$(exampleGameOutputName),$(call BASENAME,$(exampleGameApp)),$(outputDir),$(exampleGameBuildDir),$(packagingFlags))



# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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

# Set build vars
linkFlags += -l core -l utils

.PHONY: all game-assets

all: $(exampleGameApp) game-assets

# Link the object files and create an executable
$(exampleGameApp): $(utilsLib) $(coreLib) $(exampleGameObjects)
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

# Package the built application and all its assets to the output directory
package: all
	$(RM) $(call platformpth,$(outputDir)/$(exampleGameOutputName))
	$(packageScript) $(exampleGameOutputName) $(shell basename $(exampleGameApp)) $(outputDir) $(exampleGameBuildDir)
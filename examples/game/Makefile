include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for exampleGame
exampleGameSrcDir := .
exampleGameBinDir := $(binDir)/examples/game
exampleGameSources := $(call rwildcard,$(exampleGameSrcDir)/,*.cpp)
exampleGameObjects := $(call findobjs,$(exampleGameSrcDir),$(exampleGameBinDir),$(exampleGameSources))
exampleGameDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleGameBinDir)/,*.o))
exampleGameBuildDir := $(exampleGameBinDir)/build
exampleGameOutputName := "Game Example"

# Set build vars
linkFlags += -l core -l utils

# Link the object files and create an executable
$(exampleGameApp): $(exampleGameObjects) $(exampleGameBuildDir)
	$(CXX) $(exampleGameObjects) -o $(exampleGameApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleGameDepends)

# Compile object files to the build directory
$(exampleGameBinDir)/%.o: $(exampleGameSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the output files to the build directory
$(exampleGameBuildDir):
	$(MKDIR) $(call platformpth,$(exampleGameBuildDir)/assets)
	$(call COPY_DIR,assets,$(exampleGameBuildDir)/assets)

package:
	$(RM) "$(outputDir)/$(exampleGameOutputName)"*
	$(packageScript) $(exampleGameOutputName) $(shell basename $(exampleGameApp)) $(outputDir) $(exampleGameBuildDir)
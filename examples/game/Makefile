include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for exampleGame
exampleGameSrcDir := .
exampleGameBuildDir := $(buildDir)/examples/game
exampleGameSources := $(call rwildcard,$(exampleGameSrcDir)/,*.cpp)
exampleGameObjects := $(call findobjs,$(exampleGameSrcDir),$(exampleGameBuildDir),$(exampleGameSources))
exampleGameDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleGameBuildDir)/,*.o))
assetsDir := assets
assetsBuildDir := $(buildDir)/examples/game/$(assetsDir)

# Set build vars
linkFlags += -l core -l utils

# Link the object files and create an executable
$(exampleGameExecutable): $(exampleGameObjects) $(assetsBuildDir)
	$(CXX) $(exampleGameObjects) -o $(exampleGameExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(exampleGameDepends)

# Compile object files to the build directory
$(exampleGameBuildDir)/%.o: $(exampleGameSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the assets directory to the build directory
$(assetsBuildDir):
	$(MKDIR) $(call platformpth,$(assetsBuildDir))
	$(call COPY_DIR,$(assetsDir),$(assetsBuildDir))
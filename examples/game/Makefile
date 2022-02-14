include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for exampleGame
exampleGameSrcDir = .
exampleGameExecutable = $(buildDir)/examplegameapp
exampleGameBuildDir = $(buildDir)/examples/game
exampleGameSources = $(call rwildcard,$(exampleGameSrcDir)/,*.cpp)
exampleGameObjects = $(call findobjs,$(exampleGameSrcDir),$(exampleGameBuildDir),$(exampleGameSources))
exampleGameDepends = $(patsubst %.o, %.d, $(call rwildcard,$(exampleGameBuildDir)/,*.o))

# Link the object files and create an executable
$(exampleGameExecutable): $(exampleGameObjects)
	$(CXX) $(exampleGameObjects) -o $(exampleGameExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(exampleGameDepends)

# Compile object files to the build directory
$(exampleGameBuildDir)/%.o: $(exampleGameSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)
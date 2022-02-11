include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for example
exampleSrcDir = .
exampleBuildDir = $(buildDir)/example
exampleSources = $(call rwildcard,$(exampleSrcDir)/,*.cpp)
exampleObjects = $(call findobjs,$(exampleSrcDir),$(exampleBuildDir),$(exampleSources))
exampleDepends = $(patsubst %.o, %.d, $(call rwildcard,$(exampleBuildDir)/,*.o))

# Link the object files and create an executable
$(exampleExecutable): $(utilsLib) $(engineLib) $(exampleObjects)
	$(CXX) $(exampleObjects) -o $(exampleExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(exampleDepends)

# Compile object files to the build directory
$(exampleBuildDir)/%.o: $(exampleSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(srcDir) $< -o $@ $(CXXFLAGS)
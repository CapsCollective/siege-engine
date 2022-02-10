include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build variables for tests
testSrcDir = .
testBuildDir = $(buildDir)/tests
testSources = $(call rwildcard,$(testSrcDir)/,*.cpp)
testObjects = $(call findobjs,$(testSrcDir),$(testBuildDir),$(testSources))
testDepends = $(patsubst %.o, %.d, $(call rwildcard,$(testBuildDir)/,*.o))

# Link the object files and create an executable
$(testExecutable): $(utilsLib) $(engineLib) $(testObjects)
	$(CXX) $(testObjects) -o $(testExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(testDepends)

# Compile object files to the build directory
$(testBuildDir)/%.o: $(testSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(srcDir) $< -o $@ $(CXXFLAGS)
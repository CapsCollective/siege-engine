include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build variables for engine
engineSrcDir = .
engineBuildDir = $(buildDir)/engine
engineSources = $(call rwildcard,$(engineSrcDir)/,*.cpp)
engineObjects = $(call findobjs,$(engineSrcDir),$(engineBuildDir),$(engineSources))
engineDepends = $(patsubst %.o, %.d, $(call rwildcard,$(engineBuildDir)/,*.o))

# Build the static library for the engine
$(engineLib): $(utilsLib) $(engineObjects)
	ar -rc $(engineLib) $(engineObjects)

# Add all rules from dependency files
-include $(engineDepends)

# Compile object files to the build directory
$(engineBuildDir)/%.o: $(engineSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(srcDir) $< -o $@ $(CXXFLAGS)
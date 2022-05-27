include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for render
renderSrcDir := .
renderBuildDir := $(buildDir)/render
renderSources := $(call rwildcard,$(renderSrcDir)/,*.cpp)
renderObjects := $(call findobjs,$(renderSrcDir),$(renderBuildDir),$(renderSources))
renderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(renderBuildDir)/,*.o))

# Build the static library for render
$(renderLib): $(renderObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	ar -crs $(renderLib) $(renderObjects)

# Add all rules from dependency files
-include $(renderDepends)

# Compile object files to the build directory
$(renderBuildDir)/%.o: $(renderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)
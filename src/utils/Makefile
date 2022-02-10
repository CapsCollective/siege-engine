include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build variables for utils
utilsSrcDir = .
utilsBuildDir = $(buildDir)/utils
utilsSources = $(call rwildcard,$(utilsSrcDir)/,*.cpp)
utilsObjects = $(call findobjs,$(utilsSrcDir),$(utilsBuildDir),$(utilsSources))
utilsDepends = $(patsubst %.o, %.d, $(call rwildcard,$(utilsBuildDir)/,*.o))

# Build the static library for utils
$(utilsLib): $(utilsObjects)
	ar -rc $(utilsLib) $(utilsObjects)

# Add all rules from dependency files
-include $(utilsDepends)

# Compile object files to the build directory
$(utilsBuildDir)/%.o: $(utilsSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)
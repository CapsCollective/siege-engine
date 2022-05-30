include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for utils
utilsSrcDir := .
utilsBinDir := $(binDir)/engine/utils
utilsSources := $(call rwildcard,$(utilsSrcDir)/,*.cpp)
utilsObjects := $(call findobjs,$(utilsSrcDir),$(utilsBinDir),$(utilsSources))
utilsDepends := $(patsubst %.o, %.d, $(call rwildcard,$(utilsBinDir)/,*.o))

# Build the static library for utils
$(utilsLib): $(utilsObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	ar -crs $(utilsLib) $(utilsObjects)

# Add all rules from dependency files
-include $(utilsDepends)

# Compile object files to the build directory
$(utilsBinDir)/%.o: $(utilsSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)
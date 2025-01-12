# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars
utilsSrcDir := .
utilsBinDir := $(binDir)/engine/utils
utilsSources := $(call rwildcard,$(utilsSrcDir)/,*.cpp)
utilsObjects := $(call findobjs,$(utilsSrcDir),$(utilsBinDir),$(utilsSources))
utilsDepends := $(patsubst %.o, %.d, $(call rwildcard,$(utilsBinDir)/,*.o))

# Build the static library
$(utilsLib): $(utilsObjects)
	$(call MKDIR,$(call platformpth,$(libDir)))
	ar -crs $(utilsLib) $(utilsObjects)

# Add all rules from dependency files
-include $(utilsDepends)

# Compile object files to the bin directory
$(utilsBinDir)/%.o: $(utilsSrcDir)/%.cpp
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)
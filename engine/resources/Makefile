# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars
resourcesSrcDir := ./
resourcesBinDir := $(binDir)/engine/resources
resourcesSources := $(call rwildcard,$(resourcesSrcDir)/,*.cpp)
resourcesObjects := $(call findobjs,$(resourcesSrcDir),$(resourcesBinDir),$(resourcesSources))
resourcesDepends := $(patsubst %.o, %.d, $(call rwildcard,$(resourcesBinDir)/,*.o))
resourcesLibs := $(vendorDir)/zlib/build/lib/libz.a

# Set build vars
linkFlags += -l utils -l z
compileFlags += -I $(vendorDir)/zlib/build/include

# Build the static library
$(resourcesLib): $(resourcesObjects)
	$(call MKDIR,$(call platformpth,$(libDir)))
	$(call COMBINE_LIBS, $(resourcesLibs), $(resourcesObjects), $(libDir), resources)

# Add all rules from dependency files
-include $(resourcesDepends)

# Compile object files to the bin directory
$(resourcesBinDir)/%.o: $(resourcesSrcDir)/%.cpp
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)
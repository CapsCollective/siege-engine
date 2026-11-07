# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars
audioSrcDir := ./
audioBinDir := $(binDir)/engine/audio
audioSources := $(call rwildcard,$(audioSrcDir)/,*.cpp)
audioObjects := $(call findobjs,$(audioSrcDir),$(audioBinDir),$(audioSources))
audioDepends := $(patsubst %.o, %.d, $(call rwildcard,$(audioBinDir)/,*.o))

# Set build vars
linkFlags += -l utils
compileFlags += -I $(vendorDir)/miniaudio

# Build the static library
$(utilsLib): $(audioObjects)
	$(call MKDIR,$(call platformpth,$(libDir)))
	ar -crs $(audioLib) $(audioObjects)

# Add all rules from dependency files
-include $(audioDepends)

# Compile object files to the bin directory
$(audioBinDir)/%.o: $(audioSrcDir)/%.cpp
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)
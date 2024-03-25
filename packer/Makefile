# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
packerSrcDir := .
packerBinDir := $(binDir)/packer
packerSources := $(call rwildcard,$(packerSrcDir)/,*.cpp)
packerObjects := $(call findobjs,$(packerSrcDir),$(packerBinDir),$(packerSources))
packerDepends := $(patsubst %.o, %.d, $(call rwildcard,$(packerBinDir)/,*.o))
packerBuildDir := $(packerBinDir)/build

# Set build vars
linkFlags += -l resources -l utils
compileFlags += -I $(vendorDir)/tinyobjloader -I $(vendorDir)/stb_image

.PHONY: all

all: $(packerApp)

# Link the object files and create an executable
$(packerApp): $(packerObjects)
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) $(packerObjects) -o $(packerApp) $(linkFlags)

# Add all rules from dependency files
-include $(packerDepends)

# Compile object files to the bin directory
$(packerBinDir)/%.o: $(packerSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
windowSrcDir := ./
windowBinDir := $(binDir)/engine/window
windowSources := $(call rwildcard,$(windowSrcDir)/,*.cpp)
windowObjects := $(call findobjs,$(windowSrcDir),$(windowBinDir),$(windowSources))
windowDepends := $(patsubst %.o, %.d, $(call rwildcard,$(windowBinDir)/,*.o))
windowLibs := $(vendorDir)/glfw/build/src/libglfw3.a

# Set build vars
linkFlags += -l utils
compileFlags += -I $(vendorDir)/glfw/include

# Build the static library
$(windowLib): $(windowObjects)
	$(RM) $(call platformpth,$(windowLib))
	$(call COMBINE_LIBS, $(windowLibs), $(windowObjects), $(libDir), window)

# Add all rules from dependency files
-include $(windowDepends)

# Compile object files to the bin directory
$(windowBinDir)/%.o: $(windowSrcDir)/%.cpp
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)
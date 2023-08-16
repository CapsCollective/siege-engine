# Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
coreSrcDir := .
coreBinDir := $(binDir)/engine/core
coreSources := $(call rwildcard,$(coreSrcDir)/,*.cpp)
coreObjects := $(call findobjs,$(coreSrcDir),$(coreBinDir),$(coreSources))
coreDepends := $(patsubst %.o, %.d, $(call rwildcard,$(coreBinDir)/,*.o))

# Set build vars
ifeq ($(platform), windows)
    libGenDir := src
else ifeq ($(platform), macos)
    libGenDir := src
endif

# Build the static library
$(coreLib): $(coreObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	ar -crs $(coreLib) $(coreObjects)

# Add all rules from dependency files
-include $(coreDepends)

# Compile object files to the bin directory
$(coreBinDir)/%.o: $(coreSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineDir) $< -o $@ $(CXXFLAGS)

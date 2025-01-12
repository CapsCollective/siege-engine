# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
testSrcDir := .
testBinDir := $(binDir)/tests
testSources := $(call rwildcard,$(testSrcDir)/,*.cpp)
testObjects := $(call findobjs,$(testSrcDir),$(testBinDir),$(testSources))
testDepends := $(patsubst %.o, %.d, $(call rwildcard,$(testBinDir)/,*.o))
testBuildDir := $(testBinDir)/build

testAssets := $(patsubst ./%,%, $(call rwildcard,$(testSrcDir),*.obj))
testAssets += $(patsubst ./%,%, $(call rwildcard,$(testSrcDir),*.png))
testAssets += $(patsubst ./%,%, $(call rwildcard,$(testSrcDir),*.ttf))
testAssets += $(patsubst ./%,%, $(call rwildcard,$(testSrcDir),*.scene))

# Set build vars
utestIncludeDir := $(vendorDir)/include/utest
linkFlags += -l core -l utils -l resources

.PHONY: all pack-assets

all: $(testApp) pack-assets

# Link the object files and create an executable
$(testApp): $(utestIncludeDir) $(testObjects)
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) $(testObjects) -o $(testApp) $(linkFlags)

# Add all rules from dependency files
-include $(testDepends)

# Compile object files to the bin directory
$(testBinDir)/%.o: $(testSrcDir)/%.cpp
	$(call MKDIR,$(call platformpth,$(@D)))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(utestIncludeDir) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the relevant header files into includes
$(utestIncludeDir):
	$(call MKDIR,$(call platformpth,$(utestIncludeDir)))
	$(call COPY,$(vendorDir)/utest.h,$(utestIncludeDir),utest.h)

# Copy the assets directory to the build directory, and pack them
pack-assets:
	$(call MKDIR,$(call platformpth,$(testBuildDir)/data))
	$(call COPY,$(testSrcDir)/data,$(testBuildDir)/data,$(RWCARDGLOB))
	$(packerApp) $(testBuildDir)/app.pck $(testBuildDir) $(testAssets)

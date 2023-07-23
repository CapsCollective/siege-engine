# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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
raylibIncludeDir := $(vendorDir)/include/raylib
raylibLib := $(vendorDir)/raylib-cpp/vendor/raylib/$(libGenDir)/libraylib.a

# Build the static library
$(coreLib): $(raylibLib) $(raylibIncludeDir) $(coreObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(coreLib))
	$(call COMBINE_LIBS, $(vendorDir)/raylib-cpp/vendor/raylib/$(libGenDir)/libraylib.a, $(coreObjects), $(libDir), core)

# Add all rules from dependency files
-include $(coreDepends)

# Compile object files to the bin directory
$(coreBinDir)/%.o: $(coreSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(raylibIncludeDir) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# TODO move this into setup file
# Copy the relevant header files into includes
$(raylibIncludeDir):
	$(MKDIR) $(call platformpth, $(raylibIncludeDir)/raylib)
	$(call COPY,$(vendorDir)/raylib-cpp/vendor/raylib/src,$(raylibIncludeDir)/raylib,raylib.h,**)
	$(call COPY,$(vendorDir)/raylib-cpp/vendor/raylib/src,$(raylibIncludeDir)/raylib,raymath.h,**)
	$(call COPY,$(vendorDir)/raylib-cpp/include,$(raylibIncludeDir)/raylib,*.hpp)

# Build the raylib static library file
$(raylibLib):
	"$(MAKE)" -C $(vendorDir)/raylib-cpp/vendor/raylib/src PLATFORM=PLATFORM_DESKTOP
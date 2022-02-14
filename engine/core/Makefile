include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set platform vars
ifeq ($(platform), Windows)
    libGenDir := src
else ifeq ($(platform), macOS)
    libGenDir := src
endif
raylibLib = $(vendorDir)/raylib-cpp/vendor/raylib/$(libGenDir)/libraylib.a

# Set build vars for core
coreSrcDir = .
coreBuildDir = $(buildDir)/core
coreSources = $(call rwildcard,$(coreSrcDir)/,*.cpp)
coreObjects = $(call findobjs,$(coreSrcDir),$(coreBuildDir),$(coreSources))
coreDepends = $(patsubst %.o, %.d, $(call rwildcard,$(coreBuildDir)/,*.o))
raylibIncludeDir = $(vendorIncludeDir)/raylib

# Build the static library for the engine core
$(coreLib): $(raylibLib) $(raylibIncludeDir) $(coreObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	ar -rc $(coreLib) $(coreObjects)
	libtool -static -o $(coreLib) $(coreLib) $(raylibLib)

# Add all rules from dependency files
-include $(coreDepends)

# Compile object files to the build directory
$(coreBuildDir)/%.o: $(coreSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(raylibIncludeDir) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the relevant header files into includes
$(raylibIncludeDir):
	$(MKDIR) $(call platformpth, $(raylibIncludeDir)/raylib)
	$(call COPY,$(vendorDir)/raylib-cpp/vendor/raylib/src,$(raylibIncludeDir)/raylib,raylib.h)
	$(call COPY,$(vendorDir)/raylib-cpp/vendor/raylib/src,$(raylibIncludeDir)/raylib,raymath.h)
	$(call COPY,$(vendorDir)/raylib-cpp/include,$(raylibIncludeDir)/raylib,*.hpp)

# Build the raylib static library file
$(raylibLib):
	@$(MAKE) -C $(vendorDir)/raylib-cpp/vendor/raylib/src PLATFORM=PLATFORM_DESKTOP
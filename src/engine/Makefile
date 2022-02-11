include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set platform vars
ifeq ($(platform), Windows)
    libGenDir := src
else ifeq ($(platform), macOS)
    libGenDir := src
endif
raylibLib = $(vendorDir)/raylib-cpp/vendor/raylib/$(libGenDir)/libraylib.a

# Set build vars for engine
engineSrcDir = .
engineBuildDir = $(buildDir)/engine
engineSources = $(call rwildcard,$(engineSrcDir)/,*.cpp)
engineObjects = $(call findobjs,$(engineSrcDir),$(engineBuildDir),$(engineSources))
engineDepends = $(patsubst %.o, %.d, $(call rwildcard,$(engineBuildDir)/,*.o))

# Build the static library for the engine
$(engineLib): $(raylibLib) include $(utilsLib) $(engineObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	ar -rc $(engineLib) $(engineObjects)
	libtool -static -o $(engineLib) $(engineLib) $(raylibLib)

# Add all rules from dependency files
-include $(engineDepends)

# Compile object files to the build directory
$(engineBuildDir)/%.o: $(engineSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I./include -I $(srcDir) $< -o $@ $(CXXFLAGS)

# Copy the relevant header files into includes
include:
	$(MKDIR) $(call platformpth, include/raylib)
	$(call COPY,$(vendorDir)/raylib-cpp/vendor/raylib/src,include/raylib,raylib.h)
	$(call COPY,$(vendorDir)/raylib-cpp/vendor/raylib/src,include/raylib,raymath.h)
	$(call COPY,$(vendorDir)/raylib-cpp/include,include/raylib,*.hpp)

# Build the raylib static library file
$(raylibLib):
	@$(MAKE) -C $(vendorDir)/raylib-cpp/vendor/raylib/src PLATFORM=PLATFORM_DESKTOP
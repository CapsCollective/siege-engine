include make/Functions.mk

# Set platform vars and overrides
include make/Platform.mk
ifeq ($(platform), Windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
    libGenDir := src
else ifeq ($(platform), Linux)
    linkFlags += -l GL -l m -l pthread -Wl,--no-as-needed -l dl -l rt -l X11
else ifeq ($(platform), macOS)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    libGenDir := src
endif

# Set build variables for common
libDir = lib
buildDir = bin
compileFlags := -Wall -std=c++17 -I ./include
linkFlags += -L $(libDir) -l engine -l raylib
depends = $(patsubst %.o, %.d, $(call rwildcard,$(buildDir)/,*.o))
buildFlagsFile = .buildflags

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	override CXXFLAGS += -g -DDEBUG
else
    override CXXFLAGS += -DNDEBUG
endif

# Set build variables for engine
engineSrcDir = src
engineBuildDir = $(buildDir)/engine
engineLib = $(libDir)/libengine.a
engineSources = $(call rwildcard,$(engineSrcDir)/,*.cpp)
engineObjects = $(call findobjs,$(engineSrcDir),$(engineBuildDir),$(engineSources))

# Set build variables for tests
testSrcDir = tests
testBuildDir = $(buildDir)/tests
testExecutable = $(testBuildDir)/test
testSources = $(call rwildcard,$(testSrcDir)/,*.cpp)
testObjects = $(call findobjs,$(testSrcDir),$(testBuildDir),$(testSources))

# Set build variables for example
exampleSrcDir = example/src
exampleBuildDir = $(buildDir)/example
exampleExecutable = $(exampleBuildDir)/example
exampleSources = $(call rwildcard,$(exampleSrcDir)/,*.cpp)
exampleObjects = $(call findobjs,$(exampleSrcDir),$(exampleBuildDir),$(exampleSources))

.PHONY: all setup submodules build buildFlags test run clean

all: build test run clean

# Sets up the project for compiling, generates includes and libs
setup: include $(libDir)

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, include/raylib)
	$(call COPY,vendor/raylib-cpp/vendor/raylib/src,include/raylib,raylib.h)
	$(call COPY,vendor/raylib-cpp/vendor/raylib/src,include/raylib,raymath.h)
	$(call COPY,vendor/raylib-cpp/include,include/raylib,*.hpp)

# Build the raylib static library file and copy it into lib
$(libDir): submodules
	cd vendor/raylib-cpp/vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP # move hte submodule
	$(MKDIR) $(call platformpth, $(libDir))
	$(call COPY,vendor/raylib-cpp/vendor/raylib/$(libGenDir),$(libDir),libraylib.a)

build: buildFlags $(engineLib) $(testExecutable) $(exampleExecutable)

buildFlags:
	@if [[ -f "$(buildFlagsFile)" && "`cat $(buildFlagsFile)`" != "$(CXXFLAGS)" ]]; then \
  		$(RM) $(call platformpth, $(buildDir)); \
        $(RM) $(call platformpth, $(engineLib)); \
    fi; echo $(CXXFLAGS) | tee $(buildFlagsFile) >/dev/null

# Build and run all tests
test: buildFlags $(testExecutable)
	$(testExecutable) $(ARGS)

run: buildFlags $(exampleExecutable)
	$(exampleExecutable) $(ARGS)

clean:
	$(RM) $(call platformpth, $(buildDir))
	$(RM) $(call platformpth, $(engineLib))
	$(RM) $(call platformpth, $(buildFlagsFile))

# Build the static library for the engine
$(engineLib): $(engineObjects)
	ar -rc $(engineLib) $(engineObjects)

# Link the object files and create an executable
$(testExecutable): $(engineLib) $(testObjects)
	$(CXX) $(testObjects) -o $(testExecutable) $(linkFlags)

# Link the object files and create an executable
$(exampleExecutable): $(engineLib) $(exampleObjects)
	$(CXX) $(exampleObjects) -o $(exampleExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

$(engineBuildDir)/%.o: $(engineSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)

# Compile object files to the build directory
$(testBuildDir)/%.o: $(testSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineSrcDir) $< -o $@ $(CXXFLAGS)

$(exampleBuildDir)/%.o: $(exampleSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(engineSrcDir) $< -o $@ $(CXXFLAGS)

format-check:
	./format.sh "$(engineSrcDir) $(exampleSrcDir) $(testSrcDir)" "*catch*" --check

format:
	./format.sh "$(engineSrcDir) $(exampleSrcDir) $(testSrcDir)" "*catch*"
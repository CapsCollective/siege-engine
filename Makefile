include make/Functions.mk
include make/Platform.mk

# Set platform vars and overrides
ifeq ($(platform), Windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
    libGenDir := src
else ifeq ($(platform), Linux)
    linkFlags += -l GL -l m -Wl,--no-as-needed -l dl -l rt -l X11 -l pthread
else ifeq ($(platform), macOS)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    libGenDir := src
endif

# Set build variables for common
export srcDir = $(abspath src)
export libDir = $(abspath lib)
export buildDir = $(abspath bin)
export makeDir = $(abspath make)
export compileFlags := -Wall -std=c++17 -I $(abspath ./include)
export linkFlags += -L $(libDir) -l utils -l engine -l raylib
export buildFlagsFile = .buildflags

# Define top level target variables
export utilsLib = $(libDir)/libutils.a
export engineLib = $(libDir)/libengine.a
export testExecutable = $(buildDir)/testapp
export exampleExecutable = $(buildDir)/exampleapp

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	override CXXFLAGS += -g -DDEBUG
else
    override CXXFLAGS += -DNDEBUG
endif

.PHONY: all setup submodules build buildFlags test run clean utils engine tests example

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

build: buildFlags tests example

buildFlags:
	@if [[ -f "$(buildFlagsFile)" && "`cat $(buildFlagsFile)`" != "$(CXXFLAGS)" ]]; then \
  		$(RM) $(call platformpth, $(buildDir)); \
        $(RM) $(call platformpth, $(utilsLib)); \
        $(RM) $(call platformpth, $(engineLib)); \
    fi; echo $(CXXFLAGS) | tee $(buildFlagsFile) >/dev/null

# Build and run all tests
test: buildFlags tests
	$(testExecutable) $(ARGS)

run: buildFlags example
	$(exampleExecutable) $(ARGS)

clean:
	$(RM) $(call platformpth, $(buildDir))
	$(RM) $(call platformpth, $(utilsLib))
	$(RM) $(call platformpth, $(engineLib))
	$(RM) $(call platformpth, $(buildFlagsFile))

utils:
	$(MAKE) -C $(srcDir)/utils CXXFLAGS="$(CXXFLAGS)"

engine: utils
	$(MAKE) -C $(srcDir)/engine CXXFLAGS="$(CXXFLAGS)"

tests: utils engine
	$(MAKE) -C tests CXXFLAGS="$(CXXFLAGS)"

example: utils engine
	$(MAKE) -C example CXXFLAGS="$(CXXFLAGS)"

format-check:
	./format.sh "$(srcDir) example/src tests" "*catch*" --check

format:
	./format.sh "$(srcDir) example/src tests" "*catch*"
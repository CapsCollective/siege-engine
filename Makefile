include make/Functions.mk
include make/Platform.mk

# Set platform vars
ifeq ($(platform), Windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
else ifeq ($(platform), Linux)
    linkFlags += -l GL -l m -Wl,--no-as-needed -l dl -l rt -l X11 -l pthread
else ifeq ($(platform), macOS)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

# Set directories
export srcDir = $(abspath src)
export libDir = $(abspath lib)
export makeDir = $(abspath make)
export buildDir = $(abspath bin)
export vendorDir = $(abspath vendor)

# Set build vars
export compileFlags := -Wall -std=c++17 -I $(abspath ./include)
export linkFlags += -L $(libDir) -l utils -l engine
buildFlagsFile := .buildflags

# Set top level targets
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

.PHONY: all test run utils engine tests example buildFlags clean format-check format

all: tests example test run clean

# Build and run the application
run: buildFlags example
	$(exampleExecutable) $(ARGS)

# Build and run all tests
test: buildFlags tests
	$(testExecutable) $(ARGS)

utils: buildFlags
	@$(MAKE) -C $(srcDir)/utils CXXFLAGS="$(CXXFLAGS)"

engine: buildFlags utils
	@$(MAKE) -C $(srcDir)/engine CXXFLAGS="$(CXXFLAGS)"

tests: buildFlags utils engine
	@$(MAKE) -C tests CXXFLAGS="$(CXXFLAGS)"

example: buildFlags utils engine
	@$(MAKE) -C example CXXFLAGS="$(CXXFLAGS)"

# Check to invalidate the build if flags have changed
buildFlags:
	@if [[ -f "$(buildFlagsFile)" && "`cat $(buildFlagsFile)`" != "$(CXXFLAGS)" ]]; then \
  		$(RM) $(call platformpth, $(libDir)); \
  		$(RM) $(call platformpth, $(buildDir)); \
    fi; echo $(CXXFLAGS) | tee $(buildFlagsFile) >/dev/null

# Run cleanup across project-wide
clean:
	$(RM) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(buildDir))
	$(RM) $(call platformpth, $(buildFlagsFile))

# Check file formatting across all source files
format-check:
	./format.sh "$(srcDir) example/src tests" "*catch*" --check

# Run file formatting across all source files
format:
	./format.sh "$(srcDir) example/src tests" "*catch*"
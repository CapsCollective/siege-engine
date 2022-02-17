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
export libDir := $(abspath lib)
export makeDir := $(abspath make)
export buildDir := $(abspath bin)
export vendorDir := $(abspath vendor)
export engineDir := $(abspath engine)
export testsDir := $(abspath tests)
export examplesDir := $(abspath examples)
export vendorIncludeDir := $(vendorDir)/include

# Set build vars
export compileFlags := -Wall -std=c++17
export linkFlags += -L $(libDir) -l utils -l core
buildFlagsFile := .buildflags

# Set top level targets
export utilsLib := $(libDir)/libutils.a
export coreLib := $(libDir)/libcore.a

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	override CXXFLAGS += -g -DDEBUG
else
    override CXXFLAGS += -DNDEBUG
endif

.PHONY: all utils core tests examples examples/game buildExample/% buildFlags clean format

all: tests examples

utils: buildFlags
	$(MAKE) -C $(engineDir)/utils CXXFLAGS="$(CXXFLAGS)"

core: buildFlags
	$(MAKE) -C $(engineDir)/core CXXFLAGS="$(CXXFLAGS)"

tests: buildFlags utils core
	$(MAKE) -C $(testsDir) CXXFLAGS="$(CXXFLAGS)"

examples: examples/game

examples/game: buildFlags utils core buildExample/game

buildExample/%:
	$(MAKE) -C $(examplesDir)/$* CXXFLAGS="$(CXXFLAGS)"

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

# Run file formatting program across all source files
format:
	./format.sh "$(engineDir) $(examplesDir) $(testsDir)" $(ARGS)
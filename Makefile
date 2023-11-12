# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

include make/Functions.mk
include make/Platform.mk

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
    override CXXFLAGS += -g -DDEBUG -DCC_LOG_LEVEL=2
else
    override CXXFLAGS += -DNDEBUG -DCC_LOG_LEVEL=0 -O3
endif

# Set platform vars
ifeq ($(platform), windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
    export packageScript = powershell -executionpolicy bypass $(scriptsDir)/package.ps1 --pkg-name $1 --exe-name $2 --output-dir $3 --build-dir $4 $5
else ifeq ($(platform), linux)
    linkFlags += -Wl,--no-as-needed -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -no-pie
    formatScript = $(scriptsDir)/format.sh
else ifeq ($(platform), macos)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    formatScript = $(scriptsDir)/format.sh
endif

# Set directories
export libDir := $(abspath lib)
export makeDir := $(abspath make)
export binDir := $(abspath bin)
export vendorDir := $(abspath vendor)
export scriptsDir := $(abspath scripts)
export outputDir := $(abspath output)
export engineDir := $(abspath engine)
export testsDir := $(abspath tests)
export examplesDir := $(abspath examples)

# Set top level targets
export utilsLib := $(libDir)/libutils.a
export windowLib := $(libDir)/libwindow.a
export coreLib := $(libDir)/libcore.a
export renderLib := $(libDir)/librender.a
export testApp := $(binDir)/tests/build/app
export exampleGameApp := $(binDir)/examples/game/build/app
export exampleRenderApp := $(binDir)/examples/render/build/app
export exampleTilemapApp := $(binDir)/examples/tilemap/build/app

# Set build vars
export compileFlags := -Wall -std=c++17
export linkFlags += -L $(libDir)
buildFlagsFile:=.buildflags

.PHONY: all testapp gameapp renderapp tilemapapp package-gameapp package-renderapp package-tilemap buildFlags clean format

all: testapp package-gameapp package-renderapp package-tilemap

$(utilsLib): buildFlags
	"$(MAKE)" -C $(engineDir)/utils CXXFLAGS="$(CXXFLAGS)"

$(windowLib): buildFlags $(utilsLib)
	"$(MAKE)" -C $(engineDir)/window CXXFLAGS="$(CXXFLAGS)"

$(coreLib): buildFlags $(utilsLib)
	"$(MAKE)" -C $(engineDir)/core CXXFLAGS="$(CXXFLAGS)"

$(renderLib): buildFlags $(utilsLib) $(windowLib)
	"$(MAKE)" -C $(engineDir)/render CXXFLAGS="$(CXXFLAGS)"

$(testApp): buildFlags $(utilsLib) $(coreLib)
	"$(MAKE)" -C $(testsDir) CXXFLAGS="$(CXXFLAGS)"

$(exampleGameApp): buildFlags $(renderLib) $(coreLib)
	"$(MAKE)" -C $(examplesDir)/game CXXFLAGS="$(CXXFLAGS)"

$(exampleRenderApp): buildFlags $(renderLib)
	"$(MAKE)" -C $(examplesDir)/render CXXFLAGS="$(CXXFLAGS)"

$(exampleTilemapApp): buildFlags $(renderLib)
	"$(MAKE)" -C $(examplesDir)/tilemap CXXFLAGS="$(CXXFLAGS)"

testapp: $(testApp)

gameapp: $(exampleGameApp)

renderapp: $(exampleRenderApp)

tilemapapp: $(exampleTilemapApp)

package-gameapp: gameapp
	"$(MAKE)" package -C $(examplesDir)/game CXXFLAGS="$(CXXFLAGS)"

package-renderapp: renderapp
	"$(MAKE)" package -C $(examplesDir)/render CXXFLAGS="$(CXXFLAGS)"

package-tilemap: tilemapapp
	"$(MAKE)" package -C $(examplesDir)/tilemap CXXFLAGS="$(CXXFLAGS)"

# Check to invalidate the build if flags have changed
buildFlags:
	$(call BUILD_FLAGS_SCRIPT, $(buildFlagsFile), $(CXXFLAGS), $(libDir) $(binDir))

# Run cleanup across project
clean:
	$(RM) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(binDir))
	$(RM) $(call platformpth, $(outputDir))
	$(RM) $(call platformpth, $(buildFlagsFile))

# Check file formatting program across all source files
format-check:
	$(formatScript) "$(engineDir) $(examplesDir) $(testsDir)" --check

# Run file formatting program across all source files
format:
	$(formatScript) "$(engineDir) $(examplesDir) $(testsDir)"

include make/Functions.mk
include make/Platform.mk

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	override CXXFLAGS += -g -DDEBUG -DCC_LOG_LEVEL=2
else
    override CXXFLAGS += -DNDEBUG -DCC_LOG_LEVEL=0
endif

# Set platform vars
ifeq ($(platform), windows)
	glslangValidator = $(vendorDir)\glslang\build\install\bin\glslangValidator.exe
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
    export packageScript = $(scriptsDir)/package.bat
else ifeq ($(platform), linux)
    linkFlags += -l GL -l m -Wl,--no-as-needed -l dl -l rt -l X11 -l pthread
    formatScript = $(scriptsDir)/format.sh
    export packageScript = $(scriptsDir)/package.sh
else ifeq ($(platform), macos)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    formatScript = $(scriptsDir)/format.sh
    export packageScript = $(scriptsDir)/package.sh
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
export coreLib := $(libDir)/libcore.a
export renderLib := $(libDir)/librender.a
export testApp := $(binDir)/tests/build/app
export exampleGameApp := $(binDir)/examples/game/build/app
export exampleRenderApp := $(binDir)/examples/render/build/app

# Set build vars
export compileFlags := -Wall -std=c++17
export linkFlags += -L $(libDir)
buildFlagsFile := .buildflags

.PHONY: all testapp gameapp renderapp package-gameapp package-renderapp buildFlags clean format

all: testapp package-gameapp package-renderapp

$(utilsLib): buildFlags
	$(MAKE) -C $(engineDir)/utils CXXFLAGS="$(CXXFLAGS)"

$(coreLib): buildFlags
	$(MAKE) -C $(engineDir)/core CXXFLAGS="$(CXXFLAGS)"

$(renderLib): buildFlags
	$(MAKE) -C $(engineDir)/render CXXFLAGS="$(CXXFLAGS)"

$(testApp): buildFlags $(utilsLib) $(coreLib)
	$(MAKE) -C $(testsDir) CXXFLAGS="$(CXXFLAGS)"

$(exampleGameApp): buildFlags $(utilsLib) $(coreLib)
	$(MAKE) -C $(examplesDir)/game CXXFLAGS="$(CXXFLAGS)"

$(exampleRenderApp): buildFlags $(renderLib)
	$(MAKE) -C $(examplesDir)/render CXXFLAGS="$(CXXFLAGS)"

testapp: $(testApp)

gameapp: $(exampleGameApp)

renderapp: $(exampleRenderApp)

package-gameapp: gameapp
	$(MAKE) package -C $(examplesDir)/game CXXFLAGS="$(CXXFLAGS)"

package-renderapp: renderapp
	$(MAKE) package -C $(examplesDir)/render CXXFLAGS="$(CXXFLAGS)"

# Check to invalidate the build if flags have changed
buildFlags:
	@if [[ -f "$(buildFlagsFile)" && "`cat $(buildFlagsFile)`" != "$(CXXFLAGS)" ]]; then \
  		$(RM) $(call platformpth, $(libDir)); \
  		$(RM) $(call platformpth, $(binDir)); \
    fi; echo $(CXXFLAGS) | tee $(buildFlagsFile) >/dev/null

# Run cleanup across project
clean:
	$(RM) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(binDir))
	$(RM) $(call platformpth, $(outputDir))
	$(RM) $(call platformpth, $(buildFlagsFile))

# Run file formatting program across all source files
format:
	$(formatScript) "$(engineDir) $(examplesDir) $(testsDir)" $(ARGS)
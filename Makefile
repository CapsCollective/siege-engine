include make/Functions.mk
include make/Platform.mk

# Set platform vars
ifeq ($(platform), windows)
	glslangValidator = $(vendorDir)\glslang\build\install\bin\glslangValidator.exe
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
    packageScript := $(scriptsDir)/package.bat
else ifeq ($(platform), linux)
    linkFlags += -l GL -l m -Wl,--no-as-needed -l dl -l rt -l X11 -l pthread
    formatScript := $(scriptsDir)/format.sh
    packageScript := $(scriptsDir)/package.sh
else ifeq ($(platform), macos)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    formatScript := $(scriptsDir)/format.sh
    packageScript := $(scriptsDir)/package.sh
endif

# Set directories
export libDir := $(abspath lib)
export makeDir := $(abspath make)
export buildDir := $(abspath bin)
export vendorDir := $(abspath vendor)
export scriptsDir := $(abspath scripts)
export outputDir := $(abspath output)
export engineDir := $(abspath engine)
export testsDir := $(abspath tests)
export examplesDir := $(abspath examples)
export vendorIncludeDir := $(vendorDir)/include

# Set build vars
export compileFlags := -Wall -std=c++17
export linkFlags += -L $(libDir)
buildFlagsFile := .buildflags

# Set top level targets
export utilsLib := $(libDir)/libutils.a
export coreLib := $(libDir)/libcore.a
export renderLib := $(libDir)/librender.a
export testExecutable := $(buildDir)/testapp
export exampleGameExecutable := $(buildDir)/examplegameapp
export exampleRenderExecutable := $(buildDir)/examplerenderapp

# Set debugging build flags
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	override CXXFLAGS += -g -DDEBUG -DCC_LOG_LEVEL=2
else
    override CXXFLAGS += -DNDEBUG -DCC_LOG_LEVEL=0
endif

.PHONY: all buildFlags clean format

all: $(testExecutable) $(exampleGameExecutable) $(exampleRenderExecutable)

$(utilsLib): buildFlags
	$(MAKE) -C $(engineDir)/utils CXXFLAGS="$(CXXFLAGS)"

$(coreLib): buildFlags
	$(MAKE) -C $(engineDir)/core CXXFLAGS="$(CXXFLAGS)"

$(renderLib): buildFlags
	$(MAKE) -C $(engineDir)/render CXXFLAGS="$(CXXFLAGS)"

$(testExecutable): buildFlags $(utilsLib) $(coreLib)
	$(MAKE) -C $(testsDir) CXXFLAGS="$(CXXFLAGS)"

$(exampleGameExecutable): buildFlags $(utilsLib) $(coreLib)
	$(MAKE) -C $(examplesDir)/game CXXFLAGS="$(CXXFLAGS)"

$(exampleRenderExecutable): buildFlags $(renderLib)
	$(MAKE) -C $(examplesDir)/render CXXFLAGS="$(CXXFLAGS)"

testapp: $(testExecutable)

gameapp: $(exampleGameExecutable)

renderapp: $(exampleRenderExecutable)

# Check to invalidate the build if flags have changed
buildFlags:
	@if [[ -f "$(buildFlagsFile)" && "`cat $(buildFlagsFile)`" != "$(CXXFLAGS)" ]]; then \
  		$(RM) $(call platformpth, $(libDir)); \
  		$(RM) $(call platformpth, $(buildDir)); \
    fi; echo $(CXXFLAGS) | tee $(buildFlagsFile) >/dev/null

# Run cleanup across project
clean:
	$(RM) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(buildDir))
	$(RM) $(call platformpth, $(outputDir))
	$(RM) $(call platformpth, $(buildFlagsFile))

# Run file formatting program across all source files
format:
	$(formatScript) "$(engineDir) $(examplesDir) $(testsDir)" $(ARGS)

package:
	$(packageScript) "A Dark Discomfort" $(outputDir) $(buildDir) $(PACKAGE_FLAGS)
include ../make/Functions.mk

# Set platform vars and overrides
include ../make/Platform.mk
ifeq ($(platform), Windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
else ifeq ($(platform), Linux)
    linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
else ifeq ($(platform), macOS)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

# Set build vars and overrides
include ../make/BuildVars.mk
srcDir = .
target = $(buildDir)/test
compileFlags += -I ../include -I ../src
linkFlags += -L ../lib/$(platform) -l engine -l raylib

.PHONY: all run clean

all: run clean

# Set build rules and overrides
include ../make/BuildRules.mk

run: $(target)
	$(target) $(ARGS)

clean:
	$(RM) $(call platformpth, $(buildDir)/*)

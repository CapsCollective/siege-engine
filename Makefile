include vendor/engine/make/Functions.mk

# Set platform vars and overrides
include vendor/engine/make/Platform.mk
ifeq ($(platform), Windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
else ifeq ($(platform), Linux)
    linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
else ifeq ($(platform), macOS)
    linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

# Set build vars and overrides
include vendor/engine/make/BuildVars.mk
compileFlags += -I vendor/engine/include -I vendor/engine/src
linkFlags += -L vendor/engine/lib/$(platform) -l engine -l raylib

.PHONY: all setup build engine run clean

all: run clean

# Sets up the project and deps
setup:
	cd vendor/engine $(THEN) "$(MAKE)" setup $(passFlags)

# Set build rules and overrides
include vendor/engine/make/BuildRules.mk
$(target): engine $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

build: $(target)

engine:
	cd vendor/engine $(THEN) "$(MAKE)" lib/$(platform)/libengine.a $(passFlags)

run: $(target)
	$(target) $(ARGS)

clean:
	$(RM) $(call platformpth, $(buildDir)/*)
	cd vendor/engine $(THEN) "$(MAKE)" clean

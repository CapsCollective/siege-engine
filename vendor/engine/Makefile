include make/Functions.mk

# Set platform vars and overrides
include make/Platform.mk
ifneq ($(platform), Linux)
    libGenDir := src
endif

# Set build vars and overrides
include make/BuildVars.mk
target = lib/$(platform)/libengine.a
compileFlags += -I ./include

.PHONY: all setup submodules clean

all: $(target) clean

# Sets up the project for compiling, generates includes and libs
setup: include lib

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, ./include/raylib)
	$(call COPY,vendor/raylib-cpp/vendor/raylib/src,./include/raylib,raylib.h)
	$(call COPY,vendor/raylib-cpp/vendor/raylib/src,./include/raylib,raymath.h)
	$(call COPY,vendor/raylib-cpp/include,./include/raylib,*.hpp)

# Build the raylib static library file and copy it into lib
lib: submodules
	cd vendor/raylib-cpp/vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP $(passFlags)
	$(MKDIR) $(call platformpth, lib/$(platform))
	$(call COPY,vendor/raylib-cpp/vendor/raylib/$(libGenDir),lib/$(platform),libraylib.a)

# Set build rules and overrides
include make/BuildRules.mk
$(target): $(objects)
	ar -r $(target) $(objects)

# Build and run all tests
test: $(target)
	cd ./tests $(THEN) "$(MAKE)" bin/test run $(passFlags) ARGS=$(ARGS)

clean:
	$(RM) $(call platformpth, $(buildDir)/*)
	cd ./tests $(THEN) "$(MAKE)" clean

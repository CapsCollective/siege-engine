include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set source build vars
exampleRenderSrcDir := .
exampleRenderBinDir := $(binDir)/examples/render
exampleRenderSources := $(call rwildcard,$(exampleRenderSrcDir)/,*.cpp)
exampleRenderObjects := $(call findobjs,$(exampleRenderSrcDir),$(exampleRenderBinDir),$(exampleRenderSources))
exampleRenderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleRenderBinDir)/,*.o))
exampleRenderBuildDir := $(exampleRenderBinDir)/build
exampleRenderOutputName := "Render Example"

# Set packaging vars
ifeq ($(platform), linux)
    packagingEnvVars := \
    	export DYLD_LIBRARY_PATH='./lib'; \
    	export VK_LAYER_PATH='./lib/explicit_layer.d';
else ifeq ($(platform), macos)
    packagingEnvVars := \
		export DYLD_LIBRARY_PATH='./lib'; \
		export VK_LAYER_PATH='./lib/explicit_layer.d'; \
		export VK_ICD_FILENAMES='./lib/icd.d/MoltenVK_icd.json'
endif
ifneq ($(ENABLE_VALIDATION_LAYERS), 1)
    packagingExcludes := lib/explicit_layer.d
endif
packagingFlags = --env-vars "$(packagingEnvVars)" --excludes "$(packagingExcludes)"

# Set build vars
ifeq ($(platform), windows)
    linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
else ifeq ($(platform), linux)
	linkFlags += -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -no-pie
else ifeq ($(platform), macos)
	linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif
compileFlags += -I $(vendorDir)/vulkan/include -I $(vendorDir)/glfw/include -I $(vendorDir)/glm -I $(vendorDir)/tinyobjloader
linkFlags += -lrender

.PHONY: all render-assets

all: $(exampleRenderApp) render-assets

# Link the object files and create an executable
$(exampleRenderApp): $(renderLib) $(exampleRenderObjects)
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) $(exampleRenderObjects) -o $(exampleRenderApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleRenderDepends)

# Compile object files to the bin directory
$(exampleRenderBinDir)/%.o: $(exampleRenderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I$(engineDir) $< -o $@ $(CXXFLAGS)

# Copy assets (and other required build files) directory to the build directory
render-assets:
	$(MKDIR) $(call platformpth,$(exampleRenderBuildDir))
	$(call COPY_DIR,$(binDir)/engine/render/build,$(exampleRenderBuildDir))
	$(MKDIR) $(call platformpth,$(exampleRenderBuildDir)/assets)
	$(call COPY,$(exampleRenderSrcDir)/assets,$(exampleRenderBuildDir)/assets,**)

# Package the built application and all its assets to the output directory
package:
	$(RM) "$(outputDir)/$(exampleRenderOutputName)"*
	$(packageScript) $(exampleRenderOutputName) $(shell basename $(exampleRenderApp)) $(outputDir) $(exampleRenderBuildDir) $(packagingFlags)
include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for exampleRender
exampleRenderSrcDir := .
exampleRenderBinDir := $(binDir)/examples/render
exampleRenderSources := $(call rwildcard,$(exampleRenderSrcDir)/,*.cpp)
exampleRenderObjects := $(call findobjs,$(exampleRenderSrcDir),$(exampleRenderBinDir),$(exampleRenderSources))
exampleRenderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleRenderBinDir)/,*.o))
exampleRenderBuildDir := $(exampleRenderBinDir)/build
engineRenderBuildDir := $(binDir)/engine/render/build
exampleRenderOutputName := "Render Example"

# Set packaging vars for exampleRender
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

# Link the object files and create an executable
$(exampleRenderApp): $(exampleRenderObjects) $(exampleRenderBuildDir)
	$(CXX) $(exampleRenderObjects) -o $(exampleRenderApp) $(linkFlags)

# Add all rules from dependency files
-include $(exampleRenderDepends)

# Compile object files to the build directory
$(exampleRenderBinDir)/%.o: $(exampleRenderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I$(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the output files to the build directory
$(exampleRenderBuildDir):
	$(MKDIR) $(call platformpth,$(exampleRenderBuildDir))
	$(call COPY_DIR,$(engineRenderBuildDir),$(exampleRenderBuildDir))
	$(MKDIR) $(call platformpth,$(exampleRenderBuildDir)/assets)
	$(call COPY_DIR,assets,$(exampleRenderBuildDir)/assets)

package:
	$(RM) "$(outputDir)/$(exampleRenderOutputName)"*
	$(packageScript) $(exampleRenderOutputName) $(shell basename $(exampleRenderApp)) $(outputDir) $(exampleRenderBuildDir) $(packagingFlags)
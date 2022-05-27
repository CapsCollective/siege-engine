include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for exampleRender
exampleRenderSrcDir := .
exampleRenderBuildDir := $(buildDir)/examples/render
exampleRenderSources := $(call rwildcard,$(exampleRenderSrcDir)/,*.cpp)
exampleRenderObjects := $(call findobjs,$(exampleRenderSrcDir),$(exampleRenderBuildDir),$(exampleRenderSources))
exampleRenderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(exampleRenderBuildDir)/,*.o))
assetsDir := assets
assetsBuildDir := $(buildDir)/$(assetsDir)

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
$(exampleRenderExecutable): $(exampleRenderObjects) $(assetsBuildDir)
	$(CXX) $(exampleRenderObjects) -o $(exampleRenderExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(exampleRenderDepends)

# Compile object files to the build directory
$(exampleRenderBuildDir)/%.o: $(exampleRenderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I$(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the assets directory to the build directory
$(assetsBuildDir):
	$(MKDIR) $(call platformpth,$(assetsBuildDir))
	$(call COPY_DIR,$(assetsDir),$(assetsBuildDir))
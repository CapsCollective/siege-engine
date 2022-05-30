include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for render
renderSrcDir := .
renderBinDir := $(binDir)/engine/render
renderSources := $(call rwildcard,$(renderSrcDir)/,*.cpp)
renderObjects := $(call findobjs,$(renderSrcDir),$(renderBinDir),$(renderSources))
renderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(renderBinDir)/,*.o))
renderBuildDir := $(renderBinDir)/build
libBinDir := $(renderBuildDir)/lib

# Find shader values for build
shadersDir := shaders
vertSources = $(call rwildcard,$(shadersDir),*.vert)
fragSources = $(call rwildcard,$(shadersDir),*.frag)
vertObjFiles = $(patsubst %.vert,$(renderBuildDir)/%.vert.spv,$(vertSources))
fragObjFiles = $(patsubst %.frag,$(renderBuildDir)/%.frag.spv,$(fragSources))

# Set build vars
ifeq ($(platform), windows)
    volkDefines = VK_USE_PLATFORM_WIN32_KHR
    glslangValidator := $(vendorDir)\glslang\build\install\bin\glslangValidator.exe
else ifeq ($(platform), linux)
	volkDefines = VK_USE_PLATFORM_XLIB_KHR
	glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator
else ifeq ($(platform), macos)
	volkDefines = VK_USE_PLATFORM_MACOS_MVK
	glslangValidator := $(vendorDir)/glslang/build/install/bin/glslangValidator
endif
compileFlags += -I $(vendorDir)/vulkan/include -I $(vendorDir)/glfw/include -I $(vendorDir)/glm -I $(vendorDir)/tinyobjloader

# Build the static library for render
$(renderLib): $(glfwLib) $(vertObjFiles) $(fragObjFiles) $(libBinDir) $(renderObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(renderLib))
	$(call COPY,$(vendorDir)/glfw/src,$(libDir)/librender.a,libglfw3.a)
	ar -rs $(renderLib) $(renderObjects)

# Add all rules from dependency files
-include $(renderDepends)

# Compile object files to the build directory
$(renderBinDir)/%.o: $(renderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

$(libBinDir):
	$(MKDIR) $(call platformpth,$(libBinDir))
	$(call COPY_DIR,$(vendorDir)/vulkan/lib,$(libBinDir))

$(renderBuildDir)/%.spv: %
	$(MKDIR) $(call platformpth, $(@D))
	$(glslangValidator) $< -V -o $@
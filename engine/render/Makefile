include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set validation layer build flags
ifeq ($(ENABLE_VALIDATION_LAYERS), 1)
    PACKAGE_FLAGS := --include-validation-layers
endif

# Set build vars for render
renderSrcDir := .
renderBuildDir := $(buildDir)/render
renderSources := $(call rwildcard,$(renderSrcDir)/,*.cpp)
renderObjects := $(call findobjs,$(renderSrcDir),$(renderBuildDir),$(renderSources))
renderDepends := $(patsubst %.o, %.d, $(call rwildcard,$(renderBuildDir)/,*.o))

# Find shader values for build
shadersDir := shaders
vertSources = $(call rwildcard,$(shadersDir),*.vert)
fragSources = $(call rwildcard,$(shadersDir),*.frag)
vertObjFiles = $(patsubst %.vert,$(buildDir)/%.vert.spv,$(vertSources))
fragObjFiles = $(patsubst %.frag,$(buildDir)/%.frag.spv,$(fragSources))

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
$(renderLib): $(glfwLib) $(vertObjFiles) $(fragObjFiles) $(buildDir)/lib $(renderObjects)
	$(MKDIR) $(call platformpth, $(libDir))
	$(RM) $(call platformpth, $(renderLib))
	$(call COPY,$(vendorDir)/glfw/src,$(libDir)/librender.a,libglfw3.a)
	ar -rs $(renderLib) $(renderObjects)

# Add all rules from dependency files
-include $(renderDepends)

# Compile object files to the build directory
$(renderBuildDir)/%.o: $(renderSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS) -D$(volkDefines)

$(buildDir)/lib:
	$(MKDIR) $(call platformpth,$(buildDir)/lib)
	$(call COPY_DIR,$(vendorDir)/vulkan/lib,$(buildDir)/lib)

$(buildDir)/%.spv: %
	$(MKDIR) $(call platformpth, $(@D))
	$(glslangValidator) $< -V -o $@
include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for tests
testSrcDir := .
testBuildDir := $(buildDir)/tests
testSources := $(call rwildcard,$(testSrcDir)/,*.cpp)
testObjects := $(call findobjs,$(testSrcDir),$(testBuildDir),$(testSources))
testDepends := $(patsubst %.o, %.d, $(call rwildcard,$(testBuildDir)/,*.o))
dataDir := data
dataBuildDir := $(buildDir)/tests/$(dataDir)

# Set build vars
utestIncludeDir := $(vendorIncludeDir)/utest
linkFlags += -l core -l utils

# Link the object files and create an executable
$(testExecutable): $(utestIncludeDir) $(testObjects) $(dataBuildDir)
	$(CXX) $(testObjects) -o $(testExecutable) $(linkFlags)

# Add all rules from dependency files
-include $(testDepends)

# Compile object files to the build directory
$(testBuildDir)/%.o: $(testSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(utestIncludeDir) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the relevant header files into includes
$(utestIncludeDir):
	$(MKDIR) $(call platformpth, $(utestIncludeDir))
	$(call COPY,$(vendorDir)/utest.h,$(utestIncludeDir),utest.h)

# Copy the data directory to the build directory
$(dataBuildDir):
	$(MKDIR) $(call platformpth,$(dataBuildDir))
	$(call COPY_DIR,$(dataDir),$(dataBuildDir))
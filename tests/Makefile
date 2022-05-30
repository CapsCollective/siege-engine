include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for tests
testSrcDir := .
testBinDir := $(binDir)/tests
testSources := $(call rwildcard,$(testSrcDir)/,*.cpp)
testObjects := $(call findobjs,$(testSrcDir),$(testBinDir),$(testSources))
testDepends := $(patsubst %.o, %.d, $(call rwildcard,$(testBinDir)/,*.o))
dataDir := data
testBuildDir := $(testBinDir)/build
dataBinDir := $(testBuildDir)/$(dataDir)

# Set build vars
utestIncludeDir := $(vendorDir)/include/utest
linkFlags += -l core -l utils

# Link the object files and create an executable
$(testApp): $(utestIncludeDir) $(testObjects) $(dataBinDir)
	$(CXX) $(testObjects) -o $(testApp) $(linkFlags)

# Add all rules from dependency files
-include $(testDepends)

# Compile object files to the build directory
$(testBinDir)/%.o: $(testSrcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) -I $(utestIncludeDir) -I $(engineDir) $< -o $@ $(CXXFLAGS)

# Copy the relevant header files into includes
$(utestIncludeDir):
	$(MKDIR) $(call platformpth, $(utestIncludeDir))
	$(call COPY,$(vendorDir)/utest.h,$(utestIncludeDir),utest.h)

# Copy the data directory to the build directory
$(dataBinDir):
	$(MKDIR) $(call platformpth,$(dataBinDir))
	$(call COPY_DIR,$(dataDir),$(dataBinDir))
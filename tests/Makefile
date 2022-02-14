include $(makeDir)/Functions.mk
include $(makeDir)/Platform.mk

# Set build vars for tests
testSrcDir = .
testExecutable = $(buildDir)/testapp
testBuildDir = $(buildDir)/tests
testSources = $(call rwildcard,$(testSrcDir)/,*.cpp)
testObjects = $(call findobjs,$(testSrcDir),$(testBuildDir),$(testSources))
testDepends = $(patsubst %.o, %.d, $(call rwildcard,$(testBuildDir)/,*.o))
utestIncludeDir = $(vendorIncludeDir)/utest

# Link the object files and create an executable
$(testExecutable): $(utestIncludeDir) $(testObjects)
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
# Link the object files and create an executable
$(target)-default: $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile object files to the build directory
$(buildDir)/%.o-default: $(srcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $(patsubst %-default, %, $@) $(CXXFLAGS)

# Redirect unfound object targets to defaults
%.o: %.o-default
	@ true

# Redirect unfound misc targets to defaults
%: %-default
	@ true
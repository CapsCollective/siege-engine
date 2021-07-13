# Link the object files and create an executable
$(target): $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile object files to the build directory
$(buildDir)/%.o: $(srcDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)
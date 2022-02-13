# Define custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)
findobjs = $(patsubst $1/%, $2/%, $(patsubst %.cpp, %.o, $3))
passmake = $(MAKE) -C $1 CXXFLAGS="$(CXXFLAGS)"
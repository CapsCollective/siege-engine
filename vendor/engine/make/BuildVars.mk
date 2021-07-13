# Set build variables
srcDir = src
buildDir = bin
target = $(buildDir)/app
sources = $(call rwildcard,$(srcDir)/,*.cpp)
objects = $(patsubst $(srcDir)/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends = $(patsubst %.o, %.d, $(objects))
compileFlags := -Wall -std=c++17
passFlags := CXX=$(CXX) CXXFLAGS=$(CXXFLAGS)
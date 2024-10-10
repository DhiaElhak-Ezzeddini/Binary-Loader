CXX = g++
OBJ = test

.PHONY: all clean

# Specify the C++ standard
CXXFLAGS = -std=c++11

# Define the object files
OBJS = loader.o

# Default target
all: $(OBJ)

# Compile the loader object file
loader.o: loader.cc loader.h
	$(CXX) $(CXXFLAGS) -c loader.cc

# Link the test executable with the object files
$(OBJ): $(OBJS) test.cc
	$(CXX) $(CXXFLAGS) -o $(OBJ) test.cc $(OBJS) -lbfd

# Clean up the build artifacts
clean:
	rm -f $(OBJ) $(OBJS)


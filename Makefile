# Makefile for Solar System OpenGL Simulation

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Source files
SRC = mainprojet.cpp

# Output executable
TARGET = main

# Platform-specific settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIBS = -lGL -lGLU -lglut -lm
endif
ifeq ($(UNAME_S),Darwin)
    LIBS = -framework OpenGL -framework GLUT
endif

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run the simulation
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

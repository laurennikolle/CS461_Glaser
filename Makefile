# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude -std=c++11

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Target executable name
TARGET = $(BIN_DIR)/SimpleShell

# Source files
SRCS = $(SRC_DIR)/SimpleShell.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target: compile and run
all: compile run

# Compile the program
compile: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the compiled program
run:
	./$(TARGET)

# Clean up object files and executable
clean:
	rm -f $(SRC_DIR)/*.o
	rm -rf $(BIN_DIR)

.PHONY: all compile run clean

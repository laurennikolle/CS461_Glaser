# Lauren Glaser
# PS02
# CS 461

# Compiler and flags [Lauren Glaser 10/27/2024]
CXX = g++
CXXFLAGS = -Iinclude -std=c++11

# Creates the directories [Lauren Glaser 10/27/2024]
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Creates the target executable name var [Lauren Glaser 10/27/2024]
TARGET = $(BIN_DIR)/SimpleShell

# Creates the source files var [Lauren Glaser 10/27/2024]
SRCS = $(SRC_DIR)/SimpleShell.cpp

# Created the object files var [Lauren Glaser 10/27/2024]
OBJS = $(SRCS:.cpp=.o)

# Sets teh default target [Lauren Glaser 10/27/2024]
all: compile run

# This compiles the program [Lauren Glaser 10/27/2024]
compile: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# This compiles object files [Lauren Glaser 10/27/2024]
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Runs the compiled program [Lauren Glaser 10/27/2024]
run:
	./$(TARGET)

# Cleans up object files [Lauren Glaser 10/27/2024]
clean:
	rm -f $(SRC_DIR)/*.o
	rm -rf $(BIN_DIR)

.PHONY: all compile run clean

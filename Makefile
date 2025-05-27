# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# SFML libraries to link for GUI support
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directory structure
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# File paths
MAIN_SRC = $(SRC_DIR)/main.cpp
TEST_SRC = tests.cpp
DOCTEST = doctest.h

# Object and target executables
MAIN_OBJ = $(OBJ_DIR)/main.o
TEST_OBJ = $(OBJ_DIR)/tests.o
TARGET_MAIN = $(BIN_DIR)/Main
TARGET_TEST = $(BIN_DIR)/Tests

# Gather all .cpp files in src/, excluding main.cpp
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
SOURCES := $(filter-out $(MAIN_SRC), $(SOURCES))
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Default target
all: Main

# Build and run the main GUI app
Main: $(MAIN_OBJ) $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET_MAIN) $(SFML_FLAGS)
	./$(TARGET_MAIN)

# Build and run tests
test: $(TEST_OBJ) $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET_TEST)
	./$(TARGET_TEST)

# Run valgrind memory checks
valgrind: Main test
	# valgrind --leak-check=full $(TARGET_MAIN)
	valgrind --leak-check=full $(TARGET_TEST)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(MAIN_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/tests.o: $(TEST_SRC) $(DOCTEST) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure output folders exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

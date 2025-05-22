CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TEST_OBJ = $(OBJ_DIR)/tests.o
MAIN_OBJ = $(OBJ_DIR)/main.o

TARGET_MAIN = $(BIN_DIR)/Main
TARGET_TEST = $(BIN_DIR)/Tests

DOCTEST = doctest.h

all: Main

Main: $(MAIN_OBJ) $(filter-out $(TEST_OBJ), $(OBJECTS)) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET_MAIN)
	./$(TARGET_MAIN)

test: $(TEST_OBJ) $(filter-out $(MAIN_OBJ), $(OBJECTS)) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET_TEST)
	./$(TARGET_TEST)

valgrind: Main test
	valgrind --leak-check=full $(TARGET_MAIN)
	valgrind --leak-check=full $(TARGET_TEST)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: src/main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/tests.o: tests.cpp $(DOCTEST) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

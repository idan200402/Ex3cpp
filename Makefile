# idan.shumski@gmail.com

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Iinclude
SRC = src
INCLUDE = include
OBJS = main.o Game.o Player.o Governor.o
TARGET = CoupGame

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: $(SRC)/main.cpp $(INCLUDE)/Game.hpp $(INCLUDE)/Player.hpp $(INCLUDE)/Governor.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/main.cpp

Game.o: $(SRC)/Game.cpp $(INCLUDE)/Game.hpp $(INCLUDE)/Player.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/Game.cpp

Player.o: $(SRC)/Player.cpp $(INCLUDE)/Player.hpp $(INCLUDE)/Game.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/Player.cpp

Governor.o: $(SRC)/Governor.cpp $(INCLUDE)/Governor.hpp $(INCLUDE)/Player.hpp $(INCLUDE)/Game.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/Governor.cpp

clean:
	rm -f *.o $(TARGET)

run: all
	./$(TARGET)

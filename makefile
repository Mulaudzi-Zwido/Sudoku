CXX = g++
CXXFLAGS = -std=c++17 -g -Wall
LIBRARIES = -lsfml-window -lsfml-graphics -lsfml-system

TARGET = Sudoku
SOURCES = main.cpp Button.cpp Sudoku.cpp ScoreBoard.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBRARIES)
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBRARIES)

run:
	./$(TARGET)

clean:
	rm -rf *.o $(TARGET)

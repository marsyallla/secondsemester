CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
OBJ = main.o container.o
TARGET = Container_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

main.o: main.cpp Container.h
	$(CXX) $(CXXFLAGS) -c main.cpp

container.o: container.cpp Container.h
	$(CXX) $(CXXFLAGS) -c container.cpp

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
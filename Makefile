LD = g++
LFLAGS = -Wall

INCLUDES = main.cpp Graph.cpp Edge.cpp Vertex.cpp

EXECUTABLE = graph

all:
	$(LD) $(LFLAGS) -g -o $(EXECUTABLE) $(INCLUDES)

clean:
	rm -rf *.o $(EXECUTABLE)
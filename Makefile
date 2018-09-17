CC = g++
CFLAGS = -c -Wall
LDFLAGS =
SOURCES = $(wildcard *.cpp) $(wildcard *.cc)
OBJECTS = $(SOURCES:.cpp=.o)
PROTOBUF = `pkg-config --cflags --libs protobuf`
LIBS = $(PROTOBUF)
EXECUTABLE = hssim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(EXECUTABLE)

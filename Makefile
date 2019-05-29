TARGET = obj2off
LIBS = -Wl,--subsystem,windows
CFLAGS = -I.

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = main.o Mesh.o util.o Vertex.o
HEADERS =

%.o:	%.cpp	$(HEADERS)
	g++ -c $< -o $@ $(CFLAGS)


$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

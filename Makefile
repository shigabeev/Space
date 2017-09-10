CC = g++
OBJECTS = main.cpp tgaimage.cpp tgaimage.hpp
TARGET = main

$(TARGET):
	$(CC) $(OBJECTS)

clean:
	-rm -f *.tga
	-rm -f *.o
	-rm -f *.gch
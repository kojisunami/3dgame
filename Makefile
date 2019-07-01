
LFLAG=-lglut -lGLU -lGL -lc -lm -lpthread -lasound -ljpeg

all : main.o bmp.o font.o audio.o jpg.o xml.o
	g++ -o  run bmp.o font.o audio.o main.o jpg.o xml.o $(LFLAG)



font.o: font.c
	g++ -c font.c

bmp.o: bmp.c
	g++ -c bmp.c

main.o: main.cpp
	g++ -c main.cpp

audio.o: audio.c
	g++ -c audio.c

jpg.o: jpg.cpp
	g++ -c jpg.cpp

xml.o: xml.cpp
	g++ -c xml.cpp

debug : 
	./run

clean:
	rm *.o
	

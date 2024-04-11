commondir = ../common/
files = main
CC = g++
OBJCC = clang

all : $(files)

$(files) : $(files).cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	$(OBJCC) -Wall -c $(commondir)Mac/MicroGlut.m -o MicroGlut.o -framework OpenGL -framework Cocoa
	$(CC) -std=c++17 -Wall -o $(files).out -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES $(files).cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c MicroGlut.o -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

clean :
	rm *.out MicroGlut.o
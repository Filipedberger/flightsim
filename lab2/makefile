# set this variable to the director in which you saved the common files
commondir = ../common/
files = lab2-3

all : $(files)

$(files) : $(files).cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o $(files).out -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES $(files).cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++

clean :
	rm *.out

# set this variable to the director in which you saved the common files
commondir = common/
files = main

# automatically compile all .cpp files
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

all : $(files)

$(files) : $(OBJ_FILES) $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o $(files).out -I$(commondir) -I$(commondir)/Linux -DGL_GLEXT_PROTOTYPES $^ -lXt -lX11 -lGL -lm -lstdc++

%.o: %.cpp
	gcc -c -o $@ $< -I$(commondir) -I$(commondir)/Linux -DGL_GLEXT_PROTOTYPES

clean :
	rm *.out *.o
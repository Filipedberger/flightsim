commondir = common/
file = main
CPP_FILES := $(wildcard *.cpp)
CCFLAGS= -Wall -Wextra -Wpedantic -Weffc++ -Wno-deprecated-declarations

OS := $(shell uname)
ifeq ($(OS),Darwin)
    # Commands for macOS
    os = Mac/
	ext = m
	flags = -framework OpenGL -framework Cocoa -lm
else ifeq ($(OS),Linux)
    # Commands for Linux
    os = Linux/
	ext = c
	flags = -lXt -lX11 -lGL -lm -lstdc++
else
    $(error Unsupported operating system: $(OS))
endif


all : $(file)

$(file) : $(CPP_FILES) GL_utilities.o LoadTGA.o MicroGlut.o
	g++ -std=c++17 $(CCFLAGS) -o $(file).out -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES $(CPP_FILES) GL_utilities.o LoadTGA.o MicroGlut.o $(flags)

MicroGlut.o : $(commondir)$(os)MicroGlut.$(ext)
	gcc -c -Wno-deprecated-declarations $(commondir)$(os)MicroGlut.$(ext) -o MicroGlut.o

GL_utilities.o : $(commondir)GL_utilities.c
	gcc -c -Wno-deprecated-declarations $(commondir)GL_utilities.c -o GL_utilities.o

LoadTGA.o : $(commondir)LoadTGA.c
	gcc -c -Wno-deprecated-declarations $(commondir)LoadTGA.c -o LoadTGA.o


clean :
	rm *.out *.o

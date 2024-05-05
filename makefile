commondir = common/
file = main
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addsuffix .o, $(basename $(CPP_FILES)))
CCFLAGS= -Wall -Wextra -Wpedantic -Wno-deprecated-declarations -Wno-unused-parameter -fopenmp

OS := $(shell uname)
ifeq ($(OS),Darwin)
    # Commands for macOS
    os = Mac/
    ext = m
    flags = -framework OpenGL -framework Cocoa -lm -ljsoncpp -L/opt/homebrew/Cellar/jsoncpp/1.9.5/lib -I/opt/homebrew/Cellar/jsoncpp/1.9.5/include
    oflag =  -I/opt/homebrew/Cellar/jsoncpp/1.9.5/include
    compiler = /opt/homebrew/opt/llvm/bin/clang++
else ifeq ($(OS),Linux)
    # Commands for Linux
    os = Linux/
    ext = c
    flags = -lXt -lX11 -lGL -lm -lstdc++ -ljsoncpp
    oflag = 
    compiler = g++
else
    $(error Unsupported operating system: $(OS))
endif

all : $(file)

$(file) : $(OBJ_FILES) GL_utilities.o LoadTGA.o MicroGlut.o
	$(compiler) -std=c++17 $(CCFLAGS) -o $(file).out -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES GL_utilities.o LoadTGA.o MicroGlut.o $(OBJ_FILES) $(flags)

main.o : main.cpp
    $(compiler) -std=c++17 $(CCFLAGS) -c -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES $< -o $@ $(oflag)
    
%.o : %.cpp
	$(compiler) -std=c++17 $(CCFLAGS) -c -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES $< -o $@ $(oflag)

MicroGlut.o : $(commondir)$(os)MicroGlut.$(ext) $(commondir)$(os)MicroGlut.h
	gcc -c -Wno-deprecated-declarations  -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES  $(commondir)$(os)MicroGlut.$(ext) -o MicroGlut.o

GL_utilities.o : $(commondir)GL_utilities.c $(commondir)GL_utilities.h
	gcc -c -Wno-deprecated-declarations  -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES  $(commondir)GL_utilities.c -o GL_utilities.o

LoadTGA.o : $(commondir)LoadTGA.c $(commondir)LoadTGA.h
	gcc -c -Wno-deprecated-declarations  -I$(commondir) -I$(commondir)$(os) -DGL_GLEXT_PROTOTYPES $(commondir)LoadTGA.c -o LoadTGA.o

clean :
	rm *.out *.o
#ifndef STATE_H
#define STATE_H

#include "MicroGlut.h"
#include "GL_utilities.h"



class State {
    public:
    virtual void keyboard(unsigned char key, int x, int y) = 0;
    virtual void keyboard_up(unsigned char key, int x, int y) = 0;
    virtual void mouse(int x, int y) = 0;
    virtual void update() = 0;
    virtual void display() = 0;

    virtual ~State() = default;

    protected:
    GLuint program;

};

#endif
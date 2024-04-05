#ifndef STATE_H
#define STATE_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"


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
    mat4 world2view;
    mat4 projection;
    vec3 cameraPosition;
    vec3 lookAtPoint;
    vec3 upVector;

    virtual void upload2shader() {
        glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, world2view.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, projection.m);
    }
};

#endif
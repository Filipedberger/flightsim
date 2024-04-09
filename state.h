#ifndef STATE_H
#define STATE_H

#include "frustum.h"
#include "object.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include <vector>


class State {
    public:
    State();
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void keyboard_up(unsigned char key, int x, int y) = 0;
    virtual void mouse(int x, int y) = 0;
    virtual void update(int time_elapsed) = 0;
    virtual void display() = 0;

    virtual ~State();

    protected:
    GLuint program;
    mat4 world2view;
    mat4 projection;
    vec3 cameraPosition;
    vec3 lookAtPoint;
    vec3 upVector;
    Frustum frustum_obj;

    // Objects:
    std::vector<Object*> objects;
    Object* ground;
    Object* skybox;


    virtual void upload2shader();
    void create_world2view(vec3 cameraPosition = vec3(0.0f, 0.0f, 50.0f), vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f), vec3 upVector = vec3(0.0f, 1.0f, 0.0f));
    void create_projection(float near = 1.0, float far = 200.0, float right = 0.5, float left = -0.5, float top = 0.5, float bottom = -0.5);
};

#endif
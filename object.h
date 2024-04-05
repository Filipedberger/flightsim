#ifndef OBJECT_H
#define OBJECT_H

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include <string>

class Object {
    public:
    Object();
    virtual void update();
    virtual void display(GLuint program);
    virtual void create_model(std::string filename, int x = 0, int y = 0, int z = 0, float sc = 1);

    virtual ~Object() = default;

    protected:
    Model* model;
    mat4 model2world;
    vec3 position;
    float scale;

    virtual void update_model2world() {
        model2world = T(position.x, position.y, position.z) * S(scale);
    }

    virtual void upload2shader(GLuint program) {
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, model2world.m);
    }

    virtual void translate(vec3 translation) {
        position = position + translation;
        update_model2world();
    }

    virtual void move(vec3 coordinates) {
        position = coordinates;
        update_model2world();
    }





};


#endif
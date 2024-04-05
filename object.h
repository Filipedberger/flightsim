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
    virtual void create_model(std::string filename, vec3 pos = vec3(0,0,0), float sc = 1);
    virtual void translate(vec3 translation);
    virtual void move(vec3 coordinates);
    virtual void rotate(float angle, vec3 axis);
    virtual ~Object() = default;

    protected:
    Model* model;
    mat4 model2world;
    vec3 position;
    float scale;

    void update_model2world();
    void upload2shader(GLuint program);

};


#endif
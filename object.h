#ifndef OBJECT_H
#define OBJECT_H

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include <string>

class Object {
    public:
    Object() = default;
    Object(std::string filename, vec3 pos = vec3(0,0,0), float sc = 1);
    virtual void update(int time_elapsed);
    virtual void display(GLuint program);
    virtual void create_model(std::string filename, vec3 pos, float sc);

    virtual void translate(vec3 translation);
    virtual void move(vec3 coordinates);
    virtual void rotate(float angle, vec3 axis);
    virtual void scale(float sc);
    virtual ~Object() = default;

    protected:
    Model* model;
    mat4 translationMatrix;
    mat4 rotationMatrix;
    mat4 scaleMatrix;
    vec3 position;
    float scale_factor;

    void update_model2world();
    void upload2shader(GLuint program);

};


#endif
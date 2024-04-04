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
    virtual void create_model(std::string filename, int x = 0, int y = 0, int z = 0, int scale = 1);

    virtual ~Object() = default;

    protected:
    Model* model;
    mat4 mdl_matrix;
    int x;
    int y;
    int z;

    

};


#endif
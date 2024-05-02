#ifndef OBJECT_H
#define OBJECT_H

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include <string>
#include <jsoncpp/json/json.h>

class Object {
    public:
    
    // Constructors:
    Object() = default;
    Object(const std::string& filename, vec3 pos = vec3(0,0,0), float sc = 1);

    // Standard functions:
    virtual void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed);
    virtual void display(const GLuint& program, const mat4& world2view, const mat4& projection);
    virtual void create_model(const std::string& filename, vec3 pos, float sc);

    // Transformation functions:
    virtual void translate(vec3 translation);
    virtual void move(vec3 coordinates);
    virtual void rotate(float angle, vec3 axis);
    virtual void scale(float sc);

    virtual void reset();

    virtual ~Object() = default;

    float radius;
    vec3 position = vec3(0,0,0);
    vec3 center;

    Object& operator=(const Object& other) = delete;
    Object(const Object& other) = delete;

    // Getters:
    virtual vec3 get_pos() {return position;}
    virtual vec3 get_lookAtPoint() {return position;}
    virtual mat4 get_lookAtMatrix() {return IdentityMatrix();}
    virtual vec3 get_upVector() {return vec3(0,1,0);}
    virtual float get_planeSpeed() {return 0;}

    virtual std::map<std::pair<int, int>, int> get_points_on_radius() {return std::map<std::pair<int, int>, int>();}


    protected:
    GLuint object_program;
    Model* model;
    mat4 translationMatrix;
    mat4 rotationMatrix = IdentityMatrix();
    mat4 scaleMatrix = IdentityMatrix();

    Json::Value settings;
    
    float scale_factor;

    virtual void upload2shader(const GLuint& program);

};


#endif
#ifndef STATE_H
#define STATE_H

#include "frustum.h"
#include "object.h"
#include "context.h"
#include "ground.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include <vector>
#include <jsoncpp/json/json.h>



class State {
    public:
    State(Json::Value settings, Context* c);

    virtual void keyboard(unsigned char key, int x, int y);
    virtual void keyboard_up(unsigned char key, int x, int y);
    virtual void mouse(int x, int y) = 0;
    virtual void update(int time_elapsed) = 0;
    virtual void display() = 0;

    virtual ~State();

    State& operator=(const State& other) = delete;
    State(const State& other) = delete;

    protected:
    GLuint program;

    // Camera:
    mat4 world2view;
    mat4 projection;
    vec3 cameraPosition;
    vec3 lookAtPoint;
    vec3 upVector;
    Frustum frustum_obj;

    // Objects:
    std::vector<Object*> objects;
    Object* ground = nullptr;
    Object* skydome = nullptr;
    Object* hud = nullptr;

    Context* context;
    Json::Value settings;

    // Keys:
    std::map<char, bool> keys_pressed;
    std::map<char, bool> keys_toggle;


    virtual void upload2shader();
    void create_world2view(vec3 cameraPosition, vec3 lookAtPoint, vec3 upVector);
    void create_projection(float near, float far, float right, float left, float top, float bottom);
    void create_projection_from_json(Json::Value s);
};  

#endif
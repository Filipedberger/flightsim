#include "state.h"
#include "frustum.h"
#include "context.h"
#include "helper.h"
#include <iostream>


State::State(Json::Value s, Context* c)  {

    cameraPosition = json2vec(s["camera_pos"]);
    lookAtPoint = json2vec(s["look_at"]);
    upVector = json2vec(s["up"]);

    create_world2view(cameraPosition, lookAtPoint, upVector);
    create_projection_from_json(s);

    context = c;
    settings = s;
}


void State::upload2shader() {
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, world2view.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, projection.m);
}

void State::create_world2view(vec3 cameraPosition, vec3 lookAtPoint, vec3 upVector) {
    world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
}

void State::create_projection(float near, float far, float right, float left, float top, float bottom) {
    projection = frustum(left, right, bottom, top, near, far);
    frustum_obj =  Frustum(near, far, right, left, top, bottom); 
}

void State::create_projection_from_json(Json::Value s) {
    create_projection(s["near"].asFloat(), s["far"].asFloat(), s["right"].asFloat(), s["left"].asFloat(), s["top"].asFloat(), s["bottom"].asFloat());
}

void State::keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
            break;
    }
}

State::~State() {
    for (Object* object : objects) {
        delete object;
    }
    delete ground;
    delete skybox;
}
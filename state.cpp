#include "state.h"
#include "frustum.h"


State::State() {
    create_world2view();
    create_projection();
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

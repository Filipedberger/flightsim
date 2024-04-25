#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include "frustum.h"
#include <jsoncpp/json/json.h>

class Plane : public Object {
    public:
    Plane();
    Plane(Json::Value settings, vec3 pos = vec3(0,0,0));

    void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) override;

    void reset() override;

    ~Plane() override;

    // Getters:
    vec3 get_pos() override;
    vec3 get_lookAtPoint() override;
    mat4 get_lookAtMatrix() override;
    vec3 get_upVector() override;

    private:
    vec3 direction;
    float speed; // Pixels per millisecond
    mat4 standard_rotation = IdentityMatrix();
    mat4 standard_inverse = IdentityMatrix();
    vec3 model_up;
    vec3 model_forward;
    vec3 model_right;

    vec3 world_up = vec3(0, 1, 0);
    vec3 world_forward = vec3(0, 0, -1);
    vec3 world_right = vec3(1, 0, 0);

    float offset;
    int angle{0};

    mat4 tilt_matrix = IdentityMatrix();
    mat4 turn_matrix = IdentityMatrix();
    mat4 pitch_matrix = IdentityMatrix();
    float tilt_angle = 0;
    float turn_angle = 0;
    float pitch_angle = 0;

    float roll = 0;
    float pitch = 0;

    void calculate_radius();

    void tilt(std::map<char, bool> keys_pressed);

    mat4 create_rotation_matrix(vec3 forward, vec3 up, vec3 right);

    vec3 tmp_right;
    vec3 tmp_up;
    vec3 tmp_forward;

    mat4 tmp_mat = IdentityMatrix();

};

#endif
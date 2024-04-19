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

    private:
    vec3 direction;
    float speed; // Pixels per millisecond
    mat4 standard_rotation = IdentityMatrix();
    mat4 standard_inverse = IdentityMatrix();
    vec3 model_up;
    vec3 model_forward;
    vec3 model_right;
    vec3 world_up;
    vec3 world_forward;
    vec3 world_right;
    vec3 forward_direction;
    vec3 up_direction;
    vec3 offset;
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
};



#endif
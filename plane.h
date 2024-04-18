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

    private:
    vec3 direction;
    float speed; // Pixels per millisecond
    mat4 standard_rotation = IdentityMatrix();
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

    void calculate_radius();

    void tilt(std::map<char, bool> keys_pressed);
};



#endif
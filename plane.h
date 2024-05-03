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
    std::map<std::pair<int, int>, int> get_points_on_radius() override;

    private:
    float speed; // Pixels per millisecond
    vec3 model_up;
    vec3 model_forward;
    vec3 model_right;

    float offset;

    float roll = 0;
    int roll_speed = 2;
    float pitch = 0;
    int pitch_speed = 1;
    float acceleration = 0;

    float camera_y_offset = 0;
    
    void calculate_radius();

    void tilt(std::map<char, bool> keys_pressed);

    mat4 create_rotation_matrix(vec3 forward, vec3 up, vec3 right);

};

#endif
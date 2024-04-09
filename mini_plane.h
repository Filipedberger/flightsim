#ifndef MINI_PLANE_H
#define MINI_PLANE_H

#include "object.h"
#include "frustum.h"
#include <jsoncpp/json/json.h>

class Mini_Plane : public Object {
    public:
    Mini_Plane();
    Mini_Plane(const std::string& filename, const Frustum& frustum_obj, vec3 pos = vec3(0,0,0), float sc = 1);
    Mini_Plane(const std::string& filename, const Frustum& frustum_obj,  Json::Value rotation, vec3 pos = vec3(0,0,0), float sc = 1);
    void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) override;

    void reset() override;

    ~Mini_Plane() override;

    vec3 center;

    private:
    vec3 direction;
    float speed = 0.020f; // Pixels per millisecond

    void random_direction();
    void calculate_radius();
    void random_pos_direction();
};



#endif
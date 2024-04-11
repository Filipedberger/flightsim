#ifndef MINI_PLANE_H
#define MINI_PLANE_H

#include "object.h"
#include "frustum.h"
#include <jsoncpp/json/json.h>

class Mini_Plane : public Object {
    public:
    Mini_Plane();
    Mini_Plane(const std::string& filename, const Frustum& f, vec3 pos = vec3(0,0,0), float sc = 1);
    Mini_Plane(const std::string& filename, const Frustum& f,  Json::Value settings, vec3 pos = vec3(0,0,0), float sc = 1);
    Mini_Plane(Model* m, const Frustum& f, Json::Value settings);

    void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) override;

    void reset() override;

    ~Mini_Plane() override;

    vec3 center;

    private:
    vec3 direction;
    float speed; // Pixels per millisecond
    mat4 standard_rotation = IdentityMatrix();
    vec3 direction_axis;
    Frustum frustum_obj;

    void random_direction(int min = 0, int max = 360);
    void calculate_radius();
    void random_pos_direction();
};



#endif
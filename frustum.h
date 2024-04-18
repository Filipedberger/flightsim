#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "VectorUtils4.h"

class Frustum {
    public:
    Frustum()
    : near(1.0), far(20.0), right(0.5), left(-0.5), top(0.5), bottom(-0.5),
    left_normal(vec3(0.0, 0.0, 0.0)), left_p(vec3(0.0, 0.0, 0.0)),
    right_normal(vec3(0.0, 0.0, 0.0)), right_p(vec3(0.0, 0.0, 0.0)),
    top_normal(vec3(0.0, 0.0, 0.0)), top_p(vec3(0.0, 0.0, 0.0)),
    bottom_normal(vec3(0.0, 0.0, 0.0)), bottom_p(vec3(0.0, 0.0, 0.0)),
    near_normal(vec3(0.0, 0.0, 0.0)), near_p(vec3(0.0, 0.0, 0.0)),
    far_normal(vec3(0.0, 0.0, 0.0)), far_p(vec3(0.0, 0.0, 0.0)),
    left_near_bottom(vec3(0.0, 0.0, 0.0)), left_near_top(vec3(0.0, 0.0, 0.0)),
    right_near_bottom(vec3(0.0, 0.0, 0.0)), right_near_top(vec3(0.0, 0.0, 0.0)),
    left_far_bottom(vec3(0.0, 0.0, 0.0)), left_far_top(vec3(0.0, 0.0, 0.0)),
    right_far_bottom(vec3(0.0, 0.0, 0.0)), right_far_top(vec3(0.0, 0.0, 0.0)) {}
    
    Frustum(float near, float far, float right, float left, float top, float bottom);
    ~Frustum() = default;

    float near;
    float far;
    float right;
    float left;
    float top;
    float bottom;

    vec3 left_normal;
    vec3 left_p;
    vec3 right_normal;
    vec3 right_p;
    vec3 top_normal;
    vec3 top_p;
    vec3 bottom_normal;
    vec3 bottom_p;
    vec3 near_normal;
    vec3 near_p;
    vec3 far_normal;
    vec3 far_p;

    vec3 left_near_bottom;
    vec3 left_near_top;
    vec3 right_near_bottom;
    vec3 right_near_top;
    vec3 left_far_bottom; 
    vec3 left_far_top;
    vec3 right_far_bottom;
    vec3 right_far_top;

    bool side_culling(const vec3& p, float r, const mat4& world2view);

    private:
    void create_normal(vec3& normal, vec3& p, const vec3& p1, const vec3& p2, const vec3& p3);
    float left_dist(const vec3& p);
    float right_dist(const vec3& p);
    float far_dist(const vec3& p);
    float near_dist(const vec3& p);
};

#endif
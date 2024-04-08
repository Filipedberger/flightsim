#include "frustum.h"

#include "VectorUtils4.h"
#include <iostream>


Frustum::Frustum(float near, float far, float right, float left, float top, float bottom) 
    : near(near), far(far), right(right), left(left), top(top), bottom(bottom) {
    left_near_bottom = vec3(left, bottom, -near);
    left_near_top = vec3(left, top, -near);
    right_near_bottom = vec3(right, bottom, -near);
    right_near_top = vec3(right, top, -near);


    float x = (far * near) / right;


    left_far_bottom = vec3(-x, bottom, -far); //THESE FAR POINTS ARE NOT CORRECT
    left_far_top = vec3(-x, top, -far);
    right_far_bottom = vec3(x, bottom, -far);
    right_far_top = vec3(x, top, -far);

    create_normal(left_normal, left_p, left_near_top, left_far_top, left_near_bottom);
    create_normal(right_normal, right_p, right_near_top, right_near_bottom, right_far_top);
    create_normal(top_normal, top_p, left_near_top, right_near_top, left_far_top);
    create_normal(bottom_normal, bottom_p, left_near_bottom, left_far_bottom, right_near_bottom);
    create_normal(near_normal, near_p, left_near_bottom, right_near_bottom, left_near_top);
    create_normal(far_normal, far_p, left_far_top, right_far_top, left_far_bottom);
}

void Frustum::create_normal(vec3& normal, vec3& p, const vec3& p1, const vec3& p2, const vec3& p3) {
    // We want the normal to point outwards from the frustum.
    // p1 should be the start point of the vector v1 and v2.
    vec3 v1 = p2 - p1;
    vec3 v2 = p3 - p1;
    normal = normalize(cross(v1, v2));
    p = p1;
}

float Frustum::left_dist(const vec3& p) {
    return dot(p - left_p, left_normal);
}

float Frustum::right_dist(const vec3& p) {
    return dot(p - right_p, right_normal);
    
}

float Frustum::far_dist(const vec3& p) {
    return dot(p - far_p, far_normal);
}

float Frustum::near_dist(const vec3& p) {
    return dot(p - near_p, near_normal);
}

bool Frustum::side_culling(const vec3& p, float r, const mat4& world2view) {
    vec3 p_view = world2view * vec4(p, 1.0);
    /*std::cout << "LEFT: " << left_dist(p_view) << std::endl;
    std::cout << "RIGHT: " << right_dist(p_view) << std::endl;*/
    if (left_dist(p_view) - r > 0) {
        return true;
    }
    if (right_dist(p_view) - r > 0) {
        return true;
    }
    if (far_dist(p_view) - r > 0) {
        return true;
    }
    if (near_dist(p_view) - r > 0) {
        std::cout << "NEAR"<< std::endl;
        return true;
    }

    return false;
}
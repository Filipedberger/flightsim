#include <iostream>
#include "VectorUtils4.h"

std::string vec2str(vec3 v) {
    return "vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

std::string vec2str(vec4 v) {
    return "vec4(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
}
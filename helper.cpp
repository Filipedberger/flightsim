#include <iostream>
#include "VectorUtils4.h"
#include <jsoncpp/json/json.h>

std::string vec2str(vec3 v) {
    return "vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

std::string vec2str(vec4 v) {
    return "vec4(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
}

vec3 json2vec(Json::Value json) {
    return vec3(json[0].asFloat(), json[1].asFloat(), json[2].asFloat());
}
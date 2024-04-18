// helper.h

#ifndef HELPER_H
#define HELPER_H

#include "VectorUtils4.h"
#include <string>
#include <jsoncpp/json/json.h>

std::string vec2str(vec3 v);

std::string vec2str(vec4 v);

vec3 json2vec(Json::Value json);

float rad(float deg);

#endif // HELPER_H
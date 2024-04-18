#ifndef SKYDOME_H
#define SKYDOME_H

#include "object.h"
#include "frustum.h"
#include <jsoncpp/json/json.h>

class Skydome : public Object {
    public:
        Skydome() = default;
        Skydome(const std::string& filename, const vec3& camera_pos, float sc);
        Skydome(Json::Value& c, const vec3& cameraPosition);

        void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) override;
        void display(const GLuint& program, const mat4& world2view, const mat4& projection) override;
        void reset() override;

        ~Skydome() override;

    private:
        GLuint skyBoxtex;
};
#endif
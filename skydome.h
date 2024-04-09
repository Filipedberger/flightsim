#ifndef MINI_PLANE_H
#define MINI_PLANE_H

#include "object.h"
#include "frustum.h"

class Skydome : public Object {
    public:
        Skydome() = default;
        Skydome(const std::string& filename, const vec3& camera_pos, float sc);
        void update(int time_elapsed, vec3 camera_pos) override;
        void display(const GLuint& program) override;

        void reset() override;

        ~Skydome() override;

    private:
        //float speed = 0.020f; // Pixels per millisecond
};
#endif
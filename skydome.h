#ifndef SKYDOME_H
#define SKYDOME_H

#include "object.h"
#include "frustum.h"

class Skydome : public Object {
    public:
        Skydome() = default;
        Skydome(const std::string& filename, const vec3& camera_pos, float sc);
        void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) override;
        void display(const GLuint& program) override;
        void reset() override;

        ~Skydome() override;

    private:
        void upload2shader(const GLuint& program);
        GLuint skyBoxtex;
};
#endif
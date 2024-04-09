#ifndef MINI_PLANE_H
#define MINI_PLANE_H

#include "object.h"
#include "frustum.h"

class Skydome : public Object {
    public:
        Skydome();
        Skydome(const std::string& filename, const vec3& camera_pos, float sc);
        void update(int time_elapsed) override;

        void reset() override;

        ~Skydome() override;

        //vec3 center;

    private:
        //vec3 direction;
        float speed = 0.020f; // Pixels per millisecond

        //void random_direction();
        //void calculate_radius();
        //void random_pos_direction();
};
#endif
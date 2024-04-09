#include "skydome.h"
#include "frustum.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <random>

Skydome::Skydome(const std::string& filename, const vec3& camera_pos, float sc)
    : Object(filename, camera_pos, sc) {
    rotate(-M_PI_2, vec3(1,0,0));
    //random_pos_direction();
    //calculate_radius();
    std::cout << "SKYDOME CREATED" << std::endl;
    return;
}

void Skydome::update(int camera_pos) {
    translate(camepra_pos);
    return;
}

void Skydome::reset() {
    rotationMatrix = IdentityMatrix();
    //rotate(-M_PI_2, vec3(1,0,0));
    return;
}

Skydome::~Skydome() {
    delete model;
}


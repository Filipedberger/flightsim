#include "mini_plane.h"
#include "frustum.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <random>
#include <jsoncpp/json/json.h>

Mini_Plane::Mini_Plane(const std::string& filename,  const Frustum& frustum_obj, vec3 pos, float sc)
    : Object(filename, pos, sc) {
    rotate(-M_PI_2, vec3(1,0,0));
    random_pos_direction();
    calculate_radius();
    std::cout << "MINI PLANE CREATED" << std::endl;
    return;
}

Mini_Plane::Mini_Plane(const std::string& filename, const Frustum& frustum_obj,  Json::Value settings, vec3 pos, float sc) 
    : Object(filename, pos, sc){
    

    float angle;
    vec3 axis;

    Json::Value rotation = settings["rotation"];

    for (int i = 0; i < rotation.size(); i++) {
        angle = rotation[i]["angle"].asFloat();
        angle = angle * M_PI / 180;
        axis = vec3(rotation[i]["axis"][0].asFloat(), rotation[i]["axis"][1].asFloat(), rotation[i]["axis"][2].asFloat());
        rotate(angle, axis);
        standard_rotation = standard_rotation * ArbRotate(axis, angle);
    }
    Json::Value dir_axis = settings["direction_axis"];
    direction_axis = vec3(dir_axis[0].asFloat(), dir_axis[1].asFloat(), dir_axis[2].asFloat());
    random_pos_direction();
    calculate_radius();
    std::cout << "MINI PLANE CREATED" << std::endl;
}


void Mini_Plane::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) {
    translate(direction * time_elapsed * speed);
    //move(vec3(0,30,0));
    return;
}

void Mini_Plane::random_direction() {
    float rand_angle = (rand() % 360) * M_PI / 180;
    rotate(rand_angle, direction_axis);
    direction = vec3(-sin(rand_angle),0, -cos(rand_angle));
    return;
}

void Mini_Plane::calculate_radius() {
    center = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < model->numVertices; i++) {
        center = center + model->vertexArray[i];
    }

    center = (center / model->numVertices) * scale_factor;
    
    for (int i = 0; i < model->numVertices; i++) {
        vec3 dist_vec = model->vertexArray[i] - center;
        float dist = dist_vec * dist_vec;
        radius = fmax(radius, dist);
    }

    radius = sqrt(radius) * scale_factor; //OBS SHOULD CHANGE THIS WHEN SCALING IS CHANGED
    
}

void Mini_Plane::random_pos_direction() {
    //HARD CODED FOR NOW
    random_direction();

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    vec3 temp_pos = vec3(0,0,0);
    std::uniform_int_distribution<> distrib(-50, 50);
    temp_pos.x = distrib(gen);
    std::uniform_int_distribution<> distrib1(20, 50);
    temp_pos.y = distrib1(gen);
    std::uniform_int_distribution<> distrib2(-200, 10);
    temp_pos.z = distrib2(gen);

    
    move(temp_pos);

}

void Mini_Plane::reset() {
    rotationMatrix = standard_rotation;
    random_pos_direction();
    return;
}

Mini_Plane::~Mini_Plane() {
    delete model;
}


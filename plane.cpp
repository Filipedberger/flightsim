#include "plane.h"
#include "frustum.h"
#include "helper.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <string>
#include <random>
#include <jsoncpp/json/json.h>


Plane::Plane(Json::Value settings, vec3 pos) {

    std::string filename = settings["path"].asString();
    offset = vec3(settings["offset"][0].asFloat(), settings["offset"][1].asFloat(), settings["offset"][2].asFloat());
    float sc = settings["scale"].asFloat();
    position = pos + offset;

    create_model(filename, position, sc);

    std::cout << "File: " << filename << std::endl;
    

    float angle;
    vec3 axis;

    Json::Value rotation = settings["rotation"];

    for (int i = 0; i < static_cast<int>(rotation.size()); i++) {
        angle = rotation[i]["angle"].asFloat();
        angle = angle * M_PI / 180;
        axis = vec3(rotation[i]["axis"][0].asFloat(), rotation[i]["axis"][1].asFloat(), rotation[i]["axis"][2].asFloat());
        rotate(angle, axis);
        standard_rotation = standard_rotation * ArbRotate(axis, angle);
    }
    standard_inverse = transpose(standard_rotation);
    Json::Value dir_axis = settings["direction_axis"];
    calculate_radius();

    speed = settings["speed"].asFloat();
    std::cout << "MINI PLANE CREATED" << std::endl;

    // Create model vetors
    model_right = vec3(rotationMatrix.m[0], rotationMatrix.m[4], rotationMatrix.m[8]);
    model_forward = vec3(rotationMatrix.m[1], rotationMatrix.m[5], rotationMatrix.m[9]);
    model_up = vec3(rotationMatrix.m[2], rotationMatrix.m[6], rotationMatrix.m[10]);
}


void Plane::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) {
    position += model_forward * speed * time_elapsed;
    move(position);
    tilt(keys_pressed);
    return;
}

void Plane::tilt(std::map<char, bool> keys_pressed) {
    if (keys_pressed['r']) {
        reset();
        return;}

    if (!keys_pressed['a'] && !keys_pressed['d'] && !keys_pressed['w'] && !keys_pressed['s']) {
        return;
    }
    
    pitch = 0;
    roll = 0;

    if (keys_pressed['a'] || keys_pressed['d']) {
        
        if (keys_pressed['d']) {
            roll = 2;
        }
        
        if (keys_pressed['a']) {
            roll = -2;
        }
        tilt_matrix = ArbRotate(model_forward, rad(roll));
        model_right = tilt_matrix * model_right;
        model_up = tilt_matrix * model_up;
    }
    if (keys_pressed['w'] || keys_pressed['s']) {
        if (keys_pressed['s']) {
            pitch = 1;
        }

        else if (keys_pressed['w']) {
            pitch = -1;
        }
        pitch_matrix = ArbRotate(model_right, rad(pitch));
        model_forward = pitch_matrix * model_forward;
        model_up = pitch_matrix * model_up;
    }
    
    rotationMatrix =  create_rotation_matrix(model_forward, model_up, model_right);
}

mat4 Plane::create_rotation_matrix(vec3 forward, vec3 up, vec3 right) {
    return mat4(right.x, forward.x, up.x, 0,
                right.y, forward.y, up.y, 0,
                right.z, forward.z, up.z, 0,
                0, 0, 0, 1);
}

void Plane::calculate_radius() {
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

vec3 Plane::get_pos() {
    return position - model_forward * 10 + model_up * 5;
}


vec3 Plane::get_lookAtPoint() {
    return position;
}

vec3 Plane::get_upVector() {
    return model_up;
}

mat4 Plane::get_lookAtMatrix() {
    vec3 cameraPosition = position - model_forward * 10 + model_up * 5;
    return lookAtv(cameraPosition, position, model_up);
}

void Plane::reset() {
    rotationMatrix = standard_rotation;
    move(vec3(0,30,20));
    
    return;
}

Plane::~Plane() {
    //delete model;
}


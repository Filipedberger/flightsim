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
    Json::Value dir_axis = settings["direction_axis"];
    //direction_axis = vec3(dir_axis[0].asFloat(), dir_axis[1].asFloat(), dir_axis[2].asFloat());
    //forward_direction = standard_rotation * vec4(0,1,0,0);
    model_forward = vec3(0,1,0);
    std::cout << "FORWARD DIRECTION: " << vec2str(forward_direction) << std::endl;
    //up_direction = standard_rotation * vec4(0,0,1,0);
    model_up = vec3(0,0,1);
    model_right = vec3(1,0,0);
    std::cout << "UP DIRECTION: " << vec2str(up_direction) << std::endl;
    calculate_radius();

    speed = settings["speed"].asFloat();
    std::cout << "MINI PLANE CREATED" << std::endl;
}


void Plane::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) {
    //position = cameraPosition + offset;
    /*vec3 movement = world_forward * speed * time_elapsed;
    if (keys_pressed['w']) {
        position += movement;
    }
    else if (keys_pressed['s']) {
        position -= movement;
    }*/
    vec3 forward = vec3(sin(rad(turn_angle))*sin(rad(pitch_angle)),
                        cos(rad(turn_angle))*sin(rad(pitch_angle)),
                        cos(rad(pitch_angle)));

    std::cout << "FORWARD: " << vec2str(forward) << std::endl;
    position += (world_forward + world_up) * speed * time_elapsed;
    position += forward * speed * time_elapsed;
    move(position);
    tilt(keys_pressed);
    return;
}

void Plane::tilt(std::map<char, bool> keys_pressed) {
    if (keys_pressed['r']) {
        reset();
        return;}

    if (keys_pressed['a'] || keys_pressed['d']) {
        
        if (keys_pressed['d']) {
            
            if (tilt_angle != 60) {
                //tilt_angle += 1;
                tilt_angle = 0;
            }

            turn_angle -= 1;
        }
        
        if (keys_pressed['a']) {
            if (tilt_angle != -60) {
                //tilt_angle -= 1;
                tilt_angle = 0;
            }

            turn_angle += 1;  
        }
    }

    // Return to standard tilt
    /*else if (tilt_angle != 0) {
        if (tilt_angle > 0) {
            tilt_angle -= 1;
            //rotate(rad(-1), model_forward); //TILT
        }
        else if (tilt_angle < 0) {
            tilt_angle += 1;
            //rotate(rad(1), model_forward); //TILT
        }
    }*/
    
    
    if (keys_pressed['w']) {
        if (pitch_angle < 89) {
            pitch_angle += 1;
        }
    }
    else if (keys_pressed['s']) {
        if (pitch_angle > -89) {
            pitch_angle -= 1;
        }
    }

    //vec3 right = vec3(cos(rad(turn_angle)*sin))

    pitch_matrix = ArbRotate(model_right, rad(pitch_angle));
    vec3 up = pitch_matrix * model_up;

    turn_matrix = ArbRotate(up, rad(turn_angle));
    vec3 forward = turn_matrix * pitch_matrix * model_forward;
    
    tilt_matrix = ArbRotate(forward, rad(tilt_angle));

    rotationMatrix = standard_rotation * tilt_matrix * turn_matrix * pitch_matrix;
    world_forward = standard_rotation * forward;
    world_up = standard_rotation * ArbRotate(model_right, rad(pitch_angle - 90)) * model_up;
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
    return position + ArbRotate(vec3(0,1,0), rad(turn_angle)) * offset;
}

vec3 Plane::get_lookAtPoint() {
    return position;
}

void Plane::reset() {
    rotationMatrix = standard_rotation;
    move(vec3(0,30,20));
    
    return;
}

Plane::~Plane() {
    //delete model;
}


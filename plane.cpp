#include "plane.h"
#include "frustum.h"
#include "helper.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <string>
#include <random>
#include <jsoncpp/json/json.h>

Plane::Plane(Json::Value settings, vec3 pos)
{

    std::string filename = settings["path"].asString();
    offset = settings["offset"].asFloat();
    float sc = settings["scale"].asFloat();
    position = pos;

    pitch_speed = settings["pitch_speed"].asInt();
    roll_speed = settings["roll_speed"].asInt();
    acceleration = settings["acceleration"].asFloat();

    create_model(filename, position, sc);

    std::cout << "File: " << filename << std::endl;

    float angle;
    vec3 axis;

    Json::Value rotation = settings["rotation"];

    for (int i = 0; i < static_cast<int>(rotation.size()); i++)
    {
        angle = rotation[i]["angle"].asFloat();
        angle = angle * M_PI / 180;
        axis = vec3(rotation[i]["axis"][0].asFloat(), rotation[i]["axis"][1].asFloat(), rotation[i]["axis"][2].asFloat());
        rotate(angle, axis);
    }
    calculate_radius();

    speed = settings["speed"].asFloat();

    // Create model vetors
    model_right = vec3(settings["right"][0].asFloat(), settings["right"][1].asFloat(), settings["right"][2].asFloat());
    model_up = vec3(settings["up"][0].asFloat(), settings["up"][1].asFloat(), settings["up"][2].asFloat());
    model_forward = vec3(settings["forward"][0].asFloat(), settings["forward"][1].asFloat(), settings["forward"][2].asFloat());
}

void Plane::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed)
{
    position += rotationMatrix * model_forward * speed * time_elapsed;
    move(position);
    tilt(keys_pressed);
    return;
}

void Plane::tilt(std::map<char, bool> keys_pressed)
{
    pitch = 0;
    roll = 0;

    if (keys_pressed['a'] || keys_pressed['d'])
    {

        if (keys_pressed['d'])
        {
            roll = roll_speed;
        }

        if (keys_pressed['a'])
        {
            roll = -roll_speed;
        }
        rotationMatrix = rotationMatrix * ArbRotate(model_forward, rad(roll));
    }
    if (keys_pressed['w'] || keys_pressed['s'])
    {
        if (keys_pressed['s'])
        {
            pitch = pitch_speed;
        }

        else if (keys_pressed['w'])
        {
            pitch = -pitch_speed;
        }
        rotationMatrix = rotationMatrix * ArbRotate(model_right, rad(pitch));
    }

    if (keys_pressed['\1']) {
        speed += acceleration;
    }
    if (keys_pressed['\2']) {
        speed -= acceleration;
    }

}

void Plane::calculate_radius()
{
    center = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < model->numVertices; i++)
    {
        center = center + model->vertexArray[i];
    }

    center = (center / model->numVertices) * scale_factor;
    for (int i = 0; i < model->numVertices; i++)
    {
        vec3 dist_vec = model->vertexArray[i] - center;
        float dist = dist_vec * dist_vec;
        radius = fmax(radius, dist);
    }

    radius = sqrt(radius) * scale_factor; // OBS SHOULD CHANGE THIS WHEN SCALING IS CHANGED, NOT IMPLEMENTED YET
}

vec3 Plane::get_pos()
{
    return position - rotationMatrix * model_forward * offset;
}

vec3 Plane::get_lookAtPoint()
{
    return position;
}

vec3 Plane::get_upVector()
{
    return model_up;
}

mat4 Plane::get_lookAtMatrix()
{
    return lookAtv(get_pos(), position, rotationMatrix * model_up);
}

void Plane::reset()
{
    rotationMatrix = IdentityMatrix();
    move(vec3(0, 30, 20));

    return;
}

Plane::~Plane()
{
    // Model deleted in object destructor
    // delete model;
}

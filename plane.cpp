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

    // Set up light
    light_offset = vec3(settings["light_offset"][0].asFloat(), settings["light_offset"][1].asFloat(), settings["light_offset"][2].asFloat());
    light_color = vec3(settings["light_color"][0].asFloat(), settings["light_color"][1].asFloat(), settings["light_color"][2].asFloat());
    light_radius = settings["light_radius"].asInt();
}

void Plane::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed)
{
    position += rotationMatrix * model_forward * speed * time_elapsed;
    move(position);
    tilt(keys_pressed);
    update_light(time_elapsed);
    return;
}

void Plane::display(const GLuint& program, const mat4& world2view, const mat4& projection)
{
    glUniform3fv(glGetUniformLocation(program, "plane_light_pos"), 2, &light_pos[0].x);

    vec3 temp_light = light_color * light_intensity;
    glUniform3fv(glGetUniformLocation(program, "plane_light_intensity"), 1, &temp_light.x);

    glUniform1i(glGetUniformLocation(program, "plane_light_radius"), light_radius);

    Object::display(program, world2view, projection);

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
    return position - rotationMatrix * model_forward * offset + rotationMatrix * model_up * 20;
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

std::map<std::pair<int, int>, int> Plane::get_points_on_radius()
{
    std::map<std::pair<int, int>, int> points;
    int num_segments = 16;

    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        vec3 point = vec3(cosf(theta), 0,sinf(theta)) * radius;

        vec3 rotated_point = rotationMatrix * point + get_pos();
        std::pair<int, int> key = std::make_pair(rotated_point.x, rotated_point.z);
        points[key] = rotated_point.y;
    }
    return points;
}

void Plane::update_light(int time_elapsed)
{
    light_pos[0] = position + rotationMatrix * (model_forward * light_offset.x + model_right * light_offset.y + model_up * light_offset.z);
    light_pos[1] = position + rotationMatrix * (model_forward * light_offset.x - model_right * light_offset.y + model_up * light_offset.z);

    total_time += time_elapsed;

    // Calculate the new light intensity
    float pulseSpeed = 0.001f;  // Adjust this to change the speed of the pulse
    float maxIntensity = 1.0f;  // Adjust this to change the maximum intensity

    light_intensity =  maxIntensity  * (1.0f + sin(pulseSpeed * total_time));
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

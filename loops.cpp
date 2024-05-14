#include "loops.h"
#include "frustum.h"
#include "helper.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <string>
#include <random>
#include <jsoncpp/json/json.h>

Loops::Loops(Json::Value settings, vec3 cameraPosition)
{

    std::string filename = settings["filename"].asString();
    float sc = settings["scale"].asFloat();

    float distance = settings["distance"].asFloat();


    // Calc position
    // Generate a random direction.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float azimuth = 2 * M_PI * dis(gen); // Random angle in [0, 2*pi]
    float elevation = acos(2 * dis(gen) - 1); // Random angle in [0, pi]

    // Convert spherical coordinates to Cartesian coordinates.
    vec3 direction(sin(elevation) * cos(azimuth), sin(elevation) * sin(azimuth), cos(elevation));

    // Scale by the desired distance and add to the airplane's position to get the loop's position.
    loopPos = cameraPosition + direction * distance;

    create_model(filename, loopPos, sc);

    std::cout << "File: " << filename << std::endl;

    calculate_radius();

    // Create model vetors
    model_right = vec3(settings["right"][0].asFloat(), settings["right"][1].asFloat(), settings["right"][2].asFloat());
    model_up = vec3(settings["up"][0].asFloat(), settings["up"][1].asFloat(), settings["up"][2].asFloat());
    model_forward = vec3(settings["forward"][0].asFloat(), settings["forward"][1].asFloat(), settings["forward"][2].asFloat());
}

void Loops::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed)
{
    //position += rotationMatrix * model_forward * speed * time_elapsed;
    //move(position);
    //tilt(keys_pressed);
    //update_light(time_elapsed);
    return;
}

void Loops::display(const GLuint& program, const mat4& world2view, const mat4& projection, vec3 light_int)
{
    Object::display(program, world2view, projection);
}

void Loops::calculate_radius()
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

vec3 Loops::get_pos()
{
    return loopPos - rotationMatrix * model_forward * offset_distance + rotationMatrix * model_up * 0;
}

vec3 Loops::get_lookAtPoint()
{
    return loopPos;
}

vec3 Loops::get_upVector()
{
    return model_up;
}

mat4 Loops::get_lookAtMatrix()
{
    return lookAtv(get_pos() +  rotationMatrix * model_up, loopPos + rotationMatrix * (model_forward * 5), rotationMatrix * model_up);
}

std::map<std::pair<int, int>, int> Loops::get_points_on_radius()
{
    std::map<std::pair<int, int>, int> points;
    int num_segments = 16;

    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        vec3 point1 = vec3(cosf(theta), 0,sinf(theta)) * radius + model_forward * radius;
        vec3 rotated_point1 = translationMatrix * rotationMatrix * point1;
        std::pair<int, int> key = std::make_pair(round(rotated_point1.x), round(rotated_point1.z));
        points[key] = rotated_point1.y;
    }

    return points;
}

void Loops::reset()
{
    rotationMatrix = IdentityMatrix();
    move(vec3(0, 30, 20));

    return;
}

Loops::~Loops()
{
    // Model deleted in object destructor
    // delete model;
}

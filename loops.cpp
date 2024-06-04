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

    sc = settings["scale"].asFloat();
    waypointer_sc = settings["waypointer_scale"].asFloat();

    offset_distance = settings["distance"].asFloat();

    allowedSpace_y_low = settings["allowedSpace_y_low"].asFloat();
    allowedSpace_y_high = settings["allowedSpace_y_high"].asFloat();

    loopPos = cameraPosition + offset_distance;

    if (loopPos.y < allowedSpace_y_low)
    {
        std::cout << "New loop position is outside the LOW allowed space\n";
        loopPos.y = allowedSpace_y_low;
    }
    if (loopPos.y > allowedSpace_y_high)
    {
        std::cout << "New loop position is outside the HIGH allowed space\n";
        loopPos.y = allowedSpace_y_high;
    }

    create_model(settings["filename"].asString(), loopPos, sc);

    waypoint_model = LoadModel(settings["waypointer_filename"].asString().c_str());
    waypoint_scale = S(waypointer_sc, waypointer_sc, waypointer_sc);

    calculate_radius();
}

void Loops::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed)
{
    vec3 current_direction = vec3(0, 1, 0); // The current direction of the arrow
    vec3 waypoint_direction = normalize(loopPos - lookAtPoint);
    vec3 rotation_axis = cross(current_direction, waypoint_direction);
    float rotation_angle = acos(dot(current_direction, waypoint_direction));
    waypoint_rotation = ArbRotate(rotation_axis, rotation_angle);
    waypoint_translation = T(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z);

    float distanceToCenter = length(lookAtPoint - loopPos);
    bool isInsideThisFrame = distanceToCenter < radius;

    if (!wasInsideLastFrame && isInsideThisFrame)
    {
        std::cout << "Airplane has entered the torus.\n";
        create_new_loop(lookAtPoint);
    }

    wasInsideLastFrame = isInsideThisFrame;
    return;
}

void Loops::display(const GLuint &program, const mat4 &world2view, const mat4 &projection, vec3 light_int)
{
    Object::display(program, world2view, projection);

    // Draw waypoint
    glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, waypoint_translation.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, waypoint_rotation.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, waypoint_scale.m);
    DrawModel(waypoint_model, program, "in_Position", "in_Normal", NULL);
}

float Loops::length(vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Loops::create_new_loop(vec3 cameraPosition)
{
    // Generate a new random position for the loop.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float azimuth = 2 * M_PI * dis(gen);      // Random angle in [0, 2*pi]
    float elevation = acos(2 * dis(gen) - 1); // Random angle in [0, pi]

    // Convert spherical coordinates to Cartesian coordinates.
    vec3 direction(sin(elevation) * cos(azimuth), sin(elevation) * sin(azimuth), cos(elevation));

    // Scale by the desired distance and add to the airplane's position to get the loop's position.

    vec3 newLoopPos = cameraPosition + direction * offset_distance;

    if (newLoopPos.y < allowedSpace_y_low)
    {
        std::cout << "New loop position is outside the LOW allowed space\n";
        newLoopPos.y = allowedSpace_y_low;
    }
    if (newLoopPos.y > allowedSpace_y_high)
    {
        std::cout << "New loop position is outside the HIGH allowed space\n";
        newLoopPos.y = allowedSpace_y_high;
    }

    // The new position is above the terrain. Create a new loop at this position.
    loopPos = newLoopPos;

    move(loopPos);
}

void Loops::calculate_radius()
{
    center = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < model->numVertices; i++)
    {
        center = center + model->vertexArray[i];
    }

    center = (center / model->numVertices) * sc;
    for (int i = 0; i < model->numVertices; i++)
    {
        vec3 dist_vec = model->vertexArray[i] - center;
        float dist = dist_vec * dist_vec;
        radius = fmax(radius, dist);
    }

    radius = sqrt(radius) * sc; // OBS SHOULD CHANGE THIS WHEN SCALING IS CHANGED, NOT IMPLEMENTED YET
}

Loops::~Loops()
{
    // Model deleted in object destructor
    delete waypoint_model;
}

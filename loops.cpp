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

    filename = settings["filename"].asString();
    sc = settings["scale"].asFloat();
    waypointer_filename = settings["waypointer_filename"].asString();
    waypointer_sc = settings["waypointer_scale"].asFloat();

    offset_distance = settings["distance"].asFloat();
    std::cout << "Offset distance: " << offset_distance << "\n";

    allowedSpace_y_low = settings["allowedSpace_y_low"].asFloat();
    allowedSpace_y_high = settings["allowedSpace_y_high"].asFloat();


    // Calc position
    // Generate a random direction.
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_real_distribution<> dis(0, 1);
    //float azimuth = 2 * M_PI * dis(gen); // Random angle in [0, 2*pi]
    //float elevation = acos(2 * dis(gen) - 1); // Random angle in [0, pi]

    // Convert spherical coordinates to Cartesian coordinates.
    //vec3 direction(sin(elevation) * cos(azimuth), sin(elevation) * sin(azimuth), cos(elevation));

    // Scale by the desired offset_distance and add to the airplane's position to get the loop's position.
    //loopPos = cameraPosition + direction * offset_distance;
    loopPos = cameraPosition + offset_distance;
    

    if (abs(loopPos.y) < allowedSpace_y_low) {
        std::cout << "New loop position is outside the LOW allowed space\n";
        loopPos.y = allowedSpace_y_low;
    }
    if (abs(loopPos.y) > allowedSpace_y_high) {
        std::cout << "New loop position is outside the HIGH allowed space\n";
        loopPos.y = allowedSpace_y_high;
    }

    // Calculate the waypoint_direction from the airplane to the loop
    //vec3 waypoint_direction = normalize(loopPos - cameraPosition);

    // Rotate the arrow to point in the calculated waypoint_direction
    //waypoint_rotation = rotation(vec3(0, 0, 1), waypoint_direction);

    // Position the arrow around the airplane
    //arrowModel.position = cameraPosition + cameraDirection * 10; // 10 is offset distance from airplane position

    create_model(filename, loopPos, sc);
    //create_model(waypointer_filename, arrowModel.position, waypointer_sc);

    waypoint_model = LoadModel(waypointer_filename.c_str());
    waypoint_scale = S(waypointer_sc, waypointer_sc, waypointer_sc);

    std::cout << "LOOP CREATED" << "\n";

    

    // Create model vetors
    model_right = vec3(settings["right"][0].asFloat(), settings["right"][1].asFloat(), settings["right"][2].asFloat());
    model_up = vec3(settings["up"][0].asFloat(), settings["up"][1].asFloat(), settings["up"][2].asFloat());
    model_forward = vec3(settings["forward"][0].asFloat(), settings["forward"][1].asFloat(), settings["forward"][2].asFloat());
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
    calculate_radius();
    //std::cout << "Distance to center: " << distanceToCenter << "Radius: " << radius << "\n";
    bool isInsideThisFrame = distanceToCenter < radius;

    if (!wasInsideLastFrame && isInsideThisFrame) {
        std::cout << "Distance to center: " << distanceToCenter << "Radius: " << radius << "\n";
        std::cout << "Airplane has entered the torus.\n";
        create_new_loop(lookAtPoint);
    } 
    /*else if (wasInsideLastFrame && !isInsideThisFrame) {
        std::cout << "Distance to center: " << distanceToCenter << "Radius: " << radius << "\n";
        std::cout << "Airplane has exited the torus.\n";
        //create_new_loop(cameraPosition);
    }*/

    wasInsideLastFrame = isInsideThisFrame;
    return;
}

void Loops::display(const GLuint& program, const mat4& world2view, const mat4& projection, vec3 light_int)
{
    Object::display(program, world2view, projection);

    // Draw waypoint
    glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, waypoint_translation.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, waypoint_rotation.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, waypoint_scale.m);
    DrawModel(waypoint_model, program, "in_Position", "in_Normal", "in_TexCoord");

}

float Loops::length(vec3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Loops::create_new_loop(vec3 cameraPosition)
{
    // Generate a new random position for the loop.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float azimuth = 2 * M_PI * dis(gen); // Random angle in [0, 2*pi]
    float elevation = acos(2 * dis(gen) - 1); // Random angle in [0, pi]

    // Convert spherical coordinates to Cartesian coordinates.
    vec3 direction(sin(elevation) * cos(azimuth), sin(elevation) * sin(azimuth), cos(elevation));

    // Scale by the desired distance and add to the airplane's position to get the loop's position.
    
    vec3 newLoopPos = cameraPosition + direction * offset_distance;
    std::cout << "camPos: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ") "
          << "dir: (" << direction.x << ", " << direction.y << ", " << direction.z << ") "
          << "offset: " << offset_distance
          << "new pos: (" << newLoopPos.x << ", " << newLoopPos.y << ", " << newLoopPos.z << ")\n";

    if (abs(newLoopPos.y) < allowedSpace_y_low) {
        std::cout << "New loop position is outside the LOW allowed space\n";
        newLoopPos.y = allowedSpace_y_low;
    }
    if (abs(newLoopPos.y) > allowedSpace_y_high) {
        std::cout << "New loop position is outside the HIGH allowed space\n";
        newLoopPos.y = allowedSpace_y_high;
    }
    // Check if the new position is under or inside the terrain.
    // Replace getTerrainHeight with the appropriate function in your program.
    //if (newLoopPos.y <= getTerrainHeight(newLoopPos.x, newLoopPos.z)) {
    //    // The new position is under or inside the terrain. Try again.
    //    create_new_loop(cameraPosition);
    //    return;
    //}

    // The new position is above the terrain. Create a new loop at this position.
    loopPos = newLoopPos;
    create_model(filename, loopPos, sc);
    std::cout << "New loop created.\n";
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

vec3 Loops::get_pos()
{
    return loopPos - rotationMatrix * model_forward * offset_distance + rotationMatrix * model_up * 0;
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

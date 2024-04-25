#include "game_state.h"
#include "object.h"
#include "context.h"
#include "ground.h"
#include "skydome.h"
#include "helper.h"
#include "plane.h"
#include "terrain_map.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <iostream>

Game_State::Game_State(Context *c) : State(c->settings["game_state"], c)
{
    map = new TerrainMap(context->settings["terrain"], cameraPosition, frustum_obj);
    skydome = new Skydome(context->settings["skydome"], cameraPosition);
    plane = new Plane(context->settings["planes"][0], vec3(0,100,0));
    glutHideCursor();
    return;
}

void Game_State::keyboard(unsigned char key, int x, int y)
{
    State::keyboard(key, x, y);

    if (key == 'p')
    {
        context->menu_state = true;
    }

    if (key == '1' and current_plane != 0) {
        vec3 tmp_pos = plane->get_lookAtPoint();
        delete plane;
        plane = new Plane(context->settings["planes"][0], tmp_pos);
        current_plane = 0;
    }
    if (key == '2' and current_plane != 1) {
        vec3 tmp_pos = plane->get_lookAtPoint();
        delete plane;
        plane = new Plane(context->settings["planes"][1], tmp_pos);
        current_plane = 1;
    }
    if (key == '3' and current_plane != 2) {
        vec3 tmp_pos = plane->get_lookAtPoint();
        delete plane;
        plane = new Plane(context->settings["planes"][2], tmp_pos);
        current_plane = 2;
    }
}

void Game_State::mouse(int x, int y)
{

    if (!keys_toggle['m'])
    {
        return;
    }


    // Warp the cursor back to the center of the window
    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    if (deltaX == -1 && deltaY == -1)
    {
        deltaX = 0;
        deltaY = 0;
        return;
    }
    // Calculate the delta from the center of the window
    int deltaX = x - glutGet(GLUT_WINDOW_WIDTH) / 2;
    int deltaY = y - glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // Update theta and phi based on the mouse movement
    // The 0.005f factor is a sensitivity adjustment - you can change this value to make the camera more or less sensitive to mouse movement
    theta += deltaX * 0.001f;
    phi -= deltaY * 0.001f;

    // Clamp phi to be between -pi/2 and pi/2 to prevent the camera from flipping upside down
    float epsilon = 0.01f; // Small value to prevent phi from reaching exactly 0 or pi/2
    phi = std::max(std::min(phi, M_PI / 2.0f - epsilon), epsilon - M_PI / 2.0f);

    // Calculate the direction vector
    // mouse_direction = vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));
    mouse_direction = vec3(sin(theta), 0, cos(theta));
}

void Game_State::move_camera(int time_elapsed)
{
    // Update the lookAtPoint based on the camera position and the direction

    // world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
    if (!keys_toggle['m'])
    {
        world2view = plane->get_lookAtMatrix();
    }
    else
    {
        lookAtPoint = mouse_direction;
        upVector = plane->get_upVector();
        world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
    }
}

void Game_State::update(int time_elapsed)
{
    // Update camera etc. here, then update objects.

    plane->update(time_elapsed, plane->get_pos(), lookAtPoint, keys_pressed);
    map->update(plane->get_pos(), world2view);
    skydome->update(time_elapsed, plane->get_pos(), lookAtPoint, keys_pressed);

    for (Object *object : objects)
    {
        object->update(time_elapsed, plane->get_pos(), lookAtPoint, keys_pressed);
    }
    move_camera(time_elapsed);
}

void Game_State::display()
{
    // Upload matrices to shader, then display objects, map etc.
    // We make sure to use the program before uploading matrices.
    glUseProgram(program);
    upload2shader();

    skydome->display(program, world2view, projection);
    map->display(program, world2view, projection);
    plane->display(program, world2view, projection);

    for (Object *object : objects)
    {
        object->display(program, world2view, projection);
    }
}

Game_State::~Game_State()
{
    // Objects, ground and skydome are deleted in State
    // Program is deleted in State
    delete plane;
    delete map;
    return;
}

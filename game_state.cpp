#include "game_state.h"
#include "object.h"
#include "context.h"
#include "ground.h"
#include "skydome.h"
#include "helper.h"


#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <iostream>

Game_State::Game_State(Context* c) : State(c->settings["game_state"], c)  {
    ground = new Ground();
    skydome = new Skydome(context->settings["skydome"], cameraPosition);
    objects.push_back(new Object("models/teapot.obj", vec3(0,0,0), 1));
    glutHideCursor();
    return;
}

void Game_State::keyboard(unsigned char key, int x, int y) {
    State::keyboard(key, x, y);

    switch (key)
    {
    case 'w':
        forward_move = true;
        backward_move = false;
        break;
    case 's':
        backward_move = true;
        forward_move = false;
        break;
    case 'a':
        left_move = true;
        right_move = false;
        break;
    case 'd':
        right_move = true;
        left_move = false;
        break;
    case 'z':
        up_move = false;
        down_move = true;
        break;
    case ' ':
        down_move = false;
        up_move = true;
        break;

    case 'p':
        context->menu_state = true;
    return;
    }
}

void Game_State::keyboard_up(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'w':
        forward_move = false;
        break;
    case 's':
        backward_move = false;
        break;
    case 'a':
        left_move = false;
        break;
    case 'd':
        right_move = false;
        break;
    case 'z':
        down_move = false;
        break;
    case ' ':
        up_move = false;
        break;
    return;
    }
}

void Game_State::mouse(int x, int y) {
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
    vec3 direction(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));

    // Update the lookAtPoint based on the camera position and the direction
    lookAtPoint = cameraPosition + direction;

    world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
}

void Game_State::move_camera(int time_elapsed) {
    float step_size = 0.01f;
    vec3 forward_direction = normalize(lookAtPoint - cameraPosition) * step_size * time_elapsed;
    vec3 side_direction = normalize(cross(forward_direction, upVector)) * step_size * time_elapsed;


    if (forward_move) {
        cameraPosition += forward_direction;
        lookAtPoint += forward_direction;
    }
    else if (backward_move) {
        cameraPosition -= forward_direction;
        lookAtPoint -= forward_direction;
    }
    if (left_move) {
        cameraPosition -= side_direction;
        lookAtPoint -= side_direction;
    }
    else if (right_move) {
        cameraPosition += side_direction;
        lookAtPoint += side_direction;
    }
    if (up_move) {
        cameraPosition += upVector * step_size * time_elapsed;
        lookAtPoint += upVector * step_size * time_elapsed;
    }
    else if (down_move) {
        cameraPosition -= upVector * step_size * time_elapsed;
        lookAtPoint -= upVector * step_size * time_elapsed;
    }

    world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
}


void Game_State::update(int time_elapsed) {
    // Update camera etc. here, then update objects.

    move_camera(time_elapsed);
    

    ground->update(time_elapsed, cameraPosition, lookAtPoint);
    skydome->update(time_elapsed, cameraPosition, lookAtPoint);

    for (Object* object : objects) {
        object->update(time_elapsed, cameraPosition, lookAtPoint);
    
    }
}

void Game_State::display() {
    // Upload matrices to shader, then display objects, map etc.
    // We make sure to use the program before uploading matrices.
    glUseProgram(program);
    upload2shader();

    skydome->display(program, world2view, projection);
    ground->display(program, world2view, projection);
    
    for (Object* object : objects) {
        object->display(program, world2view, projection);
    }
}

Game_State::~Game_State() {
    // Objects, ground and skydome are deleted in State
    // Program is deleted in State
    return;
}

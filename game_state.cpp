#include "game_state.h"
#include "object.h"
#include "context.h"
#include "ground.h"
#include "skydome.h"
#include "helper.h"
#include "plane.h"


#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <iostream>

Game_State::Game_State(Context* c) : State(c->settings["game_state"], c)  {
    ground = new Ground();
    skydome = new Skydome(context->settings["skydome"], cameraPosition);
    //objects.push_back(new Object("models/teapot.obj", vec3(0,0,0), 1));
    //objects.push_back(new Plane(context->settings["planes"][0], cameraPosition)); 
    plane = new Plane(context->settings["planes"][0], cameraPosition);
    glutHideCursor();
    return;
}

void Game_State::keyboard(unsigned char key, int x, int y) {
    State::keyboard(key, x, y);

    if (key == 'p') {
        context->menu_state = true;
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
    //lookAtPoint = cameraPosition + direction;

    //world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
}

void Game_State::move_camera(int time_elapsed) {
    
    //cameraPosition = plane -> position + vec3(0, 10, 10);

    //cameraPosition = plane -> position + vec3(0, 5, 10);
    cameraPosition = plane -> get_pos();

    //std::cout << "CAMERA NEW: " << vec2str( plane -> get_pos()) << std::endl;
    //std::cout << "CAMERA POSITION: " << vec2str(cameraPosition) << std::endl;

    //std::cout << "CAMERA POSITION: " << vec2str(plane -> position) << std::endl;

     // Calculate the direction vector
    vec3 direction(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));

    // Update the lookAtPoint based on the camera position and the direction
    //lookAtPoint = cameraPosition + direction;
    lookAtPoint = plane -> get_lookAtPoint();

    world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
}


void Game_State::update(int time_elapsed) {
    // Update camera etc. here, then update objects.
    

    ground->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);
    skydome->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);
    plane->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);

    for (Object* object : objects) {
        object->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);
    
    }
    move_camera(time_elapsed);
}

void Game_State::display() {
    // Upload matrices to shader, then display objects, map etc.
    // We make sure to use the program before uploading matrices.
    glUseProgram(program);
    upload2shader();

    skydome->display(program, world2view, projection);
    ground->display(program, world2view, projection);
    plane->display(program, world2view, projection);
    
    for (Object* object : objects) {
        object->display(program, world2view, projection);
    }
}

Game_State::~Game_State() {
    // Objects, ground and skydome are deleted in State
    // Program is deleted in State
    delete plane;
    return;
}

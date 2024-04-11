#include "game_state.h"
#include "object.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <iostream>

Game_State::Game_State(Context* c) : State(c) {
    Object* object = new Object("models/bunny.obj");
    objects.push_back(object);

    program = loadShaders("shaders/game_state.vert", "shaders/game_state.frag");
    glUseProgram(program);
    
    return;
}

void Game_State::keyboard(unsigned char key, int x, int y) {
    return;
}

void Game_State::keyboard_up(unsigned char key, int x, int y) {
    return;
}

void Game_State::mouse(int x, int y) {
    return;
}

void Game_State::update(int time_elapsed) {
    // Update camera etc. here, then update objects.

    for (Object* object : objects) {
        object->update(time_elapsed, cameraPosition, lookAtPoint);
    
    }
}

void Game_State::display() {
    // Upload matrices to shader, then display objects, map etc.
    // We make sure to use the program before uploading matrices.
    glUseProgram(program);
    upload2shader();
    
    for (Object* object : objects) {
        object->display(program, world2view, projection);
    }
}

Game_State::~Game_State() {
    return;
}

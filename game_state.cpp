#include "game_state.h"
#include "GL_utilities.h"
#include "object.h"
#include "MicroGlut.h"

Game_State::Game_State() {
    program = loadShaders("shaders/game_state.vert", "shaders/game_state.frag");
    Object* obj = new Object();
    obj->create_model("models/teapot.obj");
    objects.push_back(obj);

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

void Game_State::update() {
    for (Object* obj : objects) {
        obj->update();
    }
}

void Game_State::display() {
    glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    glUseProgram(program);
    for (Object* obj : objects) {
        obj->display(program);
    }
    glUseProgram(0);
    
}

Game_State::~Game_State() {
    for (Object* obj : objects) {
        delete obj;
    }
}   
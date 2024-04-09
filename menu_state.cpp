#include "menu_state.h"
#include "mini_plane.h"
#include "ground.h"
#include "context.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <jsoncpp/json/json.h>

#include <iostream>


Menu_State::Menu_State(Context* c) : State(c){
    Object* object;

    Json::Value settings = context->settings["mini_planes"];

    int nr_of_planes = settings.size();

    std::cout << "nr of planes: " << nr_of_planes << std::endl;

    int index = 0;
    for (int i = 0; i < 30; i++) {
        index = rand() % nr_of_planes;
        settings = context->settings["mini_planes"][index];
        std::cout << settings["path"].asString() << std::endl;
        object = new Mini_Plane(settings["path"].asString(), frustum_obj, settings, vec3(0,0,0), settings["scale"].asFloat());
        objects.push_back(object);
    }

    program = loadShaders("shaders/game_state.vert", "shaders/game_state.frag");
    glUseProgram(program);

    ground = new Ground();
    
    return;
}

void Menu_State::keyboard(unsigned char key, int x, int y) {
    State::keyboard(key, x, y);
}

void Menu_State::keyboard_up(unsigned char key, int x, int y) {
    return;
}

void Menu_State::mouse(int x, int y) {
    return;
}

void Menu_State::update(int time_elapsed) {
    // Update camera etc. here, then update objects.

    ground->update(time_elapsed, cameraPosition, lookAtPoint);

    for (Object* object : objects) {
        object->update(time_elapsed, cameraPosition, lookAtPoint);
        if (frustum_obj.side_culling(object->center + object -> position, object->radius, world2view)) {
            object->reset();
        }
    
    }
}

void Menu_State::display() {
    // Upload matrices to shader, then display objects, map etc.
    // We make sure to use the program before uploading matrices.
    glUseProgram(program);
    upload2shader();

    ground->display(program);
    
    
    for (Object* object : objects) {
        object->display(program);
    }
    
}

Menu_State::~Menu_State() {
    return;
}

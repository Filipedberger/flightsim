#include "menu_state.h"
#include "mini_plane.h"
#include "ground.h"
#include "skydome.h"
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
    for (int i = 0; i < nr_of_planes; i++) {
        planes.push_back(LoadModel(settings[i]["path"].asString().c_str()));
    }
    std::cout << "here: " << std::endl;
    int index = 0;
    for (int i = 0; i < 40; i++) {
        index = rand() % nr_of_planes;
        settings = context->settings["mini_planes"][index];
        
        object = new Mini_Plane(planes[index], frustum_obj, settings);
        objects.push_back(object);
    }

    program = loadShaders("shaders/game_state.vert", "shaders/game_state.frag");
    glUseProgram(program);

    ground = new Ground();
    std::string filename = "models/skybox-full.obj";
    float sc = 1.0f;
    skydome = new Skydome(filename, cameraPosition, sc);
    
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
    skydome->update(time_elapsed, cameraPosition, lookAtPoint);

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
    
    skydome->display(program);
    ground->display(program);
    
    
    
    for (Object* object : objects) {
        object->display(program);
    }
    
}

Menu_State::~Menu_State() {
    for (Object* object : objects) {
        delete object;
    }
    delete ground;
    glDeleteProgram(program);
    for (Model* plane : planes) {
        delete plane;
    }
}

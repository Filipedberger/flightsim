#include "menu_state.h"
#include "mini_plane.h"
#include "ground.h"
#include "skydome.h"
#include "context.h"
#include "helper.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <jsoncpp/json/json.h>

#include <iostream>


Menu_State::Menu_State(Context* c) : State(c->settings["menu_state"], c) {
    Object* object;

    // Read settings from json file
    Json::Value plane_settings = context->settings["mini_planes"];
    int nr_of_planes = plane_settings.size(); // Number of different planes

    // Load all planes
    for (int i = 0; i < nr_of_planes; i++) {
        planes.push_back(LoadModel(plane_settings[i]["path"].asString().c_str()));
    }

    int index = 0;
    for (int i = 0; i < settings["nr_of_planes"].asInt(); i++) {
        // Randomize plane
        index = rand() % nr_of_planes;

        plane_settings = context->settings["mini_planes"][index];
        object = new Mini_Plane(planes[index], frustum_obj, plane_settings);

        objects.push_back(object);
    }

    ground = new Ground();

    skydome = new Skydome(context->settings["skydome"], cameraPosition);

    glutShowCursor();
    
   
    return;
}

void Menu_State::keyboard(unsigned char key, int x, int y) {
    State::keyboard(key, x, y);

    switch (key)
    {
    case 'p':
        context->game_state = true;
        return;
    }
}

void Menu_State::keyboard_up(unsigned char key, int x, int y) {
    return;
}

void Menu_State::mouse(int x, int y) {
    return;
}

void Menu_State::update(int time_elapsed) {
    // Update camera etc. here, then update objects.


    ground->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);
    skydome->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);

    for (Object* object : objects) {
        object->update(time_elapsed, cameraPosition, lookAtPoint, keys_pressed);
        vec3 p = object->center + object->position;
        if (frustum_obj.side_culling(p, object->radius, world2view)) {
            object->reset();
        }
    
    }
}

void Menu_State::display() {
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

Menu_State::~Menu_State() {
    // Objects, ground and skydome are deleted in State
    // Program is deleted in State

    for (Model* plane : planes) {
        delete plane;
    }
}

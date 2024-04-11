#include "menu_state.h"
#include "mini_plane.h"
#include "ground.h"
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

    program = loadShaders(settings["shader_vert"].asString().c_str(), settings["shader_frag"].asString().c_str());
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

#include "hud.h"
#include "frustum.h"
#include "GL_utilities.h"
#include "LoadTGA.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <random>
#include <jsoncpp/json/json.h>


Hud::Hud(Json::Value& c, const vec3& cameraPosition)
    : Object(c["filename"].asString(), cameraPosition, c["scale"].asFloat()) {
    std::cout << "HUD CREATED" << "\n";
    speed = 0;
    altitude = 0;
    bearing = 0;
    position = vec3(0, 0, 0);

    //object_program = loadShaders(c["shader_vert"].asString().c_str(), c["shader_frag"].asString().c_str());

    //LoadTGATextureSimple(c["texture"].asString().c_str(), &hudtex);
    return;
}

Hud::setSpeed(float speed) {
    this->speed = speed;
}

Hud::setAltitude(float altitude) {
    this->altitude = altitude;
}

Hud::setBearing(float bearing) {
    this->bearing = bearing;
}

Hud::setPosition(vec3 position) {
    this->position = position;
}

void Hud::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) {
    // Update the HUD based on the current game state.
    // This might involve fetching the current speed, altitude, bearing, and position from the game state.
}

void Hud::display(const GLuint& program, const mat4& world2view, const mat4& projection) {
    // Render the HUD here. This will depend on how you've set up text rendering.
    // For example, you might have a function `renderText` that takes a string and a position.
    renderText("Speed: " + std::to_string(speed), vec2(10, 10));
    renderText("Altitude: " + std::to_string(altitude), vec2(10, 30));
    renderText("Bearing: " + std::to_string(bearing), vec2(10, 50));
    renderText("Position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")", vec2(10, 70));
}

void Hud::reset() {
    // Reset the HUD to its initial state.
    speed = 0;
    altitude = 0;
    bearing = 0;
    position = vec3(0, 0, 0);
}

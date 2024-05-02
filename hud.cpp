#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "hud.h"
#include "frustum.h"
#include "GL_utilities.h"
#include "LoadTGA.h"



#include "math.h"
#include "cmath"
#include <iostream>
#include <random>
#include <jsoncpp/json/json.h>

#define SCREEN_WIDTH 1100
#define SCREEN_HEIGHT 1100


Hud::Hud(Json::Value& c, const vec3& cameraPosition, Object* object) 
: Object(c["filename"].asString(), cameraPosition, c["scale"].asFloat()), airplane(object) {
    std::cout << "HUD CREATED" << "\n";
    speed = 0;
    altitude = 0;
    bearing = 0;
    position = vec3(0, 0, 0);

    return;
}


void Hud::setSpeed() {
    //speed = airplane.get_planeSpeed();
    Plane* plane = dynamic_cast<Plane*>(airplane);
    if (plane != nullptr) {
        speed = plane->get_planeSpeed() * 100; // to no have decimal values
    }
    //this->speed = airplane.get_planeSpeed();
    //this->speed = speed;
}

void Hud::setAltitude(float altitude) {
    this->altitude = altitude;
}

void Hud::setBearing(float bearing) {
    this->bearing = bearing;
}

void Hud::setPosition(vec3 position) {
    this->position = position;
}

void Hud::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) {
    setPosition(lookAtPoint);
    setSpeed();
}

void Hud::display(const GLuint& program, const mat4& world2view, const mat4& projection) {

    // Save the current projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up orthographic projection
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    // Save the current modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing
    //glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
    glDisable(GL_DEPTH_TEST);
    
    // Render the HUD here
    // ... Render 2D ...

    // Restore the modelview matrix
    glPopMatrix();
    std::cout << "Speed: " << speed << "\n";
    print(300, 300, "Speed: " + std::to_string(speed));
    //print(200, 200, "Position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")");

    // Restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Re-enable depth testing
    //glDepthMask(GL_TRUE);  // enable writes to Z-Buffer
    glEnable(GL_DEPTH_TEST);
}

void Hud::print(int x, int y, const std::string& string)
{
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2i(x, y);

    //check if the raster position is valid
    GLint valid;
    glGetIntegerv(GL_CURRENT_RASTER_POSITION_VALID, &valid);
    if (valid == GL_FALSE)
    {
        std::cout << "Raster position is not valid\n";
    }

    //get the current raster position in clip coordinates
    GLfloat pos[4];
    glGetFloatv(GL_CURRENT_RASTER_POSITION, pos);

    //convert clip coordinates to window coordinates
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    pos[0] = ((pos[0] + 1) / 2) * viewport[2] + viewport[0];
    pos[1] = ((pos[1] + 1) / 2) * viewport[3] + viewport[1];

    std::cout << "Raster position: (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << ")\n";

    //set the color of the text to red
    glColor3f(1.0f, 0.0f, 0.0f);

    //get the length of the string to display
    int len = string.length();

    //loop to display character by character
    for (int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
    }
}


void Hud::reset() {
    // Reset the HUD to its initial state.
    speed = 0;
    altitude = 0;
    bearing = 0;
    position = vec3(0, 0, 0);
}

Hud::~Hud() {
    // Clean up any resources used by the HUD.
    delete model;
}

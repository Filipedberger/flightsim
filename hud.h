#ifndef HUD_H
#define HUD_H

#include "object.h"
#include "plane.h"
#include "frustum.h"

//#include "context.h"
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <jsoncpp/json/json.h>
#include <string>

class Hud : public Object {
public:
    Hud() : airplane(nullptr) {}
    //Hud(const std::string& filename, const vec3& camera_pos, float sc);
    Hud(Json::Value& c, const vec3& cameraPosition, Object* object);
     

    void setSpeed();
    void setAltitude(float altitude);
    void setBearing(float bearing);
    void setPosition(vec3 position);

    void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) override;

    void display(const GLuint& program, const mat4& world2view, const mat4& projection) override;

    void print(int x, int y, const std::string& string);

    void reset() override;

    ~Hud() override;

private:
    //Plane& airplane;
    Object* airplane = nullptr;
    float speed;
    float altitude;
    float bearing;
    vec3 position;
};

#endif
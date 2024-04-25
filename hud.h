#include "hud.h"
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <string>

class Hud : public Object {
public:
    Hud() = default;
    Hud(const std::string& filename, const vec3& camera_pos, float sc) : Object(filename, camera_pos, sc) {}
     

    void setSpeed(float speed) { this->speed = speed; }
    void setAltitude(float altitude) { this->altitude = altitude; }
    void setBearing(float bearing) { this->bearing = bearing; }
    void setPosition(vec3 position) { this->position = position; }

    void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) override {
        // Update the HUD based on the current game state.
        // This might involve fetching the current speed, altitude, bearing, and position from the game state.
    }

    void display(const GLuint& program, const mat4& world2view, const mat4& projection) override {
        // Render the HUD here. This will depend on how you've set up text rendering.
        // For example, you might have a function `renderText` that takes a string and a position.
        renderText("Speed: " + std::to_string(speed), vec2(10, 10));
        renderText("Altitude: " + std::to_string(altitude), vec2(10, 30));
        renderText("Bearing: " + std::to_string(bearing), vec2(10, 50));
        renderText("Position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")", vec2(10, 70));
    }

    void reset() override {
        // Reset the HUD to its initial state.
        speed = 0;
        altitude = 0;
        bearing = 0;
        position = vec3(0, 0, 0);
    }

    ~Hud() override {}

private:
    float speed;
    float altitude;
    float bearing;
    vec3 position;
};
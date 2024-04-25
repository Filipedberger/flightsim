
#ifndef HUD_H
#define HUD_H

#include "object.h"
#include "frustum.h"
#include <jsoncpp/json/json.h>

class Hud : public Object {
    public:
        Hud() = default;
        

        void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) override;
        void display(const GLuint& program, const mat4& world2view, const mat4& projection) override;
        void reset() override;

        ~Hud() override;

    private:
        
};
#endif
#ifndef LOOPS_H
#define LOOPS_H

#include "object.h"
#include "frustum.h"
#include <jsoncpp/json/json.h>

class Loops : public Object {
    public:
        //Loops();
        Loops(Json::Value settings, vec3 cameraPosition);
        void display(const GLuint& program, const mat4& world2view, const mat4& projection, vec3 light_int) override;
        void update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) override;
        void reset() override;
        ~Loops() override;

        float length(vec3 v);
        void create_new_loop(vec3 cameraPosition);

        // Getters:
        vec3 get_pos() override;
        //vec3 get_lookAtPoint() override;
        mat4 get_lookAtMatrix() override;
        //vec3 get_upVector() override;
        std::map<std::pair<int, int>, int> get_points_on_radius() override;

    private:
        std::string filename;
        std::string waypointer_filename;
        //float speed; // Pixels per millisecond
        float sc;
        float waypointer_sc;
        vec3 loopPos;
        vec3 model_up;
        vec3 model_forward;
        vec3 model_right;

        float allowedSpace_y_low = 150;
        float allowedSpace_y_high = 150;

        float offset_distance = 1;
        bool wasInsideLastFrame = false;
        float radius = 0;

        
        void calculate_radius();

        // WAYPOINTER
        Model* waypoint_model;
        mat4 waypoint_rotation = IdentityMatrix();
        mat4 waypoint_translation = IdentityMatrix();
        mat4 waypoint_scale = IdentityMatrix();
        vec3 waypoint_position = vec3(0,0,0);


        


};

#endif
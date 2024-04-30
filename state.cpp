#include "state.h"
#include "frustum.h"
#include "context.h"
#include "helper.h"
#include <iostream>

State::State(Json::Value s, Context *c)
{

    cameraPosition = json2vec(s["camera_pos"]);
    lookAtPoint = json2vec(s["look_at"]);
    upVector = json2vec(s["up"]);

    create_world2view(cameraPosition, lookAtPoint, upVector);
    create_projection_from_json(s);
    program = loadShaders(s["shader_vert"].asString().c_str(), s["shader_frag"].asString().c_str());

    context = c;
    settings = s;
}

void State::upload2shader()
{
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, world2view.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, projection.m);
    GLint lightDirUniform = glGetUniformLocation(program, "light_pos");
    glUniform3f(lightDirUniform, 0.0f, 1.0f, -1.0f); // Set the light direction
    GLint lightIntensityUniform = glGetUniformLocation(program, "light_intensity");
    glUniform3f(lightIntensityUniform, light_intensity, light_intensity, light_intensity); 
    glUniform3f(glGetUniformLocation(program, "camera_pos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

void State::create_world2view(vec3 cameraPosition, vec3 lookAtPoint, vec3 upVector)
{
    this->cameraPosition = cameraPosition;
    this->lookAtPoint = lookAtPoint;
    this->upVector = upVector;
    world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
}

void State::create_projection(float near, float far, float right, float left, float top, float bottom)
{
    projection = frustum(left, right, bottom, top, near, far);
    frustum_obj = Frustum(near, far, right, left, top, bottom);
}

void State::create_projection_from_json(Json::Value s)
{
    create_projection(s["near"].asFloat(), s["far"].asFloat(), s["right"].asFloat(), s["left"].asFloat(), s["top"].asFloat(), s["bottom"].asFloat());
}

void State::keyboard(unsigned char key, int x, int y)
{
    keys_pressed[key] = true;
    switch (key)
    {
    case 27:
        exit(0);
        break;
    
    case GLUT_KEY_UP:
        keys_pressed['\1'] = true;
        break;
    case GLUT_KEY_DOWN:
        keys_pressed['\2'] = true;
        break;
    case GLUT_KEY_LEFT:
        keys_pressed['\3'] = true;
        break;
    case GLUT_KEY_RIGHT:
        keys_pressed['\4'] = true;
        break;
    }
    
}

void State::keyboard_up(unsigned char key, int x, int y)
{
    keys_pressed[key] = false;
    keys_toggle[key] = !keys_toggle[key];

    switch (key)
    {
    case GLUT_KEY_UP:
        keys_pressed['\1'] = false;
        break;
    case GLUT_KEY_DOWN:
        keys_pressed['\2'] = false;
        break;
    case GLUT_KEY_LEFT:
        keys_pressed['\3'] = false;
        break;
    case GLUT_KEY_RIGHT:
        keys_pressed['\4'] = false;
        break;
    }
}

State::~State()
{
    for (Object *object : objects)
    {
        delete object;
    }
    delete ground;
    delete skydome;
    glDeleteProgram(program);
}

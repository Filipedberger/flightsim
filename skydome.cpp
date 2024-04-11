#include "skydome.h"
#include "frustum.h"
#include "GL_utilities.h"
#include "LoadTGA.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <random>
#include <jsoncpp/json/json.h>

Skydome::Skydome(Json::Value& c, const vec3& cameraPosition)
    : Object(c["filename"].asString(), cameraPosition, c["scale"].asFloat()) {
    std::cout << "SKYDOME CREATED" << "\n";

    object_program = loadShaders(c["shader_vert"].asString().c_str(), c["shader_frag"].asString().c_str());

    LoadTGATextureSimple(c["texture"].asString().c_str(), &skyBoxtex);
    return;
}
    

Skydome::Skydome(const std::string& filename, const vec3& cameraPosition, float sc)
    : Object(filename, cameraPosition, sc) // Loads model based on filename, positions and scales
    {
    std::cout << "SKYDOME CREATED" << "\n";

    object_program = loadShaders("shaders/skydome.vert", "shaders/skydome.frag");
    printError("Skybox Shader compile error");

    LoadTGATextureSimple("textures/DaylightBoxUV.tga", &skyBoxtex);
    glUniform1i(glGetUniformLocation(object_program, "texUnit"), 0);
    printError("Skybox texUnit error");

    return;
}

void Skydome::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) {
    vec3 new_pos = vec3(cameraPosition.x, cameraPosition.y-10, cameraPosition.z);
    move(new_pos); 
    return;
}

void Skydome::display(const GLuint& program, const mat4& world2view, const mat4& projection) {
    glUseProgram(object_program);
    
	glUniformMatrix4fv(glGetUniformLocation(object_program, "viewMatrix"), 1, GL_TRUE, world2view.m);
    glUniformMatrix4fv(glGetUniformLocation(object_program, "in_projectionMatrix"), 1, GL_TRUE, projection.m);
	glActiveTexture(GL_TEXTURE0);

    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    glBindTexture(GL_TEXTURE_2D, skyBoxtex);
    upload2shader(object_program);
    DrawModel(model, object_program, "in_Position", NULL, "in_TexCoord");

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    glUseProgram(program);
    return;
}

void Skydome::reset() {
    rotationMatrix = IdentityMatrix();
    return;
}

Skydome::~Skydome() {
    delete model;
    glDeleteProgram(object_program);
}




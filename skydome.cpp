#include "skydome.h"
#include "frustum.h"
#include "GL_utilities.h"
#include "LoadTGA.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <random>

Skydome::Skydome(const std::string& filename, const vec3& cameraPosition, float sc)
    : Object(filename, cameraPosition, sc) // Loads model based on filename, positions and scales
    {
    std::cout << "SKYDOME CREATED" << "\n";
    object_program = loadShaders("shaders/skydome.vert", "shaders/skydome.frag");
    printError("Skybox Shader compile error");
    LoadTGATextureSimple("textures/DaylightBoxUV.tga", &skyBoxtex);
    return;
}

void Skydome::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) {
    //move(cameraPosition); 
    //rotate(rotation.x, vec3(1,0,0));
    return;
}

void Skydome::display(const GLuint& program) {
    //printf("Skydome display\n");
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, scaleMatrix.m);
    DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");
    
    //glUseProgram(object_program);
    upload2shader(object_program);

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glUseProgram(program);
    return;
}

void Skydome::upload2shader(const GLuint& object_program) {
    //printf("Skydome upload2shader\n");
    glUseProgram(object_program);
	glUniform1i(glGetUniformLocation(object_program, "texUnit"), 0);
    printError("Skybox texUnit error");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyBoxtex);

}

void Skydome::reset() {
    rotationMatrix = IdentityMatrix();
    return;
}

Skydome::~Skydome() {
    delete model;
}




#include "skydome.h"
#include "frustum.h"

#include "math.h"
#include "cmath"
#include <iostream>
#include <random>

Skydome::Skydome(const std::string& filename, const vec3& cameraPosition, float sc)
    : Object(filename, cameraPosition, sc) // Loads model based on filename, positions and scales
    {
    std::cout << "SKYDOME CREATED" << std::endl;
    return;
}

void Skydome::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint) {
    move(cameraPosition); 
    //rotate(rotation.x, vec3(1,0,0));
    return;
}

void Skydome::display(const GLuint& program) {
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//glBindTexture(GL_TEXTURE_2D, skyBoxtex);
	//glUniformMatrix4fv(glGetUniformLocation(textureProgram, "model2world"), 1, GL_TRUE, skyPos.m);
	//DrawModel(skybox, textureProgram, "in_Position", NULL, "in_TexCoord");
	
    upload2shader(program);
    DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    return;
}

void Skydome::reset() {
    rotationMatrix = IdentityMatrix();
    return;
}

Skydome::~Skydome() {
    delete model;
}


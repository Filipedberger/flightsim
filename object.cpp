#include "object.h"

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"

Object::Object() {
    return;
}

void Object::create_model(std::string filename, int x_pos, int y_pos, int z_pos, int scale) {
    model = LoadModel(filename.c_str());
    x = x_pos;
    y = y_pos;
    z = z_pos;
    mdl_matrix = T(x, y, z) * S(scale, scale, scale);

}

void Object::update() {
    return;
}

void Object::display(GLuint program) {
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, mdl_matrix.m);
    DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");
}
#include "object.h"

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"

Object::Object() {
    return;
}

void Object::create_model(std::string filename, vec3 pos, float sc) {
    model = LoadModel(filename.c_str());
    position = pos;
    scale = sc;
    update_model2world();

}

void Object::update() {
    return;
}

void Object::display(GLuint program) {
    upload2shader(program);
    DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");
}

void Object::translate(vec3 translation) {
        position = position + translation;
        update_model2world();
    }

void Object::move(vec3 coordinates) {
    position = coordinates;
    update_model2world();
}

void Object::rotate(float angle, vec3 axis) {
    model2world = model2world * ArbRotate(axis, angle);
}

void Object::update_model2world() {
        model2world = T(position.x, position.y, position.z) * S(scale);
    }

void Object::upload2shader(GLuint program) {
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, model2world.m);
    }
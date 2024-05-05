#include "object.h"

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"

Object::Object(const std::string& filename, vec3 pos, float sc) {
    create_model(filename, pos, sc);
}

void Object::create_model(const std::string& filename, vec3 pos, float sc) {
    model = LoadModel(filename.c_str());
    move(pos);
    scale(sc);

}

void Object::update(int time_elapsed, vec3 cameraPosition, vec3 lookAtPoint, std::map<char, bool> keys_pressed) {
    return;
}

void Object::display(const GLuint& program, const mat4& world2view, const mat4& projection, vec3 light_int) {
    upload2shader(program);
    DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");
}

void Object::translate(vec3 translation) {
        position = position + translation;
        translationMatrix = T(position.x, position.y, position.z);
    }

void Object::move(vec3 coordinates) {
    position = coordinates;
    translationMatrix = T(position.x, position.y, position.z);
}

void Object::rotate(float angle, vec3 axis) {
    rotationMatrix = rotationMatrix * ArbRotate(axis, angle);
}

void Object::scale(float sc) {
    scale_factor = sc;
    scaleMatrix = S(scale_factor, scale_factor, scale_factor);

}

void Object::reset() {
    return;
}


void Object::upload2shader(const GLuint& program) {
        glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, scaleMatrix.m);
}
#include "object.h"

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"

Object::Object() {
    return;
}

void Object::create_model(std::string filename, int x, int y, int z, float sc) {
    model = LoadModel(filename.c_str());
    position = vec3(x, y, z);
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
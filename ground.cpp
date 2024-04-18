#include "ground.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

Ground::Ground() {

    float kGroundSize = 100.0f;

    vertices[0] = vec3(-kGroundSize,0.0f,-kGroundSize);
    vertices[1] = vec3(-kGroundSize,0.0f,kGroundSize);
    vertices[2] = vec3(kGroundSize,-0.0f,-kGroundSize);
    vertices[3] = vec3(kGroundSize,-0.0f,kGroundSize);
    
    vertex_normals[0] = vec3(0.0f,1.0f,0.0f);
    vertex_normals[1] = vec3(0.0f,1.0f,0.0f);
    vertex_normals[2] = vec3(0.0f,1.0f,0.0f);
    vertex_normals[3] = vec3(0.0f,1.0f,0.0f);

    tex_coords[0] = vec2(0.0f,0.0f);
    tex_coords[1] = vec2(0.0f,20.0f);
    tex_coords[2] = vec2(20.0f,0.0f);
    tex_coords[3] = vec2(20.0f,20.0f);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 1;
    indices[4] = 3;
    indices[5] = 2;

    model = LoadDataToModel(
        vertices,
        vertex_normals,
        tex_coords,
        NULL,
        indices,
        4,
        6
    );
    translate(vec3(0.0f,-5.0f,0.0f));
}

void Ground::reset() {
    return;
}
#ifndef GROUND_H
#define GROUND_H

#include "object.h"

#include "VectorUtils4.h"
#include "LittleOBJLoader.h"


class Ground : public Object {
    public:
    Ground();
    void reset() override;
    ~Ground() = default;

    private:

    const float kGroundSize = 100.0f;

    vec3 vertices[4];

    vec3 vertex_normals[4];

    vec2 tex_coords[4];

    GLuint indices[6];
    
};




#endif
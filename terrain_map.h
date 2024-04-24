#ifndef TERRAIN_MAP_H
#define TERRAIN_MAP_H

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include <string>
#include <map>
#include "PerlinNoise.hpp"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "frustum.h"

class TerrainMap
{
public:
    TerrainMap(vec3 cameraPosition, const Frustum& f);
    ~TerrainMap();
    void update(vec3 cameraPosition, const mat4 &world2view);
    void display(const GLuint &program, const mat4 &world2view, const mat4 &projection);

private:
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};
    std::map<std::pair<int, int>, Model *> chunks;
    int terrainWidth = 256;
    int terrainHeight = terrainWidth;
    Model *GeneratePerlinTerrain(int offsetX, int offsetZ);
    vec3 cameraPos;
    int cameraChunkX;
    int cameraChunkZ;
    int CHUNKS = 2;
    const int MAX_CHUNK_DISTANCE = 3 * CHUNKS;
    GLuint terrain_program = loadShaders("shaders/terrain_shader.vert", "shaders/terrain_shader.frag");
    Frustum frustum_obj;

};

#endif
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

class TerrainMap
{
public:
    TerrainMap(vec3 pos = vec3(0, 50.0, 0), float sc = 1);
    ~TerrainMap();
    void update(vec3 cameraPosition);
    void display();
    void create_chunk(int offsetX, int offsetZ);

private:
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};
    std::map<std::pair<int, int>, Model *> chunks;
    int terrainWidth = 128;
    int terrainHeight = 128;
    Model *GeneratePerlinTerrain(int offsetX, int offsetZ);
    vec3 cameraPos;
    int cameraChunkX;
    int cameraChunkZ;
    int CHUNKS = 8;
    const int MAX_CHUNK_DISTANCE = 3 * CHUNKS;
    GLuint terrain_program = loadShaders("shaders/terrain_shader.vert", "shaders/terrain_shader.frag");
};

#endif
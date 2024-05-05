#ifndef TERRAIN_MAP_H
#define TERRAIN_MAP_H

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include <string>
#include <map>
#include <jsoncpp/json/json.h>
#include "PerlinNoise.hpp"
#include "SimplexNoise.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "frustum.h"

class TerrainMap
{
public:
    TerrainMap(Json::Value settings, vec3 cameraPosition, const Frustum &f);
    ~TerrainMap();
    void update(vec3 cameraPosition, const mat4 &world2view);
    void display(const GLuint &program, const mat4 &world2view, const mat4 &projection, vec3 cameraPosition);

    bool collision(std::map<std::pair<int, int>, int> points);

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
    int CHUNKS = 6;
    const int MAX_CHUNK_DISTANCE = 3 * CHUNKS;
    GLuint terrain_program = loadShaders("shaders/terrain_shader.vert", "shaders/terrain_shader.frag");
    Frustum frustum_obj;

    // Calculate the number of vertices and triangles
    int vertexCount;
    int triangleCount;

    float amplitude;
    float frequency;

    float snow;
    float rock;
    float grass;
    float sand;
    float water;

    float snow_inter;
    float rock_inter;
    float grass_inter;
    float water_to_sand;

    float rock_size;
    float grass_size;
    float sand_size;

    SimplexNoise *noise_test;

    std::pair<int, int> getChunk(int x, int z);

    //TESTING
    int tmp_x;
    int tmp_z;
    int tmp_x2;
    int tmp_z2;
};

#endif
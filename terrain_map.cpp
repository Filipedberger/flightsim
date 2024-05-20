#include "terrain_map.h"
#include "iostream"
#include <chrono>
#include <omp.h>
#include "frustum.h"
#include <jsoncpp/json/json.h>

TerrainMap::TerrainMap(Json::Value settings, vec3 cameraPosition, const Frustum &f)
{
    cameraChunkX = cameraPosition.x / terrainWidth;
    cameraChunkZ = cameraPosition.z / terrainHeight;

    frustum_obj = f;

    // Calculate the number of vertices and triangles
    vertexCount = terrainWidth * terrainHeight;
    triangleCount = (terrainWidth - 1) * (terrainHeight - 1) * 2;

    frequency = settings["frequency"].asFloat();
    amplitude = settings["amplitude"].asFloat();

    snow = -amplitude + settings["snow_level"].asFloat() * amplitude * 2;
    rock = -amplitude + settings["rock_level"].asFloat() * amplitude * 2;
    grass = -amplitude + settings["grass_level"].asFloat() * amplitude * 2;
    sand = -amplitude + settings["sand_level"].asFloat() * amplitude * 2;
    water = sand;

    rock_size = (settings["snow_level"].asFloat() - settings["rock_level"].asFloat()) * amplitude * 2;
    grass_size = (settings["rock_level"].asFloat() - settings["grass_level"].asFloat()) * amplitude * 2;
    sand_size = (settings["grass_level"].asFloat() - settings["sand_level"].asFloat()) * amplitude * 2;

    snow_inter = settings["snow_inter"].asFloat() * rock_size;
    rock_inter = settings["rock_inter"].asFloat() * grass_size;
    grass_inter = settings["grass_inter"].asFloat() * sand_size;
    water_to_sand = settings["water_to_sand"].asFloat() * sand_size;


    noise = new SimplexNoise(1.0, 1.0);
}

TerrainMap::~TerrainMap()
{
    for (auto &chunk : chunks)
    {
        delete chunk.second;
        chunk.second = nullptr;
    }
    chunks.clear();
}

std::pair<int, int> TerrainMap::getChunk(int x, int z)
{
    int chunkX;
    int chunkZ;
    if (x < 0) {
        chunkX = (x - terrainWidth) / (terrainWidth);
    }
    else {
        chunkX = x / (terrainWidth);
    }
    if (z < 0) {
        chunkZ = (z - terrainHeight) / (terrainHeight);
    }
    else {
        chunkZ = z / (terrainHeight);
    }
    return {chunkX, chunkZ};
}

bool TerrainMap::collision(std::map<std::pair<int, int>, int> points)
{
    for (auto &point : points){
        int x = point.first.first;
        int z = point.first.second;
        int y = point.second;

        std::pair<int, int> chunk = getChunk(x, z);

        // Check if the chunk exists
        if (chunks.find(chunk) == chunks.end())
        {
            continue;
        }

        // Calculate the position of the vertex in the chunk
        if (x < 0) {
            x = -x;
            x = (terrainWidth-1) - x % (terrainWidth);
        }
        else {
            x = x - chunk.first * (terrainWidth);
        }
        if (z < 0) {
            z = -z;
            z = (terrainHeight-1) - z % (terrainHeight);
        }
        else {
            z = z - chunk.second * (terrainHeight);
        }

        Model *chunkModel = chunks[chunk];         

        if (y < chunkModel -> vertexArray[x + z * terrainWidth].y)
        {
            return true;
        }
    }
    return false;
}

void TerrainMap::update(vec3 cameraPosition, const mat4 &world2view)
{
    float rad_sq = (CHUNKS * terrainWidth) * (CHUNKS * terrainWidth);
    cameraChunkX = cameraPosition.x / terrainWidth;
    cameraChunkZ = cameraPosition.z / terrainHeight;

    for (int x = cameraChunkX - CHUNKS; x <= cameraChunkX + CHUNKS; ++x)
    {
        float chunkX = ((x * (terrainWidth)) - cameraPosition.x) * ((x * (terrainWidth)) - cameraPosition.x);

        for (int z = cameraChunkZ - CHUNKS; z <= cameraChunkZ + CHUNKS; ++z)
        {
            float chunkZ = (z * (terrainHeight) - cameraPosition.z) * (z * (terrainHeight) - cameraPosition.z);
            if (chunkX + chunkZ > rad_sq)
            {
                continue;
            }

            if (chunks.find({x, z}) == chunks.end())
            {
                chunks[{x, z}] = GeneratePerlinTerrain(x * (terrainWidth - 2), z * (terrainHeight - 2));
            }
        }
    }

    //  Iterate over all chunks
    for (auto it = chunks.begin(); it != chunks.end();)
    {
        // Calculate the distance from the chunk to the camera
        int dx = it->first.first - cameraChunkX;
        int dz = it->first.second - cameraChunkZ;
        int distanceSquared = dx * dx + dz * dz;

        // If the chunk is too far from the camera, offload it
        if (distanceSquared > MAX_CHUNK_DISTANCE * MAX_CHUNK_DISTANCE)
        {
            // Delete the model
            delete it->second;
            it->second = nullptr;

            // Remove the chunk from the map
            it = chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }

}

void TerrainMap::display(const GLuint &program, const mat4 &world2view, const mat4 &projection, vec3 cameraPosition)
{
    glUniform1f(glGetUniformLocation(program, "snow"), snow);
    glUniform1f(glGetUniformLocation(program, "rock"), rock);
    glUniform1f(glGetUniformLocation(program, "grass"), grass);
    glUniform1f(glGetUniformLocation(program, "sand"), sand);

    glUniform1f(glGetUniformLocation(program, "snow_inter"), snow_inter);
    glUniform1f(glGetUniformLocation(program, "rock_inter"), rock_inter);
    glUniform1f(glGetUniformLocation(program, "grass_inter"), grass_inter);
    glUniform1f(glGetUniformLocation(program, "water_to_sand"), water_to_sand);
    glUniform1f(glGetUniformLocation(program, "far"), frustum_obj.far);
    glUniform1f(glGetUniformLocation(program, "width"), terrainWidth);

    // Render all chunks
    for (const auto &pair : chunks)
    {

        // Calculate the chunk position
        float chunkX = pair.first.first * (terrainWidth - 2);
        float chunkZ = pair.first.second * (terrainHeight - 2);

        if (frustum_obj.side_culling(vec3(chunkX, 0, chunkZ), terrainWidth * 2, world2view))
        {
            continue;
        }

        // Pass the chunk position to the shader
        glUniform3f(glGetUniformLocation(program, "chunkPosition"), chunkX, 0, chunkZ);


        // Draw the chunk
        DrawModel(pair.second, program, "in_Position", "in_Normal", NULL);
    }
}

Model *TerrainMap::GeneratePerlinTerrain(int offsetX, int offsetZ)
{
    // Allocate memory for the vertex, normal, texture coordinate, and index arrays
    vec3 *vertexArray;
    vec3 *normalArray;
    vec2 *texCoordArray;
    GLuint *indexArray;

    // Allocate memory for the vertex, normal, texture coordinate, and index arrays
    vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
    indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

// Generate the vertices, normals, and texture coordinates using Perlin noise
#pragma omp parallel for collapse(2)
    for (int x = 0; x < terrainWidth; x++)
        for (int z = 0; z < terrainHeight; z++)
        {
            // Calculate the Perlin noise value for the current vertex

            float perlin_noise = noise->fractal(4, (x + offsetX) * frequency, (z + offsetZ) * frequency) * amplitude;
            if (perlin_noise < water)
            {
                perlin_noise = water;
            }

            //  Set the vertex position, normal, and texture coordinate
            vertexArray[x + z * terrainWidth] = vec3(x, perlin_noise, z);
            texCoordArray[x + z * terrainWidth] = vec2(x + offsetX, z + offsetZ);
        }
// Calculate the normals for the vertices
#pragma omp parallel for collapse(2)
    for (int x = 0; x < terrainWidth; x++)
    {
        for (int z = 0; z < terrainHeight; z++)
        {
            // If the vertex is not on the edge of the terrain, calculate the normal
            if ((x != 0) && (x != (terrainWidth - 1)) && (z != 0) && (z != (terrainHeight - 1)))
            {
                vec3 left = vertexArray[(x - 1 + z * terrainWidth)];
                vec3 right = vertexArray[(x + 1 + z * terrainWidth)];
                vec3 up = vertexArray[(x + (z - 1) * terrainWidth)];
                vec3 down = vertexArray[(x + (z + 1) * terrainWidth)];

                vec3 normal = normalize(CrossProduct(up - down, right - left));

                if (normal.y < 0)
                {
                    normal = -normal;
                }

                normalArray[(x + z * terrainWidth)] = normal;
            }
        }
    }

    // Copy the normals from the neighboring vertices for the edge vertices
    for (int x = 0; x < terrainWidth; x++)
    {
        normalArray[x] = normalArray[x + terrainWidth];                                                            // Top edge
        normalArray[x + (terrainHeight - 1) * terrainWidth] = normalArray[x + (terrainHeight - 2) * terrainWidth]; // Bottom edge
    }
    for (int z = 0; z < terrainHeight; z++)
    {
        normalArray[z * terrainWidth] = normalArray[1 + z * terrainWidth];                                       // Left edge
        normalArray[(terrainWidth - 1) + z * terrainWidth] = normalArray[(terrainWidth - 2) + z * terrainWidth]; // Right edge
    }
// Generate the triangle indices
#pragma omp parallel for collapse(2)
    for (int x = 0; x < terrainWidth - 1; x++)
    {
        for (int z = 0; z < terrainHeight - 1; z++)
        {
            // Calculate the index once and reuse it
            int index = (x + z * (terrainWidth - 1)) * 6;

            // Triangle 1
            indexArray[index + 0] = x + z * terrainWidth;
            indexArray[index + 1] = x + (z + 1) * terrainWidth;
            indexArray[index + 2] = x + 1 + z * terrainWidth;
            // Triangle 2
            indexArray[index + 3] = x + 1 + z * terrainWidth;
            indexArray[index + 4] = x + (z + 1) * terrainWidth;
            indexArray[index + 5] = x + 1 + (z + 1) * terrainWidth;
        }
    }

    // Load the data into a model and return it
    Model *model = LoadDataToModel(
        vertexArray,
        normalArray,
        texCoordArray,
        NULL,
        indexArray,
        vertexCount,
        triangleCount * 3);


    return model;
}

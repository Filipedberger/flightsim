#include "terrain_map.h"
#include "iostream"
#include <chrono>
#include <omp.h>
#include "frustum.h"
#include <jsoncpp/json/json.h>

TerrainMap::TerrainMap(Json::Value settings, vec3 cameraPosition, const Frustum &f)
{
    cameraChunkX = cameraPosition.x / terrainWidth - 2;
    cameraChunkZ = cameraPosition.z / terrainHeight - 2;

    frustum_obj = f;

    // Calculate the number of vertices and triangles
    vertexCount = terrainWidth * terrainHeight;
    triangleCount = (terrainWidth - 1) * (terrainHeight - 1) * 2;

    // Allocate memory for the vertex, normal, texture coordinate, and index arrays
    vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
    indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

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

    std::cout << "Snow Y " << snow << std::endl;
    std::cout << "Rock Y " << rock << std::endl;
    std::cout << "Grass Y " << grass << std::endl;
    std::cout << "Sand Y " << sand << std::endl;

    std::cout << "Snow: " << rock_size << " " << snow_inter << std::endl;
    std::cout << "Rock: " << grass_size << " " << rock_inter << std::endl;
    std::cout << "Grass: " << sand_size << " " << grass_inter << std::endl;
    std::cout << "Sand: " << water_to_sand << std::endl;

    // TESTING
    noise_test = new SimplexNoise(1.0, 1.0);
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

void TerrainMap::update(vec3 cameraPosition, const mat4 &world2view)
{
    float rad_sq = (CHUNKS * terrainWidth) * (CHUNKS * terrainWidth);
    cameraChunkX = cameraPosition.x / terrainWidth - 2;
    cameraChunkZ = cameraPosition.z / terrainHeight - 2;

    int count = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int x = cameraChunkX - CHUNKS; x <= cameraChunkX + CHUNKS; ++x)
    {
        float chunkX = ((x * (terrainWidth - 2)) - cameraPosition.x) * ((x * (terrainWidth - 2)) - cameraPosition.x);

        for (int z = cameraChunkZ - CHUNKS; z <= cameraChunkZ + CHUNKS; ++z)
        {
            float chunkZ = (z * (terrainHeight - 2) - cameraPosition.z) * (z * (terrainHeight - 2) - cameraPosition.z);
            if (chunkX + chunkZ > rad_sq)
            {
                continue;
            }
            // if (frustum_obj.side_culling(vec3(chunkX, 0, chunkZ), 500, world2view)){
            //     continue;
            // }
            if (chunks.find({x, z}) == chunks.end())
            {
                count++;
                chunks[{x, z}] = GeneratePerlinTerrain(x * (terrainWidth - 2), z * (terrainHeight - 2));
            }
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    if (count > 0)
    {
        // std::cout << "Chunks generated: " << count << std::endl;
        // std::cout << "Generate chunks: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
    }

    // std::cout << chunks.size() << std::endl;
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

    auto t3 = std::chrono::high_resolution_clock::now();
    // std::cout << "Generate chunks: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
    // std::cout << "Remove chunks: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << " ms" << std::endl;
}

void TerrainMap::display(const GLuint &program, const mat4 &world2view, const mat4 &projection)
{
    glUseProgram(terrain_program);

    glUniformMatrix4fv(glGetUniformLocation(terrain_program, "viewMatrix"), 1, GL_TRUE, world2view.m);
    glUniformMatrix4fv(glGetUniformLocation(terrain_program, "in_projectionMatrix"), 1, GL_TRUE, projection.m);

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
        glUniform3f(glGetUniformLocation(terrain_program, "chunkPosition"), chunkX, 0, chunkZ);
        glUniform1f(glGetUniformLocation(terrain_program, "snow"), snow);
        glUniform1f(glGetUniformLocation(terrain_program, "rock"), rock);
        glUniform1f(glGetUniformLocation(terrain_program, "grass"), grass);
        glUniform1f(glGetUniformLocation(terrain_program, "sand"), sand);

        glUniform1f(glGetUniformLocation(terrain_program, "snow_inter"), snow_inter);
        glUniform1f(glGetUniformLocation(terrain_program, "rock_inter"), rock_inter);
        glUniform1f(glGetUniformLocation(terrain_program, "grass_inter"), grass_inter);
        glUniform1f(glGetUniformLocation(terrain_program, "water_to_sand"), water_to_sand);

        // Draw the chunk
        DrawModel(pair.second, terrain_program, "in_Position", "in_Normal", "in_TexCoord");
    }
    glUseProgram(program);
}

Model *TerrainMap::GeneratePerlinTerrain(int offsetX, int offsetZ)
{
    auto t1 = std::chrono::high_resolution_clock::now();
// Generate the vertices, normals, and texture coordinates using Perlin noise
#pragma omp parallel for collapse(2)
    for (int x = 0; x < terrainWidth; x++)
        for (int z = 0; z < terrainHeight; z++)
        {
            // Calculate the Perlin noise value for the current vertex

            // float perlin_noise = SimplexNoise::noise((x + offsetX) * frequency, (z + offsetZ) * frequency, 4) * amplitude;
            float perlin_noise = noise_test->fractal(4, (x + offsetX) * frequency, (z + offsetZ) * frequency) * amplitude;
            if (perlin_noise < water)
            {
                perlin_noise = water;
            }

            // float perlin_noise = SimplexNoise::noise(x + offsetX, z + offsetZ, 4) * amplitude;
            //  Set the vertex position, normal, and texture coordinate
            vertexArray[x + z * terrainWidth] = vec3((x) / 1.0, perlin_noise, (z) / 1.0);
            texCoordArray[x + z * terrainWidth] = vec2(x + offsetX, z + offsetZ);
        }
    auto t2 = std::chrono::high_resolution_clock::now();
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

    auto t3 = std::chrono::high_resolution_clock::now();
    // Copy the normals from the neighboring vertices for the edge vertices
    for (int x = 0; x < terrainWidth; x++)
    {
        normalArray[x] = normalArray[x + terrainWidth];                                                            // Top edge
        normalArray[x + (terrainHeight - 1) * terrainWidth] = normalArray[x + (terrainHeight - 2) * terrainWidth]; // Bottom edge
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    for (int z = 0; z < terrainHeight; z++)
    {
        normalArray[z * terrainWidth] = normalArray[1 + z * terrainWidth];                                       // Left edge
        normalArray[(terrainWidth - 1) + z * terrainWidth] = normalArray[(terrainWidth - 2) + z * terrainWidth]; // Right edge
    }
    auto t5 = std::chrono::high_resolution_clock::now();
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
    auto t6 = std::chrono::high_resolution_clock::now();

    // Load the data into a model and return it
    Model *model = LoadDataToModel(
        vertexArray,
        normalArray,
        texCoordArray,
        NULL,
        indexArray,
        vertexCount,
        triangleCount * 3);
    auto t7 = std::chrono::high_resolution_clock::now();

    /*std::cout << "Perlin noise: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
    std::cout << "Calculate normals: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << " ms" << std::endl;
    std::cout << "Copy normals: " << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " ms" << std::endl;
    std::cout << "Copy normals: " << std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count() << " ms" << std::endl;
    std::cout << "Generate indices: " << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count() << " ms" << std::endl;
    std::cout << "Load data to model: " << std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count() << " ms" << std::endl;*/

    return model;
}
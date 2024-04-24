#include "terrain_map.h"
#include "iostream"

TerrainMap::TerrainMap(vec3 pos, float sc)
{
    for (int x = cameraChunkX - CHUNKS; x <= cameraChunkX + CHUNKS; ++x)
    {
        for (int z = cameraChunkZ - CHUNKS; z <= cameraChunkZ + CHUNKS; ++z)
        {
            if (chunks.find({x, z}) == chunks.end())
            {
                std::cout << "Generating terrain at " << x << ", " << z << std::endl;
                chunks[{x, z}] = GeneratePerlinTerrain(x * (terrainWidth - 2), z * (terrainHeight - 2));
            }
        }
    }
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

void TerrainMap::update(vec3 cameraPosition)
{
    cameraPos = cameraPosition;
    cameraChunkX = cameraPosition.x / terrainWidth - 2;
    cameraChunkZ = cameraPosition.z / terrainHeight - 2;

    // Iterate over all chunks
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

void TerrainMap::display()
{

    glUniform1i(glGetUniformLocation(terrain_program, "current_terrain"), 0);

    // Render all chunks
    for (const auto &pair : chunks)
    {
        // Calculate the chunk position
        float chunkX = pair.first.first * (terrainWidth - 2);
        float chunkZ = pair.first.second * (terrainHeight - 2);

        // Pass the chunk position to the shader
        glUniform3f(glGetUniformLocation(terrain_program, "chunkPosition"), chunkX, 0, chunkZ);

        // Draw the chunk
        DrawModel(pair.second, terrain_program, "inPosition", "inNormal", "inTexCoord");
    }
}

void TerrainMap::create_chunk(int offsetX, int offsetZ)
{
    if (chunks.find({offsetX, offsetZ}) == chunks.end())
    {
        chunks[{offsetX, offsetZ}] = GeneratePerlinTerrain(offsetX * (terrainWidth - 2), offsetZ * (terrainHeight - 2));
    }
}

Model *TerrainMap::GeneratePerlinTerrain(int offsetX, int offsetZ)
{
    std::cout << "Generating terrain at " << offsetX << ", " << offsetZ << std::endl;
    // Calculate the number of vertices and triangles
    int vertexCount = terrainWidth * terrainHeight;
    int triangleCount = (terrainWidth - 1) * (terrainHeight - 1) * 2;
    int x, z;

    // Allocate memory for the vertex, normal, texture coordinate, and index arrays
    vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

    // Generate the vertices, normals, and texture coordinates using Perlin noise
    for (x = 0; x < terrainWidth; x++)
        for (z = 0; z < terrainHeight; z++)
        {
            // Calculate the Perlin noise value for the current vertex
            float perlin_noise = 0.0f;
            float amplitude = 100.0f;
            float frequency = 1.0f / 200.0f;

            for (int i = 0; i < 4; ++i) // 4 octaves
            {
                perlin_noise += perlin.noise2D((x + offsetX) * frequency, (z + offsetZ) * frequency) * amplitude;
                frequency *= 2.0f; // Double the frequency each octave
                amplitude *= 0.5f; // Halve the amplitude each octave
            }

            // Set the vertex position, normal, and texture coordinate
            vertexArray[x + z * terrainWidth] = vec3((x) / 1.0, perlin_noise, (z) / 1.0);
            normalArray[x + z * terrainWidth] = vec3(0.0, 1.0, 0.0);
            texCoordArray[x + z * terrainWidth] = vec2(x + offsetX, z + offsetZ);
        }

    // Calculate the normals for the vertices
    for (x = 0; x < terrainWidth; x++)
    {
        for (z = 0; z < terrainHeight; z++)
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
    for (x = 0; x < terrainWidth; x++)
    {
        normalArray[x] = normalArray[x + terrainWidth];                                                            // Top edge
        normalArray[x + (terrainHeight - 1) * terrainWidth] = normalArray[x + (terrainHeight - 2) * terrainWidth]; // Bottom edge
    }
    for (z = 0; z < terrainHeight; z++)
    {
        normalArray[z * terrainWidth] = normalArray[1 + z * terrainWidth];                                       // Left edge
        normalArray[(terrainWidth - 1) + z * terrainWidth] = normalArray[(terrainWidth - 2) + z * terrainWidth]; // Right edge
    }
    // Generate the triangle indices
    for (x = 0; x < terrainWidth - 1; x++)
        for (z = 0; z < terrainHeight - 1; z++)
        {
            // Triangle 1
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 0] = x + z * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 1] = x + (z + 1) * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 2] = x + 1 + z * terrainWidth;
            // Triangle 2
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 3] = x + 1 + z * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 4] = x + (z + 1) * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 5] = x + 1 + (z + 1) * terrainWidth;
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
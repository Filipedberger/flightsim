#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "cmath"
#include "iostream"
#include <algorithm>
#include <map>
#include "PerlinNoise.hpp"

const siv::PerlinNoise::seed_type seed = 123456u;

const siv::PerlinNoise perlin{seed};
std::map<std::pair<int, int>, Model *> chunks;

vec3 cameraPosition = vec3(0.0f, 0.0f, 10.0f);
vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f);
vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
mat4 camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);

double theta = -M_PI / 2.0f;
double phi = 0;
int deltaX = -1;
int deltaY = -1;

// Function to handle mouse movement
void mouseMoved(int x, int y)
{
    // Warp the cursor back to the center of the window
    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    if (deltaX == -1 && deltaY == -1)
    {
        deltaX = 0;
        deltaY = 0;
        return;
    }

    // Calculate the delta from the center of the window
    int deltaX = x - glutGet(GLUT_WINDOW_WIDTH) / 2;
    int deltaY = y - glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // Update theta and phi based on the mouse movement
    // The 0.005f factor is a sensitivity adjustment - you can change this value to make the camera more or less sensitive to mouse movement
    theta += deltaX * 0.001f;
    phi -= deltaY * 0.001f;

    // Clamp phi to be between -pi/2 and pi/2 to prevent the camera from flipping upside down
    float epsilon = 0.01f; // Small value to prevent phi from reaching exactly 0 or pi/2
    phi = std::max(std::min(phi, M_PI / 2.0f - epsilon), epsilon - M_PI / 2.0f);

    // Calculate the direction vector
    vec3 direction(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));

    // Update the lookAtPoint based on the camera position and the direction
    lookAtPoint = cameraPosition + direction;
}

bool forwardMove = false;
bool backwardMove = false;
bool leftMove = false;
bool rightMove = false;
bool upMove = false;
bool downMove = false;

// Function to handle keyboard input for when a key is pressed
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        forwardMove = true;
        backwardMove = false;
    }
    if (key == 's')
    {
        backwardMove = true;
        forwardMove = false;
    }
    if (key == 'a')
    {
        leftMove = true;
        rightMove = false;
    }
    if (key == 'd')
    {
        rightMove = true;
        leftMove = false;
    }
    if (key == 'z')
    {
        upMove = false;
        downMove = true;
    }
    if (key == ' ')
    {
        downMove = false;
        upMove = true;
    }
    if (key == 27)
    {
        exit(0);
    }
}

// Function to handle keyboard input for when a key is released
void keyboardUp(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        forwardMove = false;
    }
    if (key == 's')
    {
        backwardMove = false;
    }
    if (key == 'a')
    {
        leftMove = false;
    }
    if (key == 'd')
    {
        rightMove = false;
    }
    if (key == 'z')
    {
        downMove = false;
    }
    if (key == ' ')
    {
        upMove = false;
    }
}

static float stepSize = 12.0f;

void moveCamera(float deltaTime)
{
    vec3 forwardDirection = normalize(lookAtPoint - cameraPosition) * stepSize * deltaTime;
    vec3 sideDirection = normalize(cross(forwardDirection, upVector)) * stepSize * deltaTime;

    if (forwardMove)
    {
        cameraPosition += forwardDirection;
        lookAtPoint += forwardDirection;
    }

    else if (backwardMove)
    {
        cameraPosition -= forwardDirection;
        lookAtPoint -= forwardDirection;
    }

    if (leftMove)
    {
        cameraPosition -= sideDirection;
        lookAtPoint -= sideDirection;
    }

    else if (rightMove)
    {
        cameraPosition += sideDirection;
        lookAtPoint += sideDirection;
    }

    if (upMove)
    {
        cameraPosition.y += stepSize * deltaTime;
        lookAtPoint.y += stepSize * deltaTime;
    }

    else if (downMove)
    {
        cameraPosition.y -= stepSize * deltaTime;
        lookAtPoint.y -= stepSize * deltaTime;
    }
}

// Global variables for the projection matrix, shader program, and terrain dimensions
mat4 projectionMatrix;
Model *m, *m2, *tm;
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain
mat4 octagonPos, spherePos;
int terrainWidth = 128;
int terrainHeight = 128;

// Function to generate a terrain model using Perlin noise
Model *GeneratePerlinTerrain(int offsetX, int offsetZ, int terrain_type)
{
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
            float amplitude = 50.0f;
            float frequency = 1.0f / 200.0f;
            if (terrain_type == 1)
            {
                amplitude = 20.0f;
                frequency = 1.0f / 10.0f;
            }

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

// Function to initialize OpenGL and load the shaders
void init(void)
{
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMoved);
    glutKeyboardUpFunc(keyboardUp);
    glutHideCursor();

    // Set the clear color and enable depth testing
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Set the projection matrix
    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);

    // Load the shaders
    program = loadShaders("terrain6.vert", "terrain6.frag");
    glUseProgram(program);

    // Set the projection matrix uniform in the shader
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    // Load the textures
    LoadTGATextureSimple("maskros512.tga", &tex1);
    LoadTGATextureSimple("fft-terrain.tga", &tex2);

    // Set the texture uniforms in the shader
    glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0
    glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 1
}

int prevTime = 0;
float angle = 0;
int CHUNKS = 8;
int terrain_types[2] = {0, 1};
int terrain_type = 0;
int terrain_i = 0;
// Function to render the scene
void display(void)
{
    // Calculate the chunk coordinates of the camera
    int cameraChunkX = cameraPosition.x / terrainWidth - 2;
    int cameraChunkZ = cameraPosition.z / terrainHeight - 2;

    // Generate new chunks around the camera
    for (int x = cameraChunkX - CHUNKS; x <= cameraChunkX + CHUNKS; ++x)
    {
        for (int z = cameraChunkZ - CHUNKS; z <= cameraChunkZ + CHUNKS; ++z)
        {
            if (chunks.find({x, z}) == chunks.end())
            {
                chunks[{x, z}] = GeneratePerlinTerrain(x * (terrainWidth - 2), z * (terrainHeight - 2), terrain_type);
                terrain_i++;
            }
        }
    }
    if (terrain_i % 8 == 4)
    {

        if (terrain_type == 0)
        {
            terrain_type = 0;
        }
        else
        {
            terrain_type = 0;
        }
        terrain_i = 0;
    }
    std::cout << "Terrain_type and terrain_i: " << terrain_type << terrain_i << std::endl;
    const int MAX_CHUNK_DISTANCE = 3 * CHUNKS;

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
    glUniform1i(glGetUniformLocation(program, "current_terrain"), 0);

    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 total, modelView, camMatrix;

    // Calculate the time since the last frame
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - prevTime) * 0.005f;
    prevTime = currentTime;

    // Move the camera
    moveCamera(deltaTime);

    // Set the shader program and calculate the camera matrix
    glUseProgram(program);
    camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);

    // Calculate the model-view matrix
    modelView = IdentityMatrix();
    total = camMatrix * modelView;
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

    // Bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);

    // Enable texturing
    glUniform1i(glGetUniformLocation(program, "useTexture"), 1);

    // Render all chunks
    for (const auto &pair : chunks)
    {
        // Calculate the chunk position
        float chunkX = pair.first.first * (terrainWidth - 2);
        float chunkZ = pair.first.second * (terrainHeight - 2);

        // Pass the chunk position to the shader
        glUniform3f(glGetUniformLocation(program, "chunkPosition"), chunkX, 0, chunkZ);

        // Draw the chunk
        DrawModel(pair.second, program, "inPosition", "inNormal", "inTexCoord");
    }

    // Swap the front and back buffers
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    // Initialize GLUT and create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1700, 1000);
    glutCreateWindow("TSBK07 Lab 4");

    // Set the display and timer callbacks
    glutDisplayFunc(display);
    glutRepeatingTimer(20);

    // Initialize OpenGL and enter the main loop
    init();
    glutMainLoop();

    return 0;
}
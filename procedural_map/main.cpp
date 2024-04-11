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

using namespace std;
using namespace siv;

const PerlinNoise::seed_type seed = 123456u;
const PerlinNoise perlin{seed};
map<pair<int, int>, Model *> chunks;

vec3 cameraPosition = vec3(0.0f, 0.0f, 10.0f);
vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f);
vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
mat4 camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);

double theta = -M_PI / 2.0f;
double phi = 0;
int deltaX = -1;
int deltaY = -1;

void mouseMoved(int x, int y)
{
    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    if (deltaX == -1 && deltaY == -1)
    {
        deltaX = 0;
        deltaY = 0;
        return;
    }
    deltaX = x - glutGet(GLUT_WINDOW_WIDTH) / 2;
    deltaY = y - glutGet(GLUT_WINDOW_HEIGHT) / 2;
    theta += deltaX * 0.001f;
    phi -= deltaY * 0.001f;
    float epsilon = 0.01f;
    phi = std::max(std::min(phi, M_PI / 2.0f - epsilon), epsilon - M_PI / 2.0f);
    vec3 direction(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));
    lookAtPoint = cameraPosition + direction;
}

bool forwardMove = false;
bool backwardMove = false;
bool leftMove = false;
bool rightMove = false;
bool upMove = false;
bool downMove = false;

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        forwardMove = true;
        backwardMove = false;
        break;
    case 's':
        backwardMove = true;
        forwardMove = false;
        break;
    case 'a':
        leftMove = true;
        rightMove = false;
        break;
    case 'd':
        rightMove = true;
        leftMove = false;
        break;
    case 'z':
        upMove = false;
        downMove = true;
        break;
    case ' ':
        downMove = false;
        upMove = true;
        break;
    case 27:
        exit(0);
        break;
    }
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        forwardMove = false;
        break;
    case 's':
        backwardMove = false;
        break;
    case 'a':
        leftMove = false;
        break;
    case 'd':
        rightMove = false;
        break;
    case 'z':
        downMove = false;
        break;
    case ' ':
        upMove = false;
        break;
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
int terrainWidth = 16; // Replace with your desired terrain width
int terrainHeight = 16;
Model *GeneratePerlinTerrain(int offsetX, int offsetZ)
{
    // Replace with your desired terrain height
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
            // Vertex array. You need to scale this properly
            float perlin_noise = 0.0f;
            float amplitude = 100.0f;
            float frequency = 1.0f / 50.0f;
            for (int i = 0; i < 4; ++i) // 4 octaves
            {
                perlin_noise += perlin.noise2D((x + offsetX) * frequency, (z + offsetZ) * frequency) * amplitude;
                frequency *= 2.0f;
                amplitude *= 0.5f;
            }
            printf("Perlin noise: %f\n", perlin_noise);
            vertexArray[x + z * terrainWidth] = vec3((x) / 1.0, perlin_noise, (z) / 1.0);
            normalArray[x + z * terrainWidth] = vec3(0.0, 1.0, 0.0);
            texCoordArray[x + z * terrainWidth] = vec2(x + offsetX, z + offsetZ);
        }

    // Calculate the normals for the vertices
    for (x = 0; x < terrainWidth; x++)
    {
        for (z = 0; z < terrainHeight; z++)
        {
            // Normal vectors. You need to calculate these.
            if ((x != 0) && (x != (terrainWidth - 1)) && (z != 0) && (z != (terrainHeight - 1)))
            {
                vec3 left = vertexArray[(x - 1 + z * terrainWidth)];
                vec3 right = vertexArray[(x + 1 + z * terrainWidth)];
                vec3 up = vertexArray[(x + (z - 1) * terrainWidth)];
                vec3 down = vertexArray[(x + (z + 1) * terrainWidth)];

                // If the vertex is at the edge of a chunk, look up the vertices in the neighboring chunks
                if (x == 0)
                    left = chunks[{offsetX - 1, offsetZ}]->vertexArray[(terrainWidth - 1 + z * terrainWidth)];
                if (x == terrainWidth - 1)
                    right = chunks[{offsetX + 1, offsetZ}]->vertexArray[(z * terrainWidth)];
                if (z == 0)
                    up = chunks[{offsetX, offsetZ - 1}]->vertexArray[(x + (terrainHeight - 1) * terrainWidth)];
                if (z == terrainHeight - 1)
                    down = chunks[{offsetX, offsetZ + 1}]->vertexArray[(x * terrainWidth)];

                vec3 normal = normalize(CrossProduct(up - down, right - left));

                if (normal.y < 0)
                {
                    normal = -normal;
                }

                normalArray[(x + z * terrainWidth)] = normal;
            }
            else
            {
                normalArray[(x + z * terrainWidth)] = vec3(0.0f, 1.0f, 0.0f);
            }
        }
    }

    for (x = 0; x < terrainWidth - 1; x++)
        for (z = 0; z < terrainHeight - 1; z++)
        {
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 0] = x + z * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 1] = x + (z + 1) * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 2] = x + 1 + z * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 3] = x + 1 + z * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 4] = x + (z + 1) * terrainWidth;
            indexArray[(x + z * (terrainWidth - 1)) * 6 + 5] = x + 1 + (z + 1) * terrainWidth;
        }
}

// End of terrain generation

// Create Model and upload to GPU:

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
void init(void)
{
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMoved);
    glutKeyboardUpFunc(keyboardUp);
    glutHideCursor();

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 400.0);

    // Load and compile shader
    program = loadShaders("terrain6.vert", "terrain6.frag");
    glUseProgram(program);
    printError("init shader");

    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    // Maskros
    glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0
    LoadTGATextureSimple("maskros512.tga", &tex1);
    LoadTGATextureSimple("fft-terrain.tga", &tex2);
    glUniform1i(glGetUniformLocation(program, "tex2"), 1);

    // Load terrain data

    // LoadTGATextureData("44-terrain.tga", &ttex);
    printError("init terrain");

    // printf("Note: The call to DrawModel will report warnings about inNormal not existing. This is because inNormal is not used in the shader yet so it is optimized away.\n");
}

int prevTime = 0;
float angle = 0;
int CHUNKS = 16;
void display(void)
{

    int cameraChunkX = cameraPosition.x / terrainWidth - 2;
    int cameraChunkZ = cameraPosition.z / terrainHeight - 2;
    for (int x = cameraChunkX - CHUNKS; x <= cameraChunkX + CHUNKS; ++x)
    {
        for (int z = cameraChunkZ - CHUNKS; z <= cameraChunkZ + CHUNKS; ++z)
        {
            if (chunks.find({x, z}) == chunks.end())
            {
                // Generate new chunk
                chunks[{x, z}] = GeneratePerlinTerrain(x * (terrainWidth - 2), z * (terrainHeight - 2));
            }
        }
    }
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 total, modelView, camMatrix;

    printError("pre display");

    // Handle movement
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calculate the time since the last frame and convert to seconds
    float deltaTime = (currentTime - prevTime) * 0.01f;
    prevTime = currentTime;
    moveCamera(deltaTime);

    glUseProgram(program);
    camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);

    modelView = IdentityMatrix();
    total = camMatrix * modelView;
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2); // Bind Our Texture tex2

    // We should use a texture when drawing the terrain
    glUniform1i(glGetUniformLocation(program, "useTexture"), 1);

    // Render all chunks
    for (const auto &pair : chunks)
    { // Calculate chunk position
      // Calculate chunk position
        float chunkX = pair.first.first * (terrainWidth - 2);
        float chunkZ = pair.first.second * (terrainHeight - 2);

        // Pass chunk position to shader
        glUniform3f(glGetUniformLocation(program, "chunkPosition"), chunkX, 0, chunkZ);

        DrawModel(pair.second, program, "inPosition", "inNormal", "inTexCoord");
    }
    printError("display 2");

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

    // glutPassiveMotionFunc(mouse);

    glutMainLoop();

    return 0;
}

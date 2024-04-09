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

mat4 projectionMatrix;
GLuint program;
GLuint tex1, tex2;
TextureData ttex;

mat4 octagonPos, spherePos;
int terrainWidth = 8;
int terrainHeight = 8;

Model *GeneratePerlinTerrain(int offsetX, int offsetZ)
{
    int vertexCount = terrainWidth * terrainHeight;
    int triangleCount = (terrainWidth - 1) * (terrainHeight - 1) * 2;
    int x, z;

    vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
    vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

    for (x = 0; x < terrainWidth; x++)
        for (z = 0; z < terrainHeight; z++)
        {
            float perlin_noise = 0.0f;
            float amplitude = 200.0f;
            float frequency = 1.0f / 200.0f;
            for (int i = 0; i < 4; ++i)
            {
                perlin_noise += perlin.noise2D((x + offsetX) * frequency, (z + offsetZ) * frequency) * amplitude;
                frequency *= 2.0f;
                amplitude *= 0.5f;
            }
            vertexArray[x + z * terrainWidth] = vec3((x) / 1.0, perlin_noise, (z) / 1.0);
            normalArray[x + z * terrainWidth] = vec3(0.0, 1.0, 0.0);
            texCoordArray[x + z * terrainWidth] = vec2(x + offsetX, z + offsetZ);
        }
    for (x = 0; x < terrainWidth; x++)
    {
        for (z = 0; z < terrainHeight; z++)
        {
            if ((x != 0) && (x != (terrainWidth - 1)) && (z != 0) && (z != (terrainHeight - 1)))
            {
                vec3 left = vertexArray[(x - 1 + z * terrainWidth)];
                vec3 right = vertexArray[(x + 1 + z * terrainWidth)];
                vec3 up = vertexArray[(x + (z - 1) * terrainWidth)];
                vec3 down = vertexArray[(x + (z + 1) * terrainWidth)];

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
    {
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
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
    program = loadShaders("terrain6.vert", "terrain6.frag");
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniform1i(glGetUniformLocation(program, "tex1"), 0);
    LoadTGATextureSimple("maskros512.tga", &tex1);
    LoadTGATextureSimple("fft-terrain.tga", &tex2);
    glUniform1i(glGetUniformLocation(program, "tex2"), 1);
}

int prevTime = 0;
float angle = 0;
int CHUNKS = 64;

void display(void)
{
    // Calculate the current chunk coordinates of the camera
    int cameraChunkX = static_cast<int>(cameraPosition.x / (terrainWidth - 2));
    int cameraChunkZ = static_cast<int>(cameraPosition.z / (terrainHeight - 2));

    // Calculate the range of chunks to load based on the camera's position
    int loadChunksXMin = cameraChunkX - CHUNKS;
    int loadChunksXMax = cameraChunkX + CHUNKS;
    int loadChunksZMin = cameraChunkZ - CHUNKS;
    int loadChunksZMax = cameraChunkZ + CHUNKS;

    // Load chunks within the calculated range
    for (int x = loadChunksXMin; x <= loadChunksXMax; ++x)
    {
        for (int z = loadChunksZMin; z <= loadChunksZMax; ++z)
        {
            if (chunks.find({x, z}) == chunks.end())
            {
                chunks[{x, z}] = GeneratePerlinTerrain(x * (terrainWidth - 2), z * (terrainHeight - 2));
            }
        }
    }

    // Offload chunks that are too far from the camera
    auto it = chunks.begin();
    while (it != chunks.end())
    {
        int chunkX = it->first.first;
        int chunkZ = it->first.second;

        // Calculate the distance between the chunk and the camera
        float distanceX = abs(cameraChunkX - chunkX);
        float distanceZ = abs(cameraChunkZ - chunkZ);
        float distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

        // If the chunk is beyond a certain distance, remove it from the map
        if (distance > CHUNKS)
        {
            delete it->second;
            it = chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Render the scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 total, modelView, camMatrix;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
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
    glBindTexture(GL_TEXTURE_2D, tex2);
    glUniform1i(glGetUniformLocation(program, "useTexture"), 1);
    for (const auto &pair : chunks)
    {
        float chunkX = pair.first.first * (terrainWidth - 2);
        float chunkZ = pair.first.second * (terrainHeight - 2);
        glUniform3f(glGetUniformLocation(program, "chunkPosition"), chunkX, 0, chunkZ);
        DrawModel(pair.second, program, "inPosition", "inNormal", "inTexCoord");
    }
    glutSwapBuffers();
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1700, 1000);
    glutCreateWindow("TSBK07 Lab 4");
    glutDisplayFunc(display);
    init();
    glutRepeatingTimer(20);
    glutMainLoop();
    exit(0);
}

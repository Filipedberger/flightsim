#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform vec3 chunkPosition; // New uniform for the chunk position

out vec2 texCoord;
out vec3 world_normal;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform int current_terrain;
out vec3 surfPos;

void main(void)
{
    mat3 normalMatrix1 = mat3(mdlMatrix);
    world_normal = inNormal;
    texCoord = inTexCoord;
    vec3 worldPosition = inPosition + chunkPosition; // Add the chunk position to the vertex position
    surfPos = worldPosition;
    gl_Position = projMatrix * mdlMatrix * vec4(worldPosition, 1.0);
}
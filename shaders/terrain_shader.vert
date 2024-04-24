#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

mat4 mdlMatrix;
uniform mat4 viewMatrix;
uniform mat4 in_projectionMatrix;

uniform vec3 chunkPosition; // New uniform for the chunk position

out vec3 world_normal;
out vec3 surfPos;


void main(void)
{
    world_normal = in_Normal;
    vec3 worldPosition = in_Position + chunkPosition; // Add the chunk position to the vertex position
    surfPos = worldPosition;
    gl_Position = in_projectionMatrix * viewMatrix * vec4(worldPosition, 1.0);  
}
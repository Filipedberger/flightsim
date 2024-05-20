#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

mat4 mdlMatrix;
uniform mat4 viewMatrix;
uniform mat4 in_projectionMatrix;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;

out vec4 world_position;
out vec3 normal_world; 

uniform int map;

uniform vec3 chunkPosition;


void main(void)
{
	if (map != 1) {
	mdlMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	gl_Position = in_projectionMatrix * viewMatrix * mdlMatrix * vec4(in_Position, 1.0);

	normal_world = vec3(mdlMatrix * vec4(in_Normal, 0.0));

	mat3 normalMatrix = transpose(inverse(mat3(mdlMatrix)));

	world_position = mdlMatrix * vec4(in_Position, 1.0);
	return;
	}

    world_position = vec4(in_Position + chunkPosition, 1.0); // Add the chunk position to the vertex position
    gl_Position = in_projectionMatrix * viewMatrix * world_position; 
	normal_world = in_Normal;
}

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

out vec3 x_zero;

uniform vec3 tmp;
uniform int tmp2;


void main(void)
{
	if (map == 0) {
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

	if (in_Position.x < 4 || in_Position.z < 4 ) {
		x_zero = vec3(0.0, 0.0, 0.0);
	} else {
		x_zero = vec3(0.0, 0.0, 0.0);
	}


	if (chunkPosition.x == 0.0 && chunkPosition.z == 0.0) {
		x_zero = vec3(0.6, 1.0, 1.0);
	}

	if (in_Position.x < 6 && in_Position.x > 3 && in_Position.z < 6 && in_Position.z > 3) {
		x_zero = vec3(0.6, 0.5, 0.5);
	}



	if (in_Position.x < tmp.x + 3.0 && in_Position.x > tmp.x - 3.0 && in_Position.z < tmp.z + 3.0 && in_Position.z > tmp.z - 3.0 && tmp2 > 0) {
		x_zero = vec3(1.0, 0, 0);
	}




}

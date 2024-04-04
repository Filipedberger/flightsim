#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 world_normal;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

out vec3 surfPos;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	world_normal = inNormal;
	texCoord = inTexCoord;
	//surfPos = vec3(mdlMatrix * vec4(inPosition, 1.0));
	surfPos = inPosition;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}

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


out vec3 normal;

void main(void)
{
	//gl_Position = mdlMatrix * vec4(in_Position, 1.0);
	mdlMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	gl_Position = in_projectionMatrix * viewMatrix* mdlMatrix * vec4(in_Position, 1.0);
	normal = in_Normal;
}

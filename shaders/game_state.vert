#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 normal;

mat4 mdlMatrix;
uniform mat4 viewMatrix;
uniform mat4 in_projectionMatrix;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;

out vec4 view_position;

out vec3 normal;

void main(void)
{
	mdlMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	gl_Position = in_projectionMatrix * viewMatrix* mdlMatrix * vec4(in_Position, 1.0);
	normal = in_Normal;
	view_position = viewMatrix* mdlMatrix * vec4(in_Position, 1.0);
}

#version 150

in  vec3 in_Position;
in  vec3 in_color;
out vec3 color;
uniform mat4 in_translation;
uniform mat4 in_scale;
uniform mat4 in_rotationZ;
uniform mat4 in_rotationY;
uniform mat4 in_rotationX;

void main(void)
{
	gl_Position = in_rotationZ * in_rotationY * in_rotationX * vec4(in_Position, 1.0);
	color = in_color;
}

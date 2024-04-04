#version 150

in  vec3 in_Position;
in  vec3 in_color;
out vec3 color;
uniform mat4 in_translation;
uniform mat4 in_scale;
uniform mat4 in_rotation;

void main(void)
{
	//gl_Position = in_scale * in_translation * in_rotation * vec4(in_Position, 1.0);
	gl_Position = in_rotation * vec4(in_Position, 1.0);
	color = in_color;
}

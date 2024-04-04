#version 150

out vec4 out_Color;
in vec3 color;
in vec3 Normal;

void main(void)
{
	out_Color = vec4(abs(Normal), 1.0);
}

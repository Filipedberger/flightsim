#version 150

out vec4 out_Color;
in vec3 color;

void main(void)
{
	out_Color = vec4(color, 1.0);
}

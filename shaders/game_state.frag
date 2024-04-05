#version 150

out vec4 out_Color;
in vec3 normal;

void main(void)
{
	out_Color = vec4(abs(normal), 1.0);
}

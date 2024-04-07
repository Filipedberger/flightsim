#version 150

out vec4 out_Color;
in vec3 normal;

void main(void)
{
	if (normal.y == 1.0) {
		out_Color = vec4(0.2, 0.8, 0.2, 1.0);
	}
	else {
		float a = abs(normal.z);
		out_Color = vec4(a,a,a, 1.0);
	}
}

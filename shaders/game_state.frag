#version 150

out vec4 out_Color;
in vec3 normal;
in vec4 view_position;
in vec4 world_position;

void main(void)
{	
	if (world_position.y < 0.0) {
		//grass
		out_Color = vec4(0.2, 0.8, 0.2, 1.0);
	}
	else {
		float a = abs(normal.z);
		out_Color = vec4(a,a,a, 1.0);
	}
}

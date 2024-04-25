#version 150

out vec4 out_Color;
in vec3 normal;
in vec4 view_position;
in vec4 world_position;


// Init in state.cpp  State::upload2shader() //
uniform vec3 light_intensity;
uniform vec3 light_direction;

void main(void)
{	
	float NdotL = max(dot(normalize(normal), normalize(light_direction)), 0.0); 

	if (world_position.y < 0.0) {
		//grass
		//out_Color = vec4(0.2, 0.8, 0.2, 1.0);
		out_Color = vec4(0.2 * light_intensity.x * NdotL, 0.8 * light_intensity.y * NdotL, 0.2 * light_intensity.z * NdotL, 1.0);
	}
	else {
		//float a = abs(normal.z);
		float a = light_intensity.x * NdotL;
		out_Color = vec4(a,a,a, 1.0);
	}
}
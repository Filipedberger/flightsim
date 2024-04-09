#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 world_normal;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform mat4 projMatrix;
uniform int useTexture;

const vec3 lightInit = vec3(0.58, 0.99, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

in vec3 surfPos;

void main(void)
{
	float intensity = max(dot(normalize(world_normal), lightInit), 0.0);
	if (useTexture == 1){
		vec3 blue = vec3(0.0, 0.0, 1.0);
		float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

		if (surfPos.y > 35) {
			outColor = vec4(1.0,1.0,1.0, 1.0);
			//outColor = vec4(0.01, 0.01, 0.01, 1.0);
		}
		else if (surfPos.y < 10) {
			outColor = vec4(0.25, 0.25, 0.25, 1.0);
		}
		else {
			outColor = vec4(0.5, 0.5, 0.5, 1.0);
			//outColor = texture(tex1, texCoord);
		}
		outColor = outColor*intensity;
	}
	else {
		
		outColor = vec4(base_color * intensity, 1.0);
	}

}

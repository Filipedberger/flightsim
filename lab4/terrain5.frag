#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 world_normal;
uniform sampler2D tex1;
uniform sampler2D tex2;

const vec3 lightInit = vec3(0.58, 0.58, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

uniform int useTexture;

in vec3 surfPos;

void main(void)
{	
	if (useTexture == 1){
		float intensity = max(dot(normalize(world_normal), lightInit), 0.0);
		if (surfPos.y > 20) {
			outColor = texture(tex2, texCoord);
		}
		else {
			outColor = texture(tex1, texCoord);
		}
	}
	else {
		float intensity = max(dot(normalize(world_normal), lightInit), 0.0);
		outColor = vec4(base_color * intensity, 1.0);
	}
}

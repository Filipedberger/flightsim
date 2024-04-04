#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 world_normal;
uniform sampler2D tex;

const vec3 lightInit = vec3(0.58, 0.58, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

void main(void)
{
	//outColor = texture(tex, texCoord);
	float intensity = max(dot(normalize(world_normal), lightInit), 0.0);
	outColor = vec4(base_color * intensity, 1.0);
	//outColor = vec4(normalize(world_normal), 1.0);
}

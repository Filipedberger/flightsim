#version 150

out vec4 out_Color;
in vec3 color;
in vec3 Normal;
in vec3 light;

void main(void)
{	
	vec3 normal = normalize(Normal);
    float intensity = max(dot(normal, light), 0.0);
    vec3 gray = vec3(0.9); // Gray color
	out_Color = vec4(gray * intensity, 1.0);
}

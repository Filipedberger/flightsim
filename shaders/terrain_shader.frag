#version 150

out vec4 outColor;
in vec3 world_normal;

vec3 ambientLight = vec3(0.1, 0.1, 0.1); // Ambient light color

const vec3 lightInit = vec3(0.58, 0.99, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

in vec3 surfPos;


void main(void)
{
    vec3 blue = vec3(0.0, 0.0, 1.0);
    float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

    if (surfPos.y > 30) {
        outColor = vec4(0.9,0.9,0.9, 1.0);
    }
    else if (surfPos.y < 0 && surfPos.y > -70) {
        outColor = vec4(0.0, 0.1, 0.0, 1.0);
    }
    else if(surfPos.y < -70)
    {
        outColor = vec4(0.0, 0.0, 0.5, 1.0);
    }
    else {
        outColor = vec4(0.3, 0.3, 0.3, 1.0);
    }
    outColor = outColor*intensity + vec4(ambientLight, 1.0);
}
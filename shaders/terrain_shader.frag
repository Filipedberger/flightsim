#version 150

out vec4 outColor;
in vec3 world_normal;

uniform vec3 cameraPos;
uniform float far;
uniform float width;

vec3 ambientLight = vec3(0.1, 0.1, 0.1); // Ambient light color

const vec3 lightInit = vec3(0.58, 0.99, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

in vec3 surfPos;


void main(void)
{
    vec3 temp_color;
    vec3 blue = vec3(0.0, 0.0, 1.0);
    float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

    if (surfPos.y > 30) {
        temp_color = vec3(0.9,0.9,0.9);
    }
    else if (surfPos.y < 0 && surfPos.y > -70) {
        temp_color = vec3(0.0, 0.1, 0.0);
    }
    else if(surfPos.y < -70)
    {
        temp_color = vec3(0.0, 0.0, 0.5);
    }
    else {
        temp_color = vec3(0.3, 0.3, 0.3);
    }

    float d = sqrt((cameraPos.x - surfPos.x) * (cameraPos.x - surfPos.x) + (cameraPos.z - surfPos.z) * (cameraPos.z - surfPos.z));

    float alpha;

    float c = far - width * 4;

    if (d > far) {
        alpha = 0;
    } 
    else if (d < c) {
        alpha = 1;
    } 
    else {
        alpha = (far - d) / (far - c);
    }

    outColor = vec4(temp_color*intensity + ambientLight, alpha);
}
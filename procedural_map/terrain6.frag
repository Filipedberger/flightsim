#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 world_normal;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform mat4 projMatrix;
uniform int useTexture;

uniform int current_terrain;

vec3 ambientLight = vec3(0.1, 0.1, 0.1); // Ambient light color

const vec3 lightInit = vec3(0.58, 0.99, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

in vec3 surfPos;

void main(void)
{
    float intensity = max(dot(normalize(world_normal), lightInit), 0.0);
    if (current_terrain == 0){
        vec3 blue = vec3(0.0, 0.0, 1.0);
        float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

        if (surfPos.y > 35) {
            outColor = vec4(1.0,1.0,1.0, 1.0);
        }
        else if (surfPos.y < 10) {
            outColor = vec4(0.25, 0.25, 0.25, 1.0);
        }
        else {
            outColor = vec4(0.5, 0.5, 0.5, 1.0);
        }
        outColor = outColor*intensity + vec4(ambientLight, 1.0);
    }
    else {
        
        float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

        if (surfPos.y > 10) {
            outColor = vec4(0.76, 0.70, 0.5, 1.0);
        }
        else if (surfPos.y < -40) {
            
            outColor = vec4(0.20,0.20,0.2, 1.0);
        }
        else {
            outColor = vec4(0.70, 0.64, 0.44, 1.0);
        }
        outColor = outColor*intensity + vec4(ambientLight, 1.0);
    }
}

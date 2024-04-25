#version 150

out vec4 outColor;
in vec3 world_normal;

uniform float snow;
uniform float rock;
uniform float grass;
uniform float sand;

uniform float snow_inter;
uniform float rock_inter;
uniform float grass_inter;
uniform float water_to_sand;

vec3 ambientLight = vec3(0.1, 0.1, 0.1); // Ambient light color

const vec3 lightInit = vec3(0.58, 0.99, 0.58); // Pos of light source
vec3 base_color = vec3(0.8);

in vec3 surfPos;


void main(void)
{
    vec4 green = vec4(0.0, 0.3, 0.0, 1.0);
    vec4 grey = vec4(0.3, 0.3, 0.3, 1.0);
    vec4 white = vec4(0.9,0.9,0.9, 1.0);
    vec4 beige = vec4(1.0, 0.96, 0.876, 1.0);
    vec4 blue = vec4(0.0, 0.0, 0.5, 1.0);

    float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

    if (surfPos.y > snow) {
        outColor = white;
    }
    else if(surfPos.y > snow - snow_inter){
        float t = (surfPos.y - (snow - snow_inter))/snow_inter;
        outColor = t*white + (1 - t)*grey;
    }
    else if(surfPos.y > rock){
        outColor = grey;
    }
    else if (surfPos.y > rock - rock_inter) {
        float t = (surfPos.y - (rock - rock_inter))/rock_inter;
        outColor = t*grey + (1 - t)*green;
    }
    else if (surfPos.y > grass)
    {
        outColor = green;
    }
    else if(surfPos.y > grass - grass_inter){
        float t = (surfPos.y - (grass-grass_inter))/grass_inter;
        outColor = t*green + (1 - t)*beige;
    }
    else if(surfPos.y > sand + water_to_sand)
    {
        outColor = beige;
    }
    else{
        outColor = blue;
    }
    outColor = outColor*intensity + vec4(ambientLight, 1.0);
}
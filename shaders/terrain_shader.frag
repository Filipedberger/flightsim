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
    vec3 green = vec3(0.0, 0.3, 0.0);
    vec3 grey = vec3(0.3, 0.3, 0.3);
    vec3 white = vec3(0.9,0.9,0.9);
    vec3 beige = vec3(1.0, 0.96, 0.876);
    vec3 blue = vec3(0.0, 0.0, 0.5);

    float intensity = max(dot(normalize(world_normal), lightInit), 0.0);

    if (surfPos.y > snow) {
        temp_color = white;
    }
    else if(surfPos.y > snow - snow_inter){
        float t = (surfPos.y - (snow - snow_inter))/snow_inter;
        temp_color = t*white + (1 - t)*grey;
    }
    else if(surfPos.y > rock){
        temp_color = grey;
    }
    else if (surfPos.y > rock - rock_inter) {
        float t = (surfPos.y - (rock - rock_inter))/rock_inter;
        temp_color = t*grey + (1 - t)*green;
    }
    else if (surfPos.y > grass)
    {
        temp_color = green;
    }
    else if(surfPos.y > grass - grass_inter){
        float t = (surfPos.y - (grass-grass_inter))/grass_inter;
        temp_color = t*green + (1 - t)*beige;
    }
    else if(surfPos.y > sand + water_to_sand)
    {
        temp_color = beige;
    }
    else{
        temp_color = blue;
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
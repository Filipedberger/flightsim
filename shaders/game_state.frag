#version 150

out vec4 out_Color;
in vec3 normal_world;
in vec4 world_position;

uniform mat4 viewMatrix;


// Init in state.cpp  State::upload2shader() //
uniform vec3 light_intensity;
uniform vec3 light_pos;
uniform vec3 camera_pos;

uniform int map;

// Map Variables

uniform float snow;
uniform float rock;
uniform float grass;
uniform float sand;

uniform float snow_inter;
uniform float rock_inter;
uniform float grass_inter;
uniform float water_to_sand;

uniform float far;
uniform float width;


void main(void)
{	

	float specularExponent = 25;

	vec4 color = vec4(0, 0, 0, 0);
	mat3 world2view_3 = mat3(viewMatrix);
	vec3 normal_view = normalize(world2view_3 * normal_world);

	// Directional light
	vec3 light_direction = normalize(world2view_3 * light_pos);

	// Diffuse light
	float diffuse = max(dot(normal_view, light_direction), 0.0);
	vec3 diffuseLight = diffuse * light_intensity;

	// Specular light
	vec3 viewDirection = normalize(world2view_3 * (camera_pos - vec3(world_position)));
	vec3 reflectDirection = reflect(-light_direction, normal_view);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
	vec3 specularLight = specular * light_intensity;


	if (map == 0) {
		
		vec3 temp_color = vec3(0.53, 0.81, 0.94); // Baby blue
		//vec3 temp_color = vec3(1.0, 1.0, 1.0); // White


		color =  vec4(temp_color * 0.4 + diffuseLight * 0.4 + specularLight * 0.4, 1);

		out_Color = color;
		return;
	}

	vec3 temp_color;
    vec3 green = vec3(0.0, 0.3, 0.0);
    vec3 grey = vec3(0.3, 0.3, 0.3);
    vec3 white = vec3(0.9,0.9,0.9);
    vec3 beige = vec3(1.0, 0.96, 0.876);
    vec3 blue = vec3(0.0, 0.0, 0.5);

	float spec = 0.0; //TEST

    if (world_position.y > snow) {
        temp_color = white;
		spec = 0.5;
    }
    else if(world_position.y > snow - snow_inter){
        float t = (world_position.y - (snow - snow_inter))/snow_inter;
        temp_color = t*white + (1 - t)*grey;
    }
    else if(world_position.y > rock){
        temp_color = grey;
    }
    else if (world_position.y > rock - rock_inter) {
        float t = (world_position.y - (rock - rock_inter))/rock_inter;
        temp_color = t*grey + (1 - t)*green;
    }
    else if (world_position.y > grass)
    {
        temp_color = green;
    }
    else if(world_position.y > grass - grass_inter){
        float t = (world_position.y - (grass-grass_inter))/grass_inter;
        temp_color = t*green + (1 - t)*beige;
    }
    else if(world_position.y > sand + water_to_sand)
    {
        temp_color = beige;
    }
    else{
        temp_color = blue;
    }

    float d = sqrt((camera_pos.x - world_position.x) * (camera_pos.x - world_position.x) + (camera_pos.z - world_position.z) * (camera_pos.z - world_position.z));

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

	//vec3 ambientLight = vec3(0.1, 0.1, 0.1); // Ambient light color

	//float intensity = max(dot(normalize(normal_world), light_pos), 0.0);

	//out_Color = vec4(temp_color*intensity + ambientLight, alpha);

	out_Color = vec4(temp_color * 0.7 + diffuseLight * 0.3 + specularLight * spec, alpha);

}
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

uniform vec3 plane_light_pos[2];
uniform vec3 plane_light_intensity;
uniform int plane_light_radius;

void main(void)
{	

	float specularExponent = 50;

	mat3 world2view_3 = mat3(viewMatrix);
	vec3 normal_view = normalize(world2view_3 * normal_world);

	// Directional light
	vec3 light_direction = normalize(world2view_3 * light_pos);

	// Diffuse light
	float diffuse = max(dot(normal_view, light_direction), 0.0);

	// Specular light
	vec3 viewDirection = normalize(world2view_3 * (camera_pos - vec3(world_position)));
	vec3 reflectDirection = reflect(-light_direction, normal_view);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);


	if (map == 0) {
		float d1 = length(vec3(world_position) - plane_light_pos[0]);
		float d2 = length(vec3(world_position) - plane_light_pos[1]);

		vec3 temp_intensity = plane_light_intensity;

		if (light_intensity.x > 0.5) {
			temp_intensity = vec3(0.0, 0.0, 0.0);
		}


		
		float light_intensity1 = 1.0;
		float light_intensity2 = 1.0;


		if (d1 > plane_light_radius) {
			light_intensity1 = 0.0;
		}
		else {
			light_intensity1 = 1.0 - d1 / plane_light_radius;
		}

		if (d2 > plane_light_radius) {
			light_intensity2 = 0.0;
		}
		else {
			light_intensity2 = 1.0 - d2 / plane_light_radius;
		}

		vec3 positional_light2 = normalize(world2view_3 * (plane_light_pos[0] - vec3(world_position)));
		float diffuse2 = max(dot(normal_view, positional_light2), 0.0);
		
		vec3 positional_light3 = normalize(world2view_3 * (plane_light_pos[1] - vec3(world_position)));
		float diffuse3 = max(dot(normal_view, positional_light3), 0.0);

		//vec3 color = vec3(0.53, 0.81, 0.94); // Baby blue
		vec3 color = vec3(1.0, 1.0, 1.0); // White
		//vec4 color = vec4(0, 0, 0, 0);

		vec3 tmp_color = color * (light_intensity * (0.4 + diffuse * 0.4 + specular * 0.4) + temp_intensity * (diffuse2 * light_intensity1 + diffuse3 * light_intensity2));
		out_Color =  vec4(tmp_color, 1);

	}
	else if (map == 1) {

		vec3 color;
		vec3 green = vec3(0.0, 0.3, 0.0);
		vec3 grey = vec3(0.3, 0.3, 0.3);
		vec3 white = vec3(0.9,0.9,0.9);
		vec3 beige = vec3(1.0, 0.96, 0.876);
		vec3 blue = vec3(0.0, 0.0, 0.5);

		float spec = 0.0; //TEST

		if (world_position.y > snow) {
			color = white;
			spec = 0.5;
		}
		else if(world_position.y > snow - snow_inter){
			float t = (world_position.y - (snow - snow_inter))/snow_inter;
			color = t*white + (1 - t)*grey;
		}
		else if(world_position.y > rock){
			color = grey;
		}
		else if (world_position.y > rock - rock_inter) {
			float t = (world_position.y - (rock - rock_inter))/rock_inter;
			color = t*grey + (1 - t)*green;
		}
		else if (world_position.y > grass)
		{
			color = green;
		}
		else if(world_position.y > grass - grass_inter){
			float t = (world_position.y - (grass-grass_inter))/grass_inter;
			color = t*green + (1 - t)*beige;
		}
		else if(world_position.y > sand + water_to_sand)
		{
			color = beige;
		}
		else{
			color = blue;
			spec = 1.0;
		}
		if (world_position.y > snow) {
			color = white;
			spec = 0.5;
		}
		else if(world_position.y > snow - snow_inter){
			float t = (world_position.y - (snow - snow_inter))/snow_inter;
			color = t*white + (1 - t)*grey;
		}
		else if(world_position.y > rock){
			color = grey;
		}
		else if (world_position.y > rock - rock_inter) {
			float t = (world_position.y - (rock - rock_inter))/rock_inter;
			color = t*grey + (1 - t)*green;
		}
		else if (world_position.y > grass)
		{
			color = green;
		}
		else if(world_position.y > grass - grass_inter){
			float t = (world_position.y - (grass-grass_inter))/grass_inter;
			color = t*green + (1 - t)*beige;
		}
		else if(world_position.y > sand + water_to_sand)
		{
			color = beige;
		}
		else{
			color = blue;
			spec = 1.0;
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
		if (d > far) {
			alpha = 0;
		} 
		else if (d < c) {
			alpha = 1;
		} 
		else {
			alpha = (far - d) / (far - c);
		}

		out_Color = vec4(color * light_intensity * (0.6 + diffuse * 0.4 + specular * spec), alpha) ;
	}
	

	
	

}
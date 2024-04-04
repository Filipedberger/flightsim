#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
out vec3 normal_world;
uniform mat4 in_projectionMatrix;
uniform mat4 model2world;
uniform mat4 world2view;

in vec2 in_TexCoord;
out vec2 fragTexCoord;

uniform vec3 in_color;
out vec3 color;

out vec3 SurfPos;



void main(void)
{	
	gl_Position = in_projectionMatrix * world2view * model2world * vec4(in_Position, 1.0);

	SurfPos = vec3(model2world * vec4(in_Position, 1.0));
	normal_world = vec3(model2world * vec4(in_Normal, 0.0));

	fragTexCoord = in_TexCoord;

	color = in_color;
}

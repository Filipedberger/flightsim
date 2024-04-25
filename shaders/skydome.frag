#version 150

in vec2 fragTexCoord;
in vec4 view_position;
in vec3 normal;

out vec4 out_Color;

uniform sampler2D texUnit;

vec3 ambient = vec3(0.9, 0.9, 0.9); // Ambient light color

void main(void)
{	
    out_Color = vec4(ambient, 1.0) * texture(texUnit, fragTexCoord);
}

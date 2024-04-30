#version 150

in vec2 fragTexCoord;
in vec4 view_position;
in vec3 normal;

out vec4 out_Color;

uniform sampler2D texUnit;

uniform vec3 light_intensity;

void main(void)
{	
    out_Color = vec4(light_intensity, 1.0) * texture(texUnit, fragTexCoord);
}

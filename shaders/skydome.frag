#version 150

in vec2 fragTexCoord;
in vec4 view_position;
in vec3 normal;

out vec4 out_Color;

uniform sampler2D texUnit;

void main(void)
{	
    out_Color = texture(texUnit, fragTexCoord);
}

#version 150

out vec4 out_Color;
in vec3 color;
in vec3 Normal;
in vec2 fragTexCoord;

uniform sampler2D texUnit;

void main(void)
{	

	out_Color = texture(texUnit, fragTexCoord * 3);
}

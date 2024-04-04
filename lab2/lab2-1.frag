#version 150

out vec4 out_Color;
in vec3 color;
in vec3 Normal;
in vec2 fragTexCoord;


void main(void)
{	
	/*vec3 color = vec3(0.5 + 0.5 * sin(fragTexCoord.x *10.0),
					  0.5 + 0.5 * sin(fragTexCoord.y *10.0),
					  0.5 + 0.5 * sin((fragTexCoord.x + fragTexCoord.y) * 5.0))*/

	/*float a = sin(fragTexCoord.s*30)/2+0.5;
	float b = sin(fragTexCoord.t*30)/2+0.5;
	out_Color = vec4(a, b, 1.0, 0.0);*/
	out_Color = vec4(fragTexCoord.s, fragTexCoord.t, 1.0, 0.0);
}

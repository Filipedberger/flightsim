#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
out vec3 Normal;
uniform mat4 in_projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;

out vec3 light;

const vec3 lightInit = vec3(0.58, 0.58, 0.58);

in vec2 in_TexCoord;
out vec2 fragTexCoord;

uniform vec3 in_color;
out vec3 color;

void main(void)
{	
	gl_Position = in_projectionMatrix * viewMatrix* mdlMatrix * vec4(in_Position, 1.0);

	mat3 normalMatrix1 = mat3(mdlMatrix);

	Normal = normalMatrix1 * in_Normal;

	light = lightInit;

	fragTexCoord = in_TexCoord;

	color = in_color;
}

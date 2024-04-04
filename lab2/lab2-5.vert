#version 150

in  vec3 in_Position;
in  vec3 in_color;
in  vec3 in_Normal;
out vec3 color;
out vec3 Normal;
uniform mat4 in_translation;
uniform mat4 in_scale;
uniform mat4 in_rotationZ;
uniform mat4 in_rotationY;
uniform mat4 in_rotationX;
uniform mat4 in_projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;


in vec2 inTexCoord;
out vec2 fragTexCoord;

void main(void)
{
	gl_Position = in_projectionMatrix * viewMatrix* mdlMatrix * in_rotationZ * in_rotationY * in_rotationX * vec4(in_Position, 1.0);
	
	mat3 normalMatrix1 = mat3(mdlMatrix * in_rotationZ * in_rotationY * in_rotationX);
	
	Normal = normalMatrix1 * in_Normal;
	fragTexCoord = inTexCoord;

}

#version 150

in vec4 mdl_matrix;

void main(void)
{
	gl_Position = mdl_matrix;
}

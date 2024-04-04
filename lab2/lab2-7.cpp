// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.
#define MAIN
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "cmath"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
// uses framework OpenGL
// uses framework Cocoa



// Globals

GLfloat in_translation[] = {1.0f, 0.0f, 0.0f, -0.2f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat in_scale[] = {		0.5f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.5f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


GLfloat in_projectionMatrix[] = {  2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
								0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
								0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
								0.0f, 0.0f, -1.0f, 0.0f };

mat4 rot, trans, total, total2;


GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

// vertex array object
unsigned int vertexArrayObjID;

Model *m;
Model *m2;

//unsigned int bunnyVertexArrayObjID;

//unsigned int cubeVertexArrayObjID;


//void DrawModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);



int d = 10;

GLuint program;
GLuint mytex;

void init(void)
{	
	vec3 cameraPosition = vec3(0.0f, 2.0f, 3.0f);
	vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f);
	vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
	mat4 viewMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);




	trans = T(0,0.3,0);
	rot = Ry(0);
	total = trans * rot;

	total2 = T(0,-1,0) * Ry(0);

	m = LoadModel("bunnyplus.obj");
	m2 = LoadModel("cubeplus.obj");

	// vertex buffer object, used for uploading the geometry
	//unsigned int vertexBufferObjID;
	//unsigned int colorBufferObjID;
	// Reference to shader program

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

    glutRepeatingTimer(d);

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
    glUseProgram(program);
	printError("init shader");
	
	LoadTGATextureSimple("rutor.tga", &mytex);
	glBindTexture(GL_TEXTURE_2D, mytex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	

	glUniformMatrix4fv(glGetUniformLocation(program, "in_translation"), 1, GL_TRUE, in_translation);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_scale"), 1, GL_TRUE, in_scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, in_projectionMatrix);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

	// End of upload of geometry
	
	printError("init arrays");
}

float a = 0.0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    a += 0.01;
    GLfloat in_rotationZ[] = { cos(a), -sin(a), 0, 0,
					          sin(a), cos(a), 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1 };

	GLfloat in_rotationY[] = { cos(a), 0 , sin(a), 0,
					          0, 1, 0, 0,
                              -sin(a), 0, cos(a), 0,
                              0, 0, 0, 1 };

	GLfloat in_rotationX[] =  { 1, 0 , 0, 0,
					          0, cos(a), -sin(a), 0,
                              0, sin(a), cos(a), 0,
                              0, 0, 0, 1 };

    glUniformMatrix4fv(glGetUniformLocation(program, "in_rotationZ"), 1, GL_TRUE, in_rotationZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_rotationY"), 1, GL_TRUE, in_rotationY);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_rotationX"), 1, GL_TRUE, in_rotationX);


	//glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 3*18);	// draw object

	//glBindVertexArray(bunnyVertexArrayObjID);	// Select VAO
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");

	//glBindVertexArray(cubeVertexArrayObjID);	// Select VAO
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total2.m);
	DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");

	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(600, 600);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}

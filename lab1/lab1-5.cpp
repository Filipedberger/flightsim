// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "cmath"
// uses framework OpenGL
// uses framework Cocoa

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,0.5f, //1
	-0.5f,-0.5f,-0.5f,//3
	0.5f,-0.5f,0.5f,  //2

	0.5f,-0.5f,0.5f,  //2
	-0.5f,-0.5f,-0.5f, //3
	0.5f,-0.5f,-0.5f, //4

	-0.5f,-0.5f,0.5f, //1
	0.0f,0.5f,0.0f,  //5
	-0.5f,-0.5f,-0.5f,//3

	-0.5f,-0.5f,0.5f, //1
	0.5f,-0.5f,0.5f,  //2
	0.0f,0.5f,0.0f,  //5

	-0.5f,-0.5f,-0.5f,//3
	0.0f,0.5f,0.0f,  //5
	0.5f,-0.5f,-0.5f, //4

	0.5f,-0.5f,0.5f,  //2
	0.5f,-0.5f,-0.5f, //4
	0.0f,0.5f,0.0f   //5
	
};

GLfloat color[] =
{
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0
};

float angle = 3.1415*2000;

GLfloat in_translation[] = {1.0f, 0.0f, 0.0f, -0.2f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat in_scale[] = {		0.5f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.5f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };


GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

// vertex array object
unsigned int vertexArrayObjID;

int d = 10;

GLuint program;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program

	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutRepeatingTimer(d);

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
    glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);

	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*18*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for color data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*18*sizeof(GLfloat), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_color"));
	

	glUniformMatrix4fv(glGetUniformLocation(program, "in_translation"), 1, GL_TRUE, in_translation);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_scale"), 1, GL_TRUE, in_scale);
	


	
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


	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3*18);	// draw object
	
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

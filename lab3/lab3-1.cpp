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
#include "iostream"
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
#define far 50.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


GLfloat in_projectionMatrix[] = {  2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
								0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
								0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
								0.0f, 0.0f, -1.0f, 0.0f };

mat4 bodyPos, bladePos, balconyPos;


GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

Model *blade;
Model *balcony;
Model *roof;
Model *walls;


int d = 10;

GLuint program;
GLuint mytex;

//vec3 body_position = vec3(0.0f, -3.5f, -1.0f);
//vec3 blade_position = vec3(0.0f, 5.5f, 4.0f);

vec3 body_position = vec3(0.0f, -3.5f, -1.0f);
// Postion of the blade relative to the body
vec3 blade_position = body_position + vec3(0, 9.0f, 5.0f);
float blade_speed = 0.01;


void keyboard(unsigned char key, int x, int y) {
    if (key == 'a') body_position.y += 0.1f;
    if (key == 'd') body_position.y -= 0.1f;
	if (key == 'w') blade_position.y += 0.1f;
    if (key == 's') blade_position.y -= 0.1f;
	if (key == 'q') blade_position.z += 0.1f;
    if (key == 'e') blade_position.z -= 0.1f;
	if (key == 'z') blade_speed += 0.01f;
    if (key == 'x') blade_speed -= 0.01f;

	std::cout << body_position.y << '\n';
	std::cout << blade_position.y << '\n';
	std::cout << blade_position.z << '\n';
}

/*void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Handle left button click
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Handle right button click
    } else if (button == GLUT_SCROLL_WHEEL_UP && state == GLUT_DOWN) {
        // Handle scroll wheel up
    } else if (button == GLUT_SCROLL_WHEEL_DOWN && state == GLUT_DOWN) {
        // Handle scroll wheel down
    }
}*/



void init(void)
{	
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);

	vec3 cameraPosition = vec3(5.0f, 10.0f, 30.0f);
	vec3 lookAtPoint = vec3(0.0f, 5.0f, 0.0f);
	vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
	mat4 viewMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);

	bladePos = T(0,0,0) * Ry(0);

	blade = LoadModel("windmill/blade.obj");
	balcony = LoadModel("windmill/windmill-balcony.obj");
	roof = LoadModel("windmill/windmill-roof.obj");
	walls = LoadModel("windmill/windmill-walls2.obj");


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
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
    glUseProgram(program);
	printError("init shader");
	

	glUniformMatrix4fv(glGetUniformLocation(program, "in_translation"), 1, GL_TRUE, in_translation);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_scale"), 1, GL_TRUE, in_scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, in_projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

	// End of upload of geometry
	
	printError("init arrays");
}

float a = 0.0;
float spin = 0.01;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

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

	bodyPos = T(body_position.x,body_position.y,body_position.z) * Ry(0);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bodyPos.m);
	DrawModel(walls, program, "in_Position", "in_Normal", "inTexCoord");

	DrawModel(roof, program, "in_Position", "in_Normal", "inTexCoord");

	bodyPos = bodyPos * Ry(-3.14/2);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bodyPos.m);

	DrawModel(balcony, program, "in_Position", "in_Normal", "inTexCoord");

	spin += blade_speed;
	
	bladePos = T(blade_position.x,blade_position.y,blade_position.z) * Ry(3.14/2) * Rx(spin);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");

	bladePos = bladePos * Rx(3.14/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");


	bladePos = bladePos * Rx(3.14/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");


	bladePos = bladePos * Rx(3.14/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");


	
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
	init();
	glutMainLoop();
	return 0;
}

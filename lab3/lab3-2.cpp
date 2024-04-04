#define MAIN
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "cmath"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "iostream"
#include <algorithm>
// uses framework OpenGL
// uses framework Cocoa

using namespace std;



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


vec3 body_position = vec3(0.0f, -3.5f, 0.0f);
// Postion of the blade relative to the body
vec3 blade_position = body_position + vec3(0.0f, 9.0f, 4.8f);

// Global variables to store the camera position
vec3 cameraPosition = vec3(0.0f, 0.0f, 30.0f);
vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f);
vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
mat4 viewMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);


// Global variables to store the camera's orientation
double theta = - M_PI / 2.0f;
double phi = 0;
int deltaX = -1;
int deltaY = -1;

// Function to handle mouse movement
void mouseMoved(int x, int y) {
	// Warp the cursor back to the center of the window
    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	if (deltaX == -1 && deltaY == -1)
	{
		deltaX = 0;
		deltaY = 0;
		return;
	}
	
    // Calculate the delta from the center of the window
    int deltaX = x - glutGet(GLUT_WINDOW_WIDTH) / 2;
    int deltaY = y - glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // Update theta and phi based on the mouse movement
    // The 0.005f factor is a sensitivity adjustment - you can change this value to make the camera more or less sensitive to mouse movement
    theta += deltaX * 0.001f;
    phi -= deltaY * 0.001f;

    // Clamp phi to be between -pi/2 and pi/2 to prevent the camera from flipping upside down
	float epsilon = 0.01f; // Small value to prevent phi from reaching exactly 0 or pi/2
    phi = std::max(std::min(phi, M_PI / 2.0f - epsilon), epsilon - M_PI / 2.0f);

    // Calculate the direction vector
    vec3 direction(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));

    // Update the lookAtPoint based on the camera position and the direction
    lookAtPoint = cameraPosition + direction;
}


bool forwardMove = false;
bool backwardMove = false;
bool leftMove = false;
bool rightMove = false;
bool upMove = false;
bool downMove = false;

// Function to handle keyboard input for when a key is pressed
void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') {
		forwardMove = true;
		backwardMove = false;
	}
	if (key == 's') {
		backwardMove = true;
		forwardMove = false;
	}
	if (key == 'a') {
		leftMove = true;
		rightMove = false;
	}
	if (key == 'd') {
		rightMove = true;
		leftMove = false;
	}
	if (key == 'z') {
		upMove = false;
		downMove = true;
	}
	if (key == ' ') {
		downMove = false;
		upMove = true;
	}
	if (key == 27) {
		exit(0);
	}
}

// Function to handle keyboard input for when a key is released
void keyboardUp(unsigned char key, int x, int y) {
    if (key == 'w') {
		forwardMove = false;
	}
	if (key == 's') {
		backwardMove = false;
	}
	if (key == 'a') {
		leftMove = false;
	}
	if (key == 'd') {
		rightMove = false;
	}
	if (key == 'z') {
		downMove = false;
	}
	if (key == ' ') {
		upMove = false;
	}
}

static float stepSize = 12.0f;

// Move the camera based on the current state of the arrow keys
void moveCamera(float deltaTime) {
	vec3 forwardDirection = normalize(lookAtPoint - cameraPosition) * stepSize * deltaTime;
	vec3 sideDirection = normalize(cross(forwardDirection, upVector)) * stepSize * deltaTime;

	if (forwardMove) {
		cameraPosition += forwardDirection;
		lookAtPoint += forwardDirection;
	}

	else if (backwardMove) {
		cameraPosition -= forwardDirection;
		lookAtPoint -= forwardDirection;
	}

	if (leftMove) {
		cameraPosition -= sideDirection;
		lookAtPoint -= sideDirection;
	}

	else if (rightMove) {
		cameraPosition += sideDirection;
		lookAtPoint += sideDirection;
	}

	if (upMove) {
        cameraPosition.y += stepSize * deltaTime;
        lookAtPoint.y += stepSize * deltaTime;
    }

    else if (downMove) {
        cameraPosition.y -= stepSize * deltaTime;
        lookAtPoint.y -= stepSize * deltaTime;
    }

}


void init(void)
{	
	glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMoved);
	glutKeyboardUpFunc(keyboardUp);
	glutHideCursor();
	
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
	program = loadShaders("lab3-2.vert", "lab3-2.frag");
    glUseProgram(program);
	printError("init shader");
	

	glUniformMatrix4fv(glGetUniformLocation(program, "in_translation"), 1, GL_TRUE, in_translation);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_scale"), 1, GL_TRUE, in_scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, in_projectionMatrix);

	// End of upload of geometry
	
	printError("init arrays");
}

// Blade spin speed in revolutions per second
float blade_speed = 0.2f;
float blade_angle = 0.0f;

int prevTime = 0;


void display(void)
{
	printError("pre display");
	
	// Handle movement
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	// Calculate the time since the last frame and convert to seconds
    float deltaTime = (currentTime - prevTime) * 0.001f;
    prevTime = currentTime;

	if (forwardMove || backwardMove || leftMove || rightMove || upMove || downMove) {
		// Move the camera
		moveCamera(deltaTime);
	}

	// Update the view matrix
	viewMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw walls and roof
	bodyPos = T(body_position.x,body_position.y,body_position.z) * Ry(0);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bodyPos.m);
	DrawModel(walls, program, "in_Position", "in_Normal", NULL);
	DrawModel(roof, program, "in_Position", "in_Normal", NULL);

	// Draw balcony
	bodyPos = bodyPos * Ry(-M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bodyPos.m);
	DrawModel(balcony, program, "in_Position", "in_Normal", NULL);

	// Draw blades
	blade_angle += blade_speed * 2 * M_PI * deltaTime;
	bladePos = T(blade_position.x,blade_position.y,blade_position.z) * Ry(M_PI/2);

	bladePos = bladePos * Rx(blade_angle);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

	bladePos = bladePos * Rx(M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

	bladePos = bladePos * Rx(M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

	bladePos = bladePos * Rx(M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

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

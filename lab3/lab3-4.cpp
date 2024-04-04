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
#define kGroundSize 100.0f
vec3 vertices[] =
{
 vec3(-kGroundSize,0.0f,-kGroundSize),
 vec3(-kGroundSize,0.0f,kGroundSize),
 vec3(kGroundSize,-0.0f,-kGroundSize),
 vec3(kGroundSize,-0.0f,kGroundSize)
};

vec3 vertex_normals[] =
{
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f)
};

vec2 tex_coords[] =
{
  vec2(0.0f,0.0f),
  vec2(0.0f,20.0f),
  vec2(20.0f,0.0f), 
  vec2(20.0f,20.0f)
};

GLuint indices[] =
{
  0, 1, 2, 1, 3, 2
};


vec3 lightSourcesColorsArr[] = { vec3(1.0f, 0.0f, 0.0f), // Red light

                                 vec3(0.0f, 1.0f, 0.0f), // Green light

                                 vec3(0.0f, 0.0f, 1.0f), // Blue light

                                 vec3(1.0f, 1.0f, 1.0f) }; // White light

GLint isDirectional[] = {0,0,1,1};


vec3 lightSourcesDirectionsPositions[] = { vec3(10.0f, 5.0f, 0.0f), // Red light, positional

                                       vec3(0.0f, 5.0f, 10.0f), // Green light, positional

                                       vec3(-1.0f, 0.0f, 0.0f), // Blue light along X

                                       vec3(0.0f, 0.0f, -1.0f) }; // White light along Z

GLfloat specularExponent[] = {100.0, 200.0, 60.0};

// Colors
vec3 windmillColor = vec3(0.9); // Gray color
vec3 groundColor = vec3(0.3); // Gray color
vec3 teaPotColor = vec3(0.5); // Blue color


#define near 1.0
#define far 100.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


GLfloat in_projectionMatrix[] = {  2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
								0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
								0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
								0.0f, 0.0f, -1.0f, 0.0f };

mat4 bodyPos, bladePos, balconyPos, groundPos, skyPos, teaPotPos;


GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

Model *blade, *balcony, *roof, *walls, *ground, *skybox, *teaPot;

int d = 10;

GLuint program, textureProgram, skyBoxtex, grasstex;


vec3 body_position = vec3(0.0f, -5.5f, 0.0f);
// Postion of the blade relative to the body
vec3 blade_position = body_position + vec3(4.8f, 9.0f, 0.0f);

// Global variables to store the camera position
vec3 cameraPosition = vec3(0.0f, 0.0f, 30.0f);
vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f);
vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
mat4 world2view = lookAtv(cameraPosition, lookAtPoint, upVector);


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
	skybox = LoadModel("labskybox.obj");
	teaPot = LoadModel("models/various/teapot.obj");
	ground = LoadDataToModel(
		vertices,
		vertex_normals,
		tex_coords,
		NULL,
		indices,
		4,
		6
	);

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
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
    glUseProgram(program);
	printError("init shader");

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	

	glUniformMatrix4fv(glGetUniformLocation(program, "in_projectionMatrix"), 1, GL_TRUE, in_projectionMatrix);

	groundPos = T(0.0f, -5.0f, 0.0f);
	teaPotPos = T(20.0f, -5.0f, 20.0f);//* Ry(-M_PI/2);

	textureProgram = loadShaders("lab3-4.vert", "lab3-4-texture.frag");
    glUseProgram(textureProgram);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(textureProgram, "in_projectionMatrix"), 1, GL_TRUE, in_projectionMatrix);
	

	LoadTGATextureSimple("labskybox512.tga", &skyBoxtex);
	glUniform1i(glGetUniformLocation(textureProgram, "texUnit"), 0);
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

	// Move the camera
	moveCamera(deltaTime);

	// Update the view matrix
	glUseProgram(program);

	world2view = lookAtv(cameraPosition, lookAtPoint, upVector);
	skyPos = T(cameraPosition.x, cameraPosition.y - 0.5f, cameraPosition.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "world2view"), 1, GL_TRUE, world2view.m);
	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
	
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	glUseProgram(textureProgram);
	glUniformMatrix4fv(glGetUniformLocation(textureProgram, "world2view"), 1, GL_TRUE, world2view.m);
	glActiveTexture(GL_TEXTURE0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D, skyBoxtex);
	glUniformMatrix4fv(glGetUniformLocation(textureProgram, "model2world"), 1, GL_TRUE, skyPos.m);
	DrawModel(skybox, textureProgram, "in_Position", NULL, "in_TexCoord");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	


	// Draw ground
	glUseProgram(program);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);
	glUniform3f(glGetUniformLocation(program, "in_color"), groundColor.x, groundColor.y, groundColor.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, groundPos.m);
	DrawModel(ground, program, "in_Position", "in_Normal", NULL);

	// Draw teapot
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[2]);
	glUniform3f(glGetUniformLocation(program, "in_color"), windmillColor.x, windmillColor.y, windmillColor.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, teaPotPos.m);
	DrawModel(teaPot, program, "in_Position", "in_Normal", NULL);


	
	// Draw walls and roof
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);
	glUniform3f(glGetUniformLocation(program, "in_color"), windmillColor.x, windmillColor.y, windmillColor.z);
	bodyPos = T(body_position.x,body_position.y,body_position.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, bodyPos.m);
	DrawModel(walls, program, "in_Position", "in_Normal", NULL);
	DrawModel(roof, program, "in_Position", "in_Normal", NULL);

	// Draw balcony
	//bodyPos = bodyPos * Ry(-M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, bodyPos.m);
	DrawModel(balcony, program, "in_Position", "in_Normal", NULL);

	// Draw blades
	blade_angle += blade_speed * 2 * M_PI * deltaTime;
	bladePos = T(blade_position.x,blade_position.y,blade_position.z);

	bladePos = bladePos * Rx(blade_angle);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

	bladePos = bladePos * Rx(M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

	bladePos = bladePos * Rx(M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, bladePos.m);
	DrawModel(blade, program, "in_Position", "in_Normal", NULL);

	bladePos = bladePos * Rx(M_PI/2);
	glUniformMatrix4fv(glGetUniformLocation(program, "model2world"), 1, GL_TRUE, bladePos.m);
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

// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "cmath"
#include "iostream"
#include <algorithm>

// Global variables to store the camera position
vec3 cameraPosition = vec3(0.0f, 0.0f, 30.0f);
vec3 lookAtPoint = vec3(0.0f, 0.0f, 0.0f);
vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
mat4 camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);


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

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	unsigned int x, z;
	
	vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint *) malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)].x = x / 1.0;
			vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)].z = z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)].x = 0.0;
			normalArray[(x + z * tex->width)].y = 1.0;
			normalArray[(x + z * tex->width)].z = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)].y = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMoved);
	glutKeyboardUpFunc(keyboardUp);
	glutHideCursor();
	
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	//LoadTGATextureData("44-terrain.tga", &ttex);
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	printf("Note: The call to DrawModel will report warnings about inNormal not existing. This is because inNormal is not used in the shader yet so it is optimized away.\n");
}

int prevTime = 0;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 total, modelView, camMatrix;
	
	printError("pre display");

	// Handle movement
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	// Calculate the time since the last frame and convert to seconds
    float deltaTime = (currentTime - prevTime) * 0.001f;
    prevTime = currentTime;

	// Move the camera
	moveCamera(deltaTime);
	
	
	glUseProgram(program);

	// Build matrix
	
	//vec3 cam = vec3(0, 5, 8);
	//vec3 lookAtPoint = vec3(2, 0, 2);
	//camMatrix = lookAt(cam.x, cam.y, cam.z,
	//			lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
	//			0.0, 1.0, 0.0);

	camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);
	
	modelView = IdentityMatrix();
	total = camMatrix * modelView;
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void mouse(int x, int y)
{
	// This function is included in case you want some hints about using passive mouse movement.
	// Uncomment to see mouse coordinates:
	// printf("%d %d\n", x, y);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);
	
	//glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}

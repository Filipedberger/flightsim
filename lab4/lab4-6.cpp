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
vec3 cameraPosition = vec3(0.0f, 0.0f, 10.0f);
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
	for (x = 0; x < tex->width; x++) {
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)].x = x / 1.0;
			vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 7.0;
			vertexArray[(x + z * tex->width)].z = z / 1.0;

			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)].y = z; // (float)z / tex->height;
		}
	}

	for (x = 0; x < tex->width; x++) {
		for (z = 0; z < tex->height; z++)
		{
			// Normal vectors. You need to calculate these.
			if ((x != 0) && (x != (tex->width - 1)) && (z != 0) && (z != (tex->height - 1))) {
				vec3 left = vertexArray[(x-1 + z * tex->width)];
				vec3 right = vertexArray[(x+1 + z * tex->width)];
				vec3 up = vertexArray[(x + (z-1) * tex->width)];
				vec3 down = vertexArray[(x + (z+1) * tex->width)];

				vec3 normal = normalize(CrossProduct(up - down, right - left));

				if (normal.y < 0) {
					normal = -normal;
				}

				//std::cout << "x: " << normal.x << " y: " << normal.y << " z: " << normal.z << std::endl;
				
				normalArray[(x + z * tex->width)] = normal;
			}
			else {normalArray[(x + z * tex->width)] = vec3(0.0f, 1.0f, 0.0f);}
		}
	}
		
	
	for (x = 0; x < tex->width-1; x++) {
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

float Get_height(float x, float z, Model* model, TextureData* tex) {
    // Determine which quad the point falls into
    uint xIndex = (int)x;
    uint zIndex = (int)z;
    if (xIndex < 0 || xIndex >= tex->width - 1 || zIndex < 0 || zIndex >= tex->height - 1) {
        // The point is outside the terrain
        return 0.0f;
    }

    // Determine which triangle in the quad the point falls into
    vec3 v1 = model->vertexArray[xIndex + zIndex * tex->width];
    vec3 v2 = model->vertexArray[(xIndex + 1) + zIndex * tex->width];
    vec3 v3 = model->vertexArray[xIndex + (zIndex + 1) * tex->width];
    vec3 v4 = model->vertexArray[(xIndex + 1) + (zIndex + 1) * tex->width];
    bool isUpperTriangle = (x - xIndex) + (z - zIndex) > 1;

    // Use barycentric coordinates to interpolate the y-coordinate
    if (isUpperTriangle) {
        // The point is in the upper triangle (v2, v3, v4)
        float u = x - (xIndex + 1);
        float v = z - (zIndex + 1);
        return v2.y * (1 - u - v) + v3.y * u + v4.y * v;
    } else {
        // The point is in the lower triangle (v1, v2, v3)
        float u = x - xIndex;
        float v = z - zIndex;
        return v1.y * (1 - u - v) + v2.y * u + v3.y * v;
    }
}

float currX = 10;
float currZ = 10;
float objectAngle = M_PI/4;
void moveObject(float step) {
    // Calculate new position
    currX += step * cos(objectAngle);
    currZ += step * sin(objectAngle);

	bool changeAngle = false;
	if (currX < 0) {
		currX = 0;
		changeAngle = true;
	}
	if (currX > 200) {
		currX = 200;
		changeAngle = true;
	}
	if (currZ < 0) {
		currZ = 0;
		changeAngle = true;
	}
	if (currZ > 200) {
		currZ = 200;
		changeAngle = true;
	}
	if (changeAngle) {
		objectAngle = M_PI * (rand() % 360) / 180;
		//printf("angle: %f\n", angle);
	}
}

// vertex array object
Model *m, *m2, *tm, *octagon, *sphere;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

mat4 octagonPos, spherePos;

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

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 400.0);

	// Load and compile shader
	program = loadShaders("terrain6.vert", "terrain6.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Maskros
	glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);

	LoadTGATextureSimple("fft-terrain.tga", &tex2);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1);
	
	// Load terrain data

	//LoadTGATextureData("44-terrain.tga", &ttex);
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	//printf("Note: The call to DrawModel will report warnings about inNormal not existing. This is because inNormal is not used in the shader yet so it is optimized away.\n");

// Load the octagon model
	octagon = LoadModel("octagon.obj");

	float octagon_startPosx = 100;
	float octagon_startPosz = 100;

	float octagon_startPosy = Get_height(octagon_startPosx, octagon_startPosz, tm, &ttex);
	octagonPos = T(octagon_startPosx, octagon_startPosy, octagon_startPosz) * S(10, 10, 10);
	printf("Octagon: x: %f, y: %f, z: %f\n", octagonPos.m[3], octagonPos.m[7], octagonPos.m[11]);
	//newoctagonPos = octagonPos;

// Load the octagon model
	sphere = LoadModel("groundsphere.obj");
	float sphere_startPosx = 80;
	float sphere_startPosz = 80;
	float sphere_startPosy = Get_height(sphere_startPosx, sphere_startPosz, tm, &ttex);
	spherePos = T(sphere_startPosx, sphere_startPosy, sphere_startPosz) * S(10, 10, 10);
	printf("Sphere: x: %f, y: %f, z: %f\n", spherePos.m[3], spherePos.m[7], spherePos.m[11]);
}

int prevTime = 0;
float angle = 0;

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
	camMatrix = lookAtv(cameraPosition, lookAtPoint, upVector);
	
	modelView = IdentityMatrix();
	total = camMatrix * modelView;
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);		// Bind Our Texture tex2

	// We should use a texture when drawing the terrain
	glUniform1i(glGetUniformLocation(program, "useTexture"), 1);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	moveObject(deltaTime*80);
	float currY = Get_height(currX, currZ, tm, &ttex);
	octagonPos = T(currX, currY, currZ) * S(10, 10, 10);
	angle += deltaTime * 10 * M_PI / 2;

	// We should not use a texture when drawing the octagon
	glUniform1i(glGetUniformLocation(program, "useTexture"), 0);

	//newoctagonPos = Change_octagon_pos(newoctagonPos.m[3], newoctagonPos.m[11], tm, &ttex, deltaTime);
	total = camMatrix * octagonPos;// * Ry(angle);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(octagon, program, "inPosition", "inNormal", "inTexCoord");

	total = camMatrix * spherePos;
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(sphere, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}




int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1700, 1000);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);
	
	//glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
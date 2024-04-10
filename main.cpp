#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "game_state.h"
#include "menu_state.h"
#include "context.h"

#include <iostream> 
#include <fstream>
#include <time.h> 
#include <jsoncpp/json/json.h>

#define FPS 4
#define FRAME_DURATION (1000 / FPS) // Duration of each frame in milliseconds

State* state = nullptr;

Context* context = new Context();


static void keyboard_wrapper(unsigned char key, int x, int y){
	state->keyboard(key, x, y);
}

static void keyboard_up_wrapper(unsigned char key, int x, int y){
	state->keyboard_up(key, x, y);
}

static void mouse_wrapper(int x, int y){
	state->mouse(x, y);
}

void display(void){
	static int prev_time = glutGet(GLUT_ELAPSED_TIME);
    int curr_time = glutGet(GLUT_ELAPSED_TIME);
    int time_elapsed = curr_time - prev_time;

    //if (time_elapsed >= FRAME_DURATION) {
     //   prev_time = curr_time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	state->update(time_elapsed);
	state->display();
	glutSwapBuffers();
    //}

}


void init(void)
{
    //glutWarpPointer(1920/2, 1080/2);
    glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	dumpInfo();

	srand(time(NULL));

	// Read settings from json file
    std::ifstream file("settings.json");
    //json reader
    Json::Reader reader;
    //contain complete JSON data
    Json::Value settings;
    // reader reads the data and stores it in settings
    reader.parse(file, settings);

	context -> next_state = nullptr;
	context -> settings = settings;

	state = new Menu_State(context);	
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Flight Simulator");
	glutFullScreen(); 
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);

	glutKeyboardFunc(keyboard_wrapper);
	glutKeyboardUpFunc(keyboard_up_wrapper);
	glutPassiveMotionFunc(mouse_wrapper);

	glutMainLoop();
	exit(0);
}
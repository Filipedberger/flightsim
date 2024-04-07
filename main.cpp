#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "game_state.h"

State* state = nullptr;
int prev_time;
int time_elapsed;


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

	time_elapsed = glutGet(GLUT_ELAPSED_TIME) - prev_time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	state->update(time_elapsed);
	state->display();
	glutSwapBuffers();

}


void init(void)
{
    //glutWarpPointer(1920/2, 1080/2);
    glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	dumpInfo();
	glutKeyboardFunc(keyboard_wrapper);
	glutKeyboardUpFunc(keyboard_up_wrapper);
	glutPassiveMotionFunc(mouse_wrapper);

	state = new Game_State();
	prev_time = 0;
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1920, 1080);
	glutCreateWindow ("Flight Simulator");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);
	glutMainLoop();
	exit(0);
}
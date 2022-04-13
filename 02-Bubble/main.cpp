#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <Windows.h>
#include  "MMSystem.h"


//Remove console (only works in Visual Studio)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps


static int prevTime;
int arc; 
char** arv;


// If a key is pressed this callback is called

static void keyboardDownCallback(unsigned char key, int x, int y)
{
	Game::instance().keyPressed(key);
}

// If a key is released this callback is called

static void keyboardUpCallback(unsigned char key, int x, int y)
{
	Game::instance().keyReleased(key);
}

// If a special key is pressed this callback is called

static void specialDownCallback(int key, int x, int y)
{
	Game::instance().specialKeyPressed(key);
}

// If a special key is released this callback is called

static void specialUpCallback(int key, int x, int y)
{
	Game::instance().specialKeyReleased(key);
}

// Same for changes in mouse cursor position

static void motionCallback(int x, int y)
{
	Game::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases

static void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		Game::instance().mousePress(button);
	else if(state == GLUT_UP)
		Game::instance().mouseRelease(button);
}

static void drawCallback()
{
	Game::instance().render();
	glutSwapBuffers();
}

static void idleCallback()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;
	int slow = 0;

	if (Game::instance().timeSlow) {
		slow = 40;
	}
	if(deltaTime > TIME_PER_FRAME + slow)
	{
		// Every time we enter here is equivalent to a game loop execution
		if(!Game::instance().update(deltaTime))
			exit(0);
		prevTime = currentTime;
		glutPostRedisplay();
	}
}

void playgame(int argc, char** argv) {
	//int game_id = glutCreateWindow(argv[0]);

	//glutSetWindow(game_id);
	glutDisplayFunc(drawCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);

	// GLEW will take care of OpenGL extension functions
	glewExperimental = GL_TRUE;
	glewInit();


	// Game instance initialization
	Game::instance().init();

}

void menu_display() {

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0, 1, 0);
	glRasterPos3f(-0.35, 0.2, 0);
	char msg2[] = "CYBERPUNK 1980";
	for (int i = 0; i < strlen(msg2); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg2[i]);
	}
	
	glColor3f(0, 0, 0);
	glRasterPos3f(-0.3, -0.3, 0);
	char msg1[] = "Press Right Click!";
	for (int i = 0; i < strlen(msg1); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg1[i]);
	}
	glutSwapBuffers();
}


void instruccions_display() {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 1, 0);
	glRasterPos3f(-0.3, 0.8, 0);

	char msg2[] = "CYBERPUNK 1980";
	char msg3[] = "PRESS KEY LEFT/RIGHT TO GO LEFT/RIGHT";
	char msg4[] = "PRESS SPACE TO JUMP ";
	char msg5[] = "PRESS a TO DASH ";
	char msg6[] = "PRESS KEY LEFT/RIGHT + SPACE TO GO LEFT/RIGHT";

	for (int i = 0; i < strlen(msg2); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg2[i]);
	}
	glColor3f(1, 0, 0);
	glRasterPos3f(-0.9, 0.4, 0);
	for (int i = 0; i < strlen(msg3); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg3[i]);
	}
	glRasterPos3f(-0.9, 0.2, 0);
	for (int i = 0; i < strlen(msg4); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg4[i]);
	}
	glRasterPos3f(-0.9, 0, 0);
	for (int i = 0; i < strlen(msg5); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg5[i]);
	}
	glRasterPos3f(-0.9, -0.2, 0);
	for (int i = 0; i < strlen(msg6); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg6[i]);
	}

	glutSwapBuffers();
}

void my_menu(int id) {
	int inst;
	switch (id) {
	case 0:
		PlaySound(NULL, NULL, SND_ASYNC);
		playgame(arc, arv);
		break;
	case 1:
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		inst = glutCreateWindow("Instruccions");
		glutDisplayFunc(instruccions_display);
		glutSetWindow(inst);
		break;
	case 2:
		exit(0);
		break;
	}
}

int main(int argc, char **argv)
{
	// GLUT initialization

	bool played = PlaySound(TEXT("sounds/cyberpunk_music.wav"), NULL, SND_ASYNC);
	glutInit(&argc, argv);
	arc = argc;
	arv = argv;
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	int menu_id = glutCreateWindow("Menu");
	glutDisplayFunc(menu_display);
	glutSetWindow(menu_id);
	int menu = glutCreateMenu(my_menu);
	glutAddMenuEntry("Jugar", 0);
	glutAddMenuEntry("Instruccions", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	prevTime = glutGet(GLUT_ELAPSED_TIME);
	// GLUT gains control of the application
	glutMainLoop();

	return 0;
}





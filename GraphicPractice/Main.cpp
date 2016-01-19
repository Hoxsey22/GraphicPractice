/*
Author: Joseph Hoxsey
Created: 01/19/16


*/

# define __Windows__ // define your target operating system
# ifndef __INCLUDES__
# include "includes.hpp"
# define __INCLUDES__
# endif

using namespace glm;

//////////////////////////////////////////	Global Variables	/////////////////////////////////////////////
// initialze the size of the window
int windowsHeight = 600;
int windowsWidth = 800;

// display state and "state strings" for title display
// window title strings
char baseStr[50] = "Graphics : ";
char fpsStr[15], viewStr[15] = "NULL";
char titleStr[100];

const int nObject = 2;

GLuint VAO[nObject];
GLuint buffer[nObject];
GLuint shaderProgram;
GLuint MVP;
GLuint vPosition[nObject], vColor[nObject], vNormal[nObject];
GLuint Position[nObject], Color[nObject], Normal[nObject];

char * vertexShaderFile = "VertexShader.glsl";
char * fragmentShaderFile = "FragmentShader.glsl";

mat4 pMatrix;	// projection matrix
mat4 vMatrix;	// view matrix
mat4 mMatrix;	// model matrix
mat4 mvpMatrix;	// model view projection matrix

vec3 eye, at, up;

int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;

// values for the time quantum
int timerMode = 0;
const int timeQuantum[4] = {
	40,	//ace
	100,	// pilot
	1000,	//250 trainee
	5000,	// 500 debug
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void init()	{


}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	pMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 1.0f, 300000.0f);
}

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, baseStr);
	strcat(titleStr, viewStr);
	strcat(titleStr, fpsStr);
	glutSetWindowTitle(titleStr);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glutSwapBuffers();
	frameCount++;

	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;

	if (timeInterval >= 1000) {
		sprintf(fpsStr, " fps %4d", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}

void updateMouseMovement()	{
}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	}
	updateTitle();
}

void mouseClick(int button, int state, int x, int y)	{
}

void mouseMotion(int x, int y)	{

}

// updates the model
void update(int i) {
	glutTimerFunc(timeQuantum[(timerMode % 4)], update, 1);
}


int main(int argc, char* argv[])	{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// set the size of the window
	glutInitWindowSize(windowsHeight, windowsWidth);
	// initializes the version of opengl
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// creates the window with a title that is listed in the params
	glutCreateWindow("Graphic Practice");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutTimerFunc(timerDelay, update, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}
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

int index = 0;

// display state and "state strings" for title display
// window title strings
char baseStr[50] = "Graphics : ";
char fpsStr[15], viewStr[15] = "NULL";
char titleStr[100];

const int nObject = 2;
GLuint vao, vbo, ibo;
GLuint shaderProgram;
GLuint MVP;
GLuint vPosition, vColor, vNormal;
GLuint Position, Color, Normal;
GLuint Texture;
GLuint texture;
GLuint MV;
const GLuint NumVertices = 36;

char * textureFilename = "ImageEX.raw";

char * vertexShaderFile = "VertexShader.glsl";
char * fragmentShaderFile = "FragmentShader.glsl";

mat4 pMatrix;	// projection matrix
mat4 vMatrix;	// view matrix
mat4 mMatrix;	// model matrix
mat3 nMatrix;
mat4 mvpMatrix;	// model view projection matrix
mat4 mvMatrix;	// model view projection matrix

vec3 eye, at, up;

vec4 point[NumVertices];
vec4 diffuseColorMaterial[NumVertices];

vec4 const vertex_positions[8] = {
	vec4(-5.0f, -8.0f, 5.0f, 1.0f), // 0 front left bottom 
	vec4(5.0f, -8.0f, 5.0f, 1.0f), // 1 front right bottom
	vec4(5.0f, -8.0f, -5.0f, 1.0f), // 2 back right bottom
	vec4(-5.0f, -8.0f, -5.0f, 1.0f), // 3 back left bottom
	//glm::vec4(0.0f, 8.0f, 0.0f, 1.0f), // 4 apex
	vec4(-5.0f, 8.0f, 5.0f, 1.0f), // 0 front left bottom 
	vec4(5.0f, 8.0f, 5.0f, 1.0f), // 1 front right bottom
	vec4(5.0f, 8.0f, -5.0f, 1.0f), // 2 back right bottom
	vec4(-5.0f, 8.0f, -5.0f, 1.0f), // 3 back left bottom
};

// RGBA colors for each vertex
vec4 const vertex_color[8] = {
	vec4(1.0f, 0.0f, 0.0f, 1.0f),  // 0, red
	vec4(0.0f, 1.0f, 0.0f, 1.0f),  // 1, green
	vec4(0.0f, 0.0f, 1.0f, 1.0f),  // 2, blue
	vec4(0.8f, 0.8f, 0.8f, 1.0f),  // 3, light gray
	vec4(0.8f, 0.8f, 0.8f, 1.0f),  // 4, light gray
	vec4(0.8f, 0.8f, 0.8f, 1.0f),  // 3, light gray
	vec4(0.8f, 0.8f, 0.8f, 1.0f),  // 3, light gray
	vec4(0.8f, 0.8f, 0.8f, 1.0f)   // 4, light gray
};

// Make a triagle surface by vertex reference
void triangle(int a, int b, int c) {
	point[index] = vertex_positions[a]; diffuseColorMaterial[index] = vertex_color[0]; index++;
	point[index] = vertex_positions[b]; diffuseColorMaterial[index] = vertex_color[1]; index++;
	point[index] = vertex_positions[c]; diffuseColorMaterial[index] = vertex_color[2]; index++;
};

void pyramid() {
	triangle(0, 1, 4); // Front Face
	triangle(1, 2, 4); // Right Face 
	triangle(2, 3, 4); // Back Face
	triangle(3, 0, 4); // Left Face
	triangle(0, 2, 1); // Bottom Face front
	triangle(0, 3, 2); // Bottom Face back
};

void cube() {
	// bottom plate
	triangle(0, 1, 2);
	triangle(0, 3, 2);
	// W wall
	triangle(0, 3, 7);
	triangle(0, 4, 7);
	// S wall
	triangle(3, 2, 6);
	triangle(3, 7, 6);
	// E wall
	triangle(1, 2, 6);
	triangle(1, 5, 6);
	// N wall
	triangle(0, 1, 5);
	triangle(0, 4, 5);
	// top plate
	triangle(4, 5, 6);
	triangle(4, 7, 6);
};

int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;

// values for the time quantum
int timerMode = 0;

const int timeQuantum[4] = {
	40,	//ace
	100,	// pilot
	1000,	//250 trainee
	5000	// 500 debug
};

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

void init()	{
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	//pyramid();
	cube();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and init a buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point) + sizeof(diffuseColorMaterial), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point), point);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point), sizeof(diffuseColorMaterial), diffuseColorMaterial);

	// set up vertex arrays (after shaders are loaded)
	vPosition = glGetAttribLocation(shaderProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point)));

	// initially use a front view
	eye = glm::vec3(0.0f, 0.0f, 70.0f);   // eye is 50 "out of screen" from origin
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
	up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector

	glEnable(GL_DEPTH_TEST);

	// set the background to white
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// get elapsed system time
	lastTime = glutGet(GLUT_ELAPSED_TIME);
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
	mvMatrix = lookAt(eye, at, up);
	mvpMatrix = pMatrix * mvMatrix;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

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
	case 'b': case 'B':  // bottom view
		eye = glm::vec3(0.0f, -50.0f, 0.0f);   // eye is -50 "into screen" from origin
		at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		up = glm::vec3(0.0f, 0.0f, -1.0f);  // camera'a up vector
		strcpy(viewStr, " bottom view"); break;
	case 'f': case 'F':  // front view
		eye = glm::vec3(0.0f, 0.0f, 50.0f);   // eye is 50 "out of screen" from origin
		at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
		strcpy(viewStr, " front view"); break;
	case 'l': case 'L':  // left view
		eye = glm::vec3(-50.0f, 0.0f, 0.0f);   // eye is 50 "out of screen" from origin
		at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
		strcpy(viewStr, " left view"); break;
	case 'r': case 'R':  // right view
		eye = glm::vec3(50.0f, 0.0f, 0.0f);   // eye is 50 "out of screen" from origin
		at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera's up vector  
		strcpy(viewStr, " right view"); break;
	case 't': case 'T':  // right view
		eye = glm::vec3(0.0f, 50.0f, 0.0f);   // eye is 50  up from origin
		at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
		up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's up is looking towards -Z vector
		strcpy(viewStr, " top view"); break;
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
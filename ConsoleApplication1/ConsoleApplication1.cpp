#include "pch.h"
#include <windows.h>  // for MS Windows
#include <glut.h>  // GLUT, include glu.h and gl.h
#include <list>
#include "Triangle.h"
#include <random>

std::list<Triangle*> triangles;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> distr(0.0f, 1.0f);
std::uniform_real_distribution<> distr_clamped(0.5f, 1.0f);
int refreshMills = 16;  //around 60 fps

GLfloat angle = 0.0f;
GLfloat angleIncr = 2.0f;
GLfloat spreadIncr = 0.02f;
bool expanding = true;

void Timer(int value) {
	glutPostRedisplay();      
	glutTimerFunc(refreshMills, Timer, 0); 
}

void generateRandomColors(Triangle* t) {
	t->setColor(distr(gen), distr(gen), distr(gen));
	t->setOpacity(distr_clamped(gen));
}

void initGL() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
	Triangle* t;
	//innermost ring
	for (GLuint i = 0; i < 4; ++i) {
		t = new Triangle(0.0f, 0.0f, i * 90.0f);
		generateRandomColors(t);
		triangles.push_back(t);
	}
	
	//middle ring
	for (GLuint i = 0; i < 4; ++i) {
		t = new Triangle(1 * 1.0f, 0 * 1.0f, i * 90.0f);
		generateRandomColors(t);
		triangles.push_back(t);
		t = new Triangle(0 * 1.0f, 1 * 1.0f, i * 90.0f);
		generateRandomColors(t);
		triangles.push_back(t);
	}

	//outer ring
	for (GLuint i = 0; i < 4; ++i) {
		t = new Triangle(1 * 2.0f, 0 * 2.0f, i * 90.0f);
		generateRandomColors(t);
		triangles.push_back(t);
		t = new Triangle(0 * 2.0f , 1 * 2.0f, i * 90.0f);
		generateRandomColors(t);
		triangles.push_back(t);
		t = new Triangle(1 * 1.0f, 1 * 1.0f, i * 90.0f);
		generateRandomColors(t);
		triangles.push_back(t);
	}


}


void display() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);  //rotate the whole coordinate system
	for (auto triangle : triangles) {
		triangle->rotate(angleIncr);
		triangle->spread(spreadIncr);
		triangle->render();
	}
	glFlush();
	angle += angleIncr;
}


void reshape(GLsizei width, GLsizei height) {  

	if (height == 0) height = 1;                
	GLfloat aspect = (GLfloat)width / (GLfloat)height;


	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();             
	if (width >= height) {
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	}
	else {
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
	}
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT
	glutInitWindowSize(640, 480);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("Viewport Transform");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutTimerFunc(0, Timer, 0);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
////////////////////////////////////
//
//	Graph drawing in 3D using forces
//	by Albert Martinez github.com/albertnez
//	12/2/2013
//
////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>


//include openGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//Vec and graph
#include "Vec3d.h"
#include "Graph.h"

using namespace std;

const int screenWidth = 900;
const int screenHeight = 600;
const short RADIUS = 5;

const int margin = 1500;

int K;
double t = 60;

//Mouse variables
bool mouseLeftDown = false;
bool mouseRightDown = false;

float mouseX = 0;
float mouseY = 0;

float posX = 0;
float posY = 0;

float Zoom = -15;

float angleX = 0;
float angleY = 0;

bool autoRotate = false;



//global graf
Graph g;


void update(int value);


double Fa (const double& d) {
	return d*d/K;
}
double Fr (const double& d) {
	return (K*K)/d;
}



//Called when a key is pressed

//S to stop
//Space to add temperature
//R to Reset view angle
//T to toggle autorotate on/off
//Left button (hold) to rotate
//Right button (hold) to move (a bit messy)
void handleKeypress(unsigned char key, int x, int y) {    
	switch (key) {
    case 27: exit(0); break;
    case 's': t = 0; break;
    case ' ': t += 10; break;
    case 'r': angleX = angleY = 0; break;
    case 't': autoRotate = !autoRotate; break;
  }
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    mouseLeftDown = (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN);
    mouseRightDown = (button == GLUT_RIGHT_BUTTON and state == GLUT_DOWN);

    if(button == 3) Zoom += 0.5;
    else if(button == 4) Zoom -= 0.5;
	glutPostRedisplay();	
}

void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        angleX += (x - mouseX);
        angleY += (y - mouseY);
        mouseX = x;
        mouseY = y;
    	glutPostRedisplay();

    }
    else if(mouseRightDown)
    {
    	posX += (x - mouseX)*0.1;
    	posY -= (y - mouseY)*0.1;
    	mouseX = x;
    	mouseY = y;
    	glutPostRedisplay();
    }
}

//initialize

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
/*
	//lighting
	glEnable(GL_LIGHTING); //Enable lighting
	//glEnable(GL_LIGHT0); //light 0
	//glEnable(GL_LIGHT1); 
	glEnable(GL_NORMALIZE); //Automatically normalize
*/
	glClearColor(0.7f, 0.9f, 1.0f, 1.0f); //Change the background to sky blue
}



void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

//
void drawGraph(const Graph& g) {

	glBegin(GL_LINES);
	glColor3f(0.5f, 0.5f, 0.5f);
	double x, y, z;
	int k;
	for (int i = 0; i < g.nodes; ++i) {
		x = g.pos[i].getX()/float(screenHeight);
		y = g.pos[i].getY()/float(screenHeight);
		//Z =
		z = g.pos[i].getZ()/float(screenHeight);


		for (int j = g.ind[i]; j < g.ind[i+1]; ++j) {
			k = g.adj[j];
			glVertex3f(x, y, z);
			//z set to 0
			glVertex3f(g.pos[k].getX()/float(screenHeight), g.pos[k].getY()/float(screenHeight), g.pos[k].getZ()/float(screenHeight));

		}
	}
	glEnd();

	glColor3f(0.6, 0.0, 0.0);
	for (int i = 0; i < g.nodes; ++i) {
		glPushMatrix();
		glTranslatef(g.pos[i].getX()/float(screenHeight), g.pos[i].getY()/float(screenHeight), g.pos[i].getZ()/float(screenHeight));
		glutSolidSphere( 0.1, 12, 12);
		glPopMatrix();
		//drawCircle(g.pos[i].x, g.pos[i].y, RADIUS);
	}	
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,-8);

/*
		//Add ambient light
	GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	//Add positioned light
	GLfloat lightColor0[] = {0.5, 0.5, 0.5, 1};
	GLfloat lightPos0[] = {4, 0, 6, 1}; 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
*/

	glTranslatef(0, 0, Zoom);

	glRotatef(angleY, 1, 0, 0);
	glRotatef(angleX, 0, 1, 0);

	glTranslated(posX, posY, 0);


	glLineWidth(Zoom);
	drawGraph(g);



  glutSwapBuffers();
}
void rotate(int value) {
	if (autoRotate) {
	++angleX;
	if (angleX > 360) angleX -= 360;
	glutPostRedisplay();
}
	if (t > 0) update(0);
	else glutTimerFunc(50, rotate, 0);

}

void update(int value) {
	//repulsion
	for (int i = 0; i < g.nodes; ++i) {
		g.disp[i] = Vec(0,0,0);
		for (int j = 0; j < g.nodes; ++j) {
			if (i != j) {
				Vec dif = g.pos[i] - g.pos[j];
				double m = dif.module();
				if (m != 0) {
					g.disp[i] = g.disp[i] + dif.unit()*Fr(m);
				}
			}
		}
	}

	//atraction
	for (int i = 0; i < g.nodes; ++i) {
		for (int j = g.ind[i]; j < g.ind[i+1]; ++j) {
			Vec dif = g.pos[i] - g.pos[g.adj[j]];
			double m = dif.module();
			if (m != 0) {
				g.disp[i] = g.disp[i] - dif.unit()*Fa(m);
			}
		}
	}

	//temperature
	for (int i = 0; i < g.nodes; ++i) {
		double m = g.disp[i].module();
		//cout << "node " << i << ":  " << m << endl;
		if (m != 0) {
			if ( m < t) {
				g.pos[i] = g.pos[i] + g.disp[i];
			}
			else {
				g.pos[i] = g.pos[i] + g.disp[i].unit()*t;
			}
		}
	}

	t *= 0.99;
	cout << "T: " << t << endl;

	glutPostRedisplay();
	if (t < 0.4) rotate(0);
	else glutTimerFunc(50, update, 0);
}



int main(int argc, char** argv) {
	srand(time(0));
	//Initialize GLUT
	initGraph(g);

	//"Space" = margin^3
	//K = 
	K = sqrt( (margin * margin) / sqrt(g.edges));

	setNodesPosition(g, margin);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	
	//Create the window
	glutCreateWindow("Graph drawing by Albert, now in awesome 3D");
	initRendering();
	
	//Set handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
	glutReshapeFunc(handleResize);
	
	glutTimerFunc(500, update, 0); //Add a timer
	
	glutMainLoop();
	return 0;
}


/*

	//temperature
	for (int i = 0; i < g.nodes; ++i) {
		double m = modf(g.disp[i].x, g.disp[i].y);
		if (m == 0) m = 0.1;
		//change the fucking t!
		g.pos[i].x += (g.disp[i].x/m) * min( abs(m), t );
		g.pos[i].y += (g.disp[i].y/m) * min( abs(m), t );
		//g.pos[i].x = min(xSize - RADIUS, max(0+RADIUS, g.pos[i].x));
		//g.pos[i].y = min(ySize - RADIUS, max(0+RADIUS, g.pos[i].y));
	}
	//t *= 0.99;
	cout << "T: " << t << endl;

	glutPostRedisplay(); //Tell GLUT that the display has changed
	
	//First parameter of timerFunc is the interval in miliseconds
	
	glutTimerFunc(50, update, 0);

}
*/

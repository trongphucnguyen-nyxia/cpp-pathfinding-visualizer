#ifndef MACOS
#include <windows.h>
#pragma comment(lib, "user32.lib") 
#endif

#include <GL/glut.h>  // (or others, depending on the system in use)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#include "Vector.h"
using namespace mathtool;

#include "Environment.h"

bool editEnvironment = true;
bool editStart = false;
bool editGoal = false;

bool isSearching = false;
int iterations = 10;

int numDraws=0;
int windowW = 800;
int windowH = 800;

static int window_id;
static int menu_id;

Environment* gEnv;

void initView() {
  int w = windowW;
  int h = windowH;
  //cout << "initView cameraMode=" << cameraMode << endl;
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0)
    h = 1;
  float ratio = w * 1.0 / h;

  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);

  // Reset Matrix
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the correct perspective.
  //gluPerspective(45.0f, ratio, 0.1f, 10000.0f);
  gluPerspective(90.0f, ratio, 0.1f, 10000.0f);

  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);

  GLfloat Diffuse[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat WhiteLight[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat light_position0[] = { 100.0, 100.0, 100.0, 0.0 };
  GLfloat light_position1[] = { -100.0, 100.0, 100.0, 0.0 };

  glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glLightfv(GL_LIGHT0,GL_DIFFUSE,WhiteLight);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,WhiteLight);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
}

void changeSize(int w, int h) {
  initView();
}

void initEnv() {
  gEnv = new Environment("env.txt",-windowW/2,windowW/2, -windowH/2, windowH/2);
}

void menu(int num) {
  string fileToLoad="";
  bool fileSpecified = false;
  if(num==0) {
    glutDestroyWindow(window_id);
    exit(0);
  }
  else if (num == 1) {
    //cout << "Calling env->FindPathFunction()" << endl;
    //gEnv->FindPathFunction();
    //cout << "--done! Calling env->FindPathFunction()" << endl;
    //gEnv->Search(iterations);
    gEnv->InitializeSearch();
    isSearching = true;
  }
  else if (num == 2) {
    gEnv->ClearPath();
  }
  else if (num == 3) {
    editEnvironment = true;
    editStart = false;
    editGoal = false;
  }
  else if (num == 4) {
    gEnv->SetRandomStartGoalCells();
    gEnv->InitializeSearch();
  }
  else if (num == 5) {
    editEnvironment = false;
    editStart = true;
    editGoal = false;
  }
  else if (num == 6) {
    editEnvironment = false;
    editStart = false;
    editGoal = true;
  }
  else if (num == 7) {
    gEnv->SetSearchType(SearchType::BFS);
    gEnv->InitializeSearch();
  }
  else if (num == 8) {
    gEnv->SetSearchType(SearchType::ASTAR);
    gEnv->InitializeSearch();
  }
  else if (num == 9) {
    gEnv->SetSearchType(SearchType::BESTFIRST);
    gEnv->InitializeSearch();
  }
  else if (num == 10) {
    gEnv = new Environment("env.txt",-windowW/2,windowW/2, -windowH/2, windowH/2);
  }
  else if (num == 11) {
    gEnv = new Environment("env2.txt",-windowW/2,windowW/2, -windowH/2, windowH/2);
  }
}

void createMenu() {
  menu_id = glutCreateMenu(menu);
  glutAddMenuEntry("Quit",0);
  glutAddMenuEntry("FindPath", 1);
  glutAddMenuEntry("ClearPath", 2);
  glutAddMenuEntry("Edit Environment",3);
  glutAddMenuEntry("Random Start/Goal",4);
  glutAddMenuEntry("Set Start",5);
  glutAddMenuEntry("Set Goal",6);
  glutAddMenuEntry("Set Search: BFS",7);
  glutAddMenuEntry("Set Search: A*",8);
  glutAddMenuEntry("Set Search: BestFirst",9);
  glutAddMenuEntry("env.txt",10);
  glutAddMenuEntry("env2.txt",11);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int homeX, homeY;
bool mousePressed=false;
int timeSincePress=0;
int changeY=0;
int changeX=0;
double alpha = 0.5;

void customDraw();
void update() {
  //if( isSimulating ) 
  //  gSim.Update();
  if( isSearching ) 
    gEnv->Search(iterations);

  //cout << "update mousePressed=" << mousePressed << endl;
  customDraw();
}
void customDraw() {
  if( 0 && ++numDraws % 100 == 0 ) {
    cout << " customDraw numDraws = " << numDraws << endl; //<< " doSkel: " << doSkeletalAnimation << endl;
  }
  //////////////////////////////////////////////////////////////
  //perspective drawing
  //
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1, 1, 1, 1.0); //Set display-window color to white.
  //glClear(GL_COLOR_BUFFER_BIT);     //Clear display window.
  glEnable(GL_DEPTH_TEST);
  glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );

  /*if (cameraMode == 0) {
	  // Reset transformations
	  glLoadIdentity();
	  // Set the camera
	  float x = windowW / 2.0;
	  float y = 80;
	  float z = windowH / 2.0;
	  gluLookAt(x, y, z,
		  0, 0, 0,
		  0.0f, 1.0f, 0.0f);
  }
  else {*/
	  // Reset transformations
	  glLoadIdentity();
	  // Set the camera
	  float y = windowW / 2.0;
	  gluLookAt(0, y, 0,
		  0, 0, 0,
		  0.0f, 0.0f, 1.0f);
  //}

  //done test code-perspective drawing
  //////////////////////////////////////////////////////////////



  gEnv->Draw();
  //gSim.Draw();

  //glFlush();                        //Process all OpenGL routines as quickly as possible.
  glutSwapBuffers();
}

void mousebutton(int button, int state, int x, int y)
{
  cout << "editEnvironment=" << editEnvironment << " editStart=" << editStart << " editGoal=" << editGoal << endl;
  if( editEnvironment && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    cout << "Toggle environment at x="<<x<< " y=" << y << endl;
    double tx = -1.0*x + windowW/2.0;
    double tz = 1.0*windowH/2.0-y;
    if( gEnv != NULL ) {
      gEnv->ToggleBlockedCell(tx,tz);
      gEnv->InitializeSearch();
    }
  }
  else if( editStart && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    cout << "set start at environment at x="<<x<< " y=" << y << endl;
    double tx = -1.0*x + windowW/2.0;
    double tz = 1.0*windowH/2.0-y;
    if( gEnv != NULL ) {
      gEnv->SetStartCell(tx,tz);
      gEnv->InitializeSearch();
    }
  }
  else if( editGoal && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    cout << "set goal at environment at x="<<x<< " y=" << y << endl;
    double tx = -1.0*x + windowW/2.0;
    double tz = 1.0*windowH/2.0-y;
    if( gEnv != NULL ) {
      gEnv->SetGoalCell(tx,tz);
      gEnv->InitializeSearch();
    }
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    cout << " release button " << endl;
    mousePressed = false;
  }
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'q':
    case 27: // ESCAPE key
      exit (0);
      break;
    case 'f':
      //gSim.Update();
      //isSimulating = false;
      //break;
      gEnv->Search(iterations);
      isSearching = false;
      break;
    case ' ':
      isSearching = !isSearching; 
  }
}

void otherKeyInput(int key, int x, int y) {
  glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);                         //Initialize GLUT.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   //Set display mode.
	glutInitWindowSize(windowW, windowH);                  //Set display-window width and height.
	window_id = glutCreateWindow("pathFinder");  //Create display window.

	initEnv();                                        //Execute initialization procedure.
	string fileToLoad = "";

	createMenu();
	glutIdleFunc(update);
	glutDisplayFunc(customDraw);                   //Send graphics to display window.
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);                    //How to handle mouse press events. 
	glutSpecialFunc(otherKeyInput);
	glutMainLoop();                                //Display everything and wait.
}


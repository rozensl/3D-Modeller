// Include OpenGL/GLUT Libraries
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utilities/mathLib3D.h"
#include "utilities/material.h"
#include "utilities/sceneObject.h"
#include "utilities/camera.h"
#include "utilities/PPM.h"

/* Global mouse ray in and out points */
double* m_start = new double[3];
double* m_end = new double[3];
/* Floor vertices */
float verts[8][3] = { {10,-2,10}, {0,-0.01,10}, {10,-0.01,10}, {10,-2,10}, {0,-2,0}, {0,-0.01,0}, {10,-0.01,0}, {10,-2,0} };
/* Global object transformation increments */
float x = 0;
float y = 0;
float z = 0;
/* Global world rotation increments */
float sceneRotation[4] = {0, 0, 0, 0};
/* Position, orientation and scale of newly added scene objects */
float objectPosition[] = { 0, 0, 0 };
float objectOrientation[] = {0, 0, 0, 0};
/* Global selected object indicator*/
int globalPicked = 1000;
/* Camera position, light positions, light parameters */
float target[] = {0, 1, 0};
float light_pos0[] = {-3, 2, 3};
float light_pos1[] = {3, 2, -3};
float amb[] = {0.05, 0.05, 0.05};
float diff[] = {0.5, 0.5, 0.5};
float spec[] = {0.5, 0.5, 0.5};
/* Global materials */
Material objectMat = Material();
Material floorMat = Material();
Material axisMat = Material('g');
Material lightMat = Material ('w');
int matType = 0;
/* Global texture variables*/
GLuint textures[3];
int floorTexture = 0;
int carpetWidth, carpetHeight;
int marbleWidth, marbleHeight;
int gridWidth, gridHeight;
/* Camera system */
CameraSystem camera = CameraSystem();
/* Scene object container */
std::vector<sceneObject> sceneObjects;
/* Number of objects currently in the scene */
int objectCount = 0;
/* Mouse movement indicators */
int oldX = 0;
int oldY = 0;
int MIDDLE_BUTTON_CLICKED = 0;
int global_X_handle= 0;
int global_Y_handle= 0;
int global_Z_handle= 0;

int saveFile(){
	std::string filename;
	std::string name = "";
	std::cout << "Enter file name: ";
	std::getline(std::cin, filename);
	filename = filename + ".txt";
	char* Fname = new char[filename.length()];
	for (int i = 0; i < filename.length() ; i++){
		Fname[i] = filename[i];
	}

	std::ofstream myfile;
	myfile.open(Fname);


	for (auto &obj : sceneObjects) {
		int type = obj.objectType;
		if (type == 1){
			name = name + "sphere; ";
		}
		else {
			name = name + "object; ";
		}
    }

    myfile << name;
    myfile.close();
    std::cout << "File saved successfully.\n";
    return 1;
}

int loadFile () {
  std::string filename;
  std::string line;
  std::cout << "Enter file name: ";
  std::getline(std::cin, filename);
  std::ifstream myfile (filename);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      std::cout << line << '\n';
    }
    myfile.close();
  }

  else std::cout << "Unable to open file"; 

  return 0;
}

// adds an object to the scene list
void addObject(unsigned int objectType){
	if (globalPicked != 1000){
		sceneObjects[globalPicked].picked = false;
	}
	sceneObject object = sceneObject(objectType, objectPosition, objectOrientation, matType);
	sceneObjects.push_back(object);
	globalPicked = objectCount;
	objectCount++;
	glutPostRedisplay();
}
// reset global object-transformation increments
void reset(){
	x = 0;
	y = 0;
	z = 0;
}
// draws an axis 
void drawAxis(){
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, -0.01, 0.0);
    glVertex3f(10.0, -0.01, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, -0.01, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, -0.01, 0.0);
    glVertex3f(0.0, -0.01, 10.0);
    glEnd();
    glPopMatrix();
}
/* drawPolygon - takes 4 indices and an array of vertices
 *   and draws a polygon using the vertices indexed by the indices*/
void drawPolygon(int a, int b, int c, int d, float v[8][3]){
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 1);
		glVertex3fv(v[a]);
		glTexCoord2f(0, 0);
		glVertex3fv(v[b]);
		glTexCoord2f(1,0);
		glVertex3fv(v[c]);
		glTexCoord2f(1, 1);
		glVertex3fv(v[d]);
	glEnd();
}
/* cube - takes an array of 8 vertices, and draws 6 faces
 *  with drawPolygon, making up a box */
void floorCube(float v[8][3]){
	glNormal3f(0,1,0);
	drawPolygon(5, 1, 2, 6, v); //top, CCW
}
//draws a floor
void drawFloor(){
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
	floorCube(verts);
	glPopMatrix();
}

//sets lighting parameters
void setLighting(){
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
	glPushMatrix();
	glTranslatef(light_pos0[0], light_pos0[1], light_pos0[2]);
	glutSolidSphere(0.05, 8, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(light_pos1[0], light_pos1[1], light_pos1[2]);
	glutSolidSphere(0.05, 8, 8);
	glPopMatrix();
}
//sets object materials and lighting parameters
void setMaterialsAndLighting(){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightMat.ambient.getColour());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightMat.diffuse.getColour());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightMat.specular.getColour());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, lightMat.shininess);	
	setLighting();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floorMat.ambient.getColour());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floorMat.diffuse.getColour());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorMat.specular.getColour());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2);	
	glBindTexture(GL_TEXTURE_2D, textures[floorTexture]);
	drawFloor();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, axisMat.ambient.getColour());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, axisMat.diffuse.getColour());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, axisMat.specular.getColour());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2);
	glBindTexture(GL_TEXTURE_2D,0);
	drawAxis();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera.getX(), camera.getY(), camera.getZ(), target[0], target[1], target[2], camera.rotX, camera.rotY, camera.rotZ);
	glColor3f(1,1,1);

	//Rotate the entire scene
	glRotatef(sceneRotation[0], sceneRotation[1], sceneRotation[2], sceneRotation[3]);

	setMaterialsAndLighting();

  	for (auto &obj : sceneObjects) {
    	obj.draw();
  	}
	glutSwapBuffers();
}

//set texture
void setTexture(int i, const char* name, int width, int height){
	GLubyte* img_data = LoadPPM(name, &width, &height);
	glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
    	GL_UNSIGNED_BYTE, img_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
//initialize
void init(){
	glClearColor(0.65, 0.55, 0.45, 0);
    glColor3f(1, 1, 1);
    //turn on lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    //turn on perspective projection mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);
    //initialize the ray values
    m_start[0] = 0;
    m_start[1] = 0;
    m_start[2] = 0;

    m_end[0] = 0;
    m_end[1] = 0;
    m_end[2] = 0;

    //Enable texturing
    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, textures);
    setTexture(0, "images/carpet.ppm", carpetWidth, carpetHeight);
    setTexture(1, "images/marble.ppm", marbleWidth, marbleHeight);
    setTexture(2, "images/grid2.ppm", gridWidth, gridHeight);
    }

// Keyboard callback
void keyboard(unsigned char key, int xIn, int yIn){
	switch (key) {
		//Adding objects
		case '0':
			addObject(0);
			break;
		case '1':
			addObject(1);
			break;
		case '2':
			addObject(2);
			break;
		case '3':
			addObject(3);
			break;
		case '4':
			addObject(4);
			break;		
		//Resetting the scene
		case 'q':
			sceneObjects.clear();
			camera.reset();
			objectCount = 0;
			globalPicked = 1000;
			sceneRotation[0] = 0;
			break;
		case 'r':
			camera.reset();
			target[0] = 0;
			sceneRotation[0] = 0;
			break;

		//Changing currently selected object material
		case 'm':
			if (globalPicked != 1000){
        			sceneObjects[globalPicked].changeMaterial(matType);
        		}
        	break;
		//Camera left and right
		case 'a':
			camera.moveLeft();
			break;
		case 'd':
			camera.moveRight();
			break;
		// Zoom camera in and out
		case 'e':
			camera.zoomIn();
			break;
		case 'c':
			camera.zoomOut();
			break;
		case 'w':
			camera.cameraUp();
			break;
		case 'x':
			camera.cameraDown();				
			break;
		//Light movements
		case 'f':
			light_pos0[0] -= 0.5;
			light_pos1[0] -= 0.5;
			break;
		case 'g':
			light_pos0[0] += 0.5;
			light_pos1[0] += 0.5;
			break;
		case 't':
			light_pos0[1] += 0.5;
			light_pos1[1] += 0.5;
			break;
		case 'v':
			light_pos0[1] -= 0.5;
			light_pos1[1] -= 0.5;
			break;
		case 's':
			saveFile();
			break;
		case 'l':
			loadFile();
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
// Mouse-activated scene rotation
void motionMouse(int x, int y){
	y = 800 - y;
	int diffX = x - oldX; //Positive if moving left-to-right
	int diffY = y - oldY; //Positive if moving up
	oldX = x;
   	oldY = y;

	if (global_X_handle){
		if (diffX > 0){
			sceneObjects[globalPicked].update(0.05, 0, 0);
		}
		else {
			sceneObjects[globalPicked].update(-0.05, 0, 0);			
		}
	}
	else if (global_Y_handle){
		if (diffY > 0){
			sceneObjects[globalPicked].update(0, 0.05, 0);
		}
		else {
			sceneObjects[globalPicked].update(0, -0.05, 0);			
		}
	}
	else if (global_Z_handle){
		if (diffX < 0){
			sceneObjects[globalPicked].update(0, 0, 0.05);
		}
		else {
			sceneObjects[globalPicked].update(0, 0, -0.05);			
		}
	}

	else if (MIDDLE_BUTTON_CLICKED){
	// Move camera left and right
   		if (abs(diffX) > 30){
   			if (diffX < 0){
   				target[0] = target[0] - 1;
   				camera.moveLeft();
   			}
   			else{
   				target[0] = target[0] + 1;
   				camera.moveRight();
   			}
   		}
   	// Move camera up and down
   		else if (abs(diffY) > 20){
   			 if (diffY < 0){
   			 	camera.cameraDown();
   			}
   			else{
				camera.cameraUp();
   			}
   		}
   	}	
	glutPostRedisplay();
}

// AABB ray-intersection
double intersect(double* Rd, double updatedBounds[]) { 

	double t1 = (updatedBounds[0] - m_start[0]) / Rd[0];
	double t2 = (updatedBounds[3] - m_start[0]) / Rd[0];
	double t3 = (updatedBounds[1] - m_start[1]) / Rd[1];
	double t4 = (updatedBounds[4] - m_start[1]) / Rd[1];
	double t5 = (updatedBounds[2] - m_start[2]) / Rd[2];
	double t6 = (updatedBounds[5] - m_start[2]) / Rd[2];

	double tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax < 0) {
		return -1;
	}

	if (tmin > tmax){
		return -1;
	}

	if (tmin < 0.0){
		return tmax;
	}
 	return tmin;
}
// Ray-sphere intersection
double raySphereIntersect(double* Rd, double* R0, sceneObject obj){

	double A = Rd[0] * Rd[0] + Rd[1] * Rd[1] + Rd[2] * Rd[2];
	
	// Find intersection point
	double* R0Pc = new double[3];

	R0Pc[0] = R0[0] - (obj).position[0];
	R0Pc[1] = R0[1] - (obj).position[1];
	R0Pc[2] = R0[2] - (obj).position[2];

	double B = 2 * (R0Pc[0] * Rd[0] + R0Pc[1] * Rd[1] + R0Pc[2] * Rd[2]);
	double C = (R0Pc[0] * R0Pc[0] + R0Pc[1] * R0Pc[1] + R0Pc[2] * R0Pc[2])
		- ((obj).scaleFactor[0] * (obj).scaleFactor[1]);

	double discriminant = B*B - 4 * A * C;

	if (discriminant < 0){
		return -1;
	}
	else{
		double t0 = (-B + sqrt(discriminant)) / (2*A);
		double t1 = (-B - sqrt(discriminant)) / (2*A);
		return std::min(t0, t1);
	}
}

void mouse(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){

		double matModelView[16], matProjection[16]; 
		int viewport[4]; 
		// get matrix and viewport:
		glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
		glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
		glGetIntegerv( GL_VIEWPORT, viewport ); 
		// window pos of mouse, Y is inverted on Windows
		double winX = (double)x; 
		double winY = viewport[3] - (double)y; 
		// get point on the 'near' plane (third param is set to 0.0)
		gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
			viewport, &m_start[0], &m_start[1], &m_start[2]); 
		// get point on the 'far' plane (third param is set to 1.0)
		gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
			viewport, &m_end[0], &m_end[1], &m_end[2]); 

			double* R0 = new double[3];
			double* Rd = new double[3];
			R0 = m_start;
			Rd = createVector(m_start, m_end);

			double current_min = 1000;
			int Min_index = 1000;
			double mousePicked;
			double handleXPicked;
			double handleYPicked;
			double handleZPicked;
			global_X_handle = 0;
			global_Y_handle = 0;
			global_Z_handle = 0;

			for (auto obj = sceneObjects.begin(); obj != sceneObjects.end(); ++obj){
				(*obj).picked = false;
				mousePicked = intersect(Rd, (*obj).updatedBounds);
				handleXPicked = intersect(Rd, (*obj).handleBoundsX);
				handleYPicked = intersect(Rd, (*obj).handleBoundsY);
				handleZPicked = intersect(Rd, (*obj).handleBoundsZ);
				if ( (mousePicked != -1) | (handleXPicked != -1) | (handleYPicked != -1) | (handleZPicked != -1)){
					if (handleXPicked != -1){
						global_X_handle= 1;
						current_min = mousePicked;
						Min_index = std::distance( sceneObjects.begin(), obj );
					}
					if (handleYPicked != -1){
						global_Y_handle= 1;
						current_min = mousePicked;
						Min_index = std::distance( sceneObjects.begin(), obj );
					}

					if (handleZPicked != -1){
						global_Z_handle= 1;
						current_min = mousePicked;
						Min_index = std::distance( sceneObjects.begin(), obj );
					}
					else if (mousePicked < current_min){
						current_min = mousePicked;
						Min_index = std::distance( sceneObjects.begin(), obj );
					}					
				}
			}
			// Update selected object
			if (Min_index != 1000){
				globalPicked = Min_index;
				sceneObjects[globalPicked].picked = true;
			}
			glutPostRedisplay();
    	}  
   	}
   	else if (btn == GLUT_RIGHT_BUTTON){
   		if (state == GLUT_DOWN){
   			
		double matModelView[16], matProjection[16]; 
		int viewport[4]; 
		// get matrix and viewport:
		glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
		glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
		glGetIntegerv( GL_VIEWPORT, viewport ); 
		// window pos of mouse, Y is inverted on Windows
		double winX = (double)x; 
		double winY = viewport[3] - (double)y; 
		// get point on the 'near' plane (third param is set to 0.0)
		gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
			viewport, &m_start[0], &m_start[1], &m_start[2]); 
		// get point on the 'far' plane (third param is set to 1.0)
		gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
			viewport, &m_end[0], &m_end[1], &m_end[2]); 

			double* R0 = new double[3];
			double* Rd = new double[3];
			R0 = m_start;
			Rd = createVector(m_start, m_end);

			double current_min = 1000;
			int Min_index = 1000;
			double mousePicked;

			for (auto obj = sceneObjects.begin(); obj != sceneObjects.end(); ++obj){
				(*obj).picked = false;
				mousePicked = intersect(Rd, (*obj).updatedBounds);
				if ( mousePicked != -1){
					if (mousePicked < current_min){
						current_min = mousePicked;
						Min_index = std::distance( sceneObjects.begin(), obj );
					}					
				}
			}
			if (Min_index != 1000){
				sceneObjects.erase(sceneObjects.begin() + (Min_index));
			}
			glutPostRedisplay();
		}
   	}      
   	else if (btn == GLUT_MIDDLE_BUTTON){
   		global_X_handle= 0;
   		global_Y_handle= 0;
   		if (state == GLUT_DOWN){
   			MIDDLE_BUTTON_CLICKED = 1;
   		}
   		else{
   			MIDDLE_BUTTON_CLICKED = 0;
   		}
   	}
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, (float)((w+0.0f)/h), 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

void special(int key, int mouseX, int mouseY){
	int mod = glutGetModifiers();
	 switch(key) {
        case GLUT_KEY_F1:
            matType = 0;
            break;
        case GLUT_KEY_F2:
        	matType = 1;
        	break;
        case GLUT_KEY_F3:
        	matType = 2;
        	break;
        case GLUT_KEY_F4:
            matType = 3;
            break;
        case GLUT_KEY_F5:
        	matType = 4;
        	break;
        case GLUT_KEY_F6:
        	floorTexture = 0;
        	break;
        case GLUT_KEY_F7:
        	floorTexture = 1;
        	break;
        case GLUT_KEY_F8:
        	floorTexture = 2;
        	break;
        case GLUT_KEY_UP:
        	if(globalPicked != 1000){
        		if (mod & GLUT_ACTIVE_SHIFT){
        			sceneObjects[globalPicked].scaleFactor[1] += 0.1;
        		}
        		else if (mod & GLUT_ACTIVE_CTRL){
        			sceneObjects[globalPicked].rotate(10, 1, 0, 1);
        		}
        		else{
        			sceneObjects[globalPicked].scaleFactor[2] += 0.1;
        		}
        	}
        		break;
        case GLUT_KEY_DOWN:
        	if(globalPicked != 1000){
        		if (mod & GLUT_ACTIVE_SHIFT & (sceneObjects[globalPicked].getBottom() >= 0.0f)){
        			sceneObjects[globalPicked].scaleFactor[1] -= 0.1;
        		}
        		else if (mod & GLUT_ACTIVE_CTRL){
        			sceneObjects[globalPicked].rotate(-10, 1, 0, 1);
        		}
        		else{
        			sceneObjects[globalPicked].scaleFactor[2] -= 0.1;
        		}
        	}
        		break;
        case GLUT_KEY_LEFT:
                if (globalPicked != 1000){
                	if (mod & GLUT_ACTIVE_CTRL){
        				sceneObjects[globalPicked].rotate(-10, 0, 1, 0);
        			}
        			else{
        				sceneObjects[globalPicked].scaleFactor[0] -= 0.1;
        			}
        		}
        		break;
        case GLUT_KEY_RIGHT:
                if (globalPicked != 1000){
                	if (mod & GLUT_ACTIVE_CTRL){
        				sceneObjects[globalPicked].rotate(10, 0, 1, 0);
        			}
        			else{
        				sceneObjects[globalPicked].scaleFactor[0] += 0.1;
        			}
        		}
        		break;
        default:
        	break;
    }
    glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3D Modeling System");	//creates the window

	//display callback
	glutDisplayFunc(display);

	//keyboard callback
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	//mouse callback
	glutMouseFunc(mouse);
	glutMotionFunc(motionMouse);

	//resize callback
	glutReshapeFunc(reshape);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	init();
	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}
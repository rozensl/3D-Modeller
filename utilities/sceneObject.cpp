#include "sceneObject.h"
#include <cstdio>

sceneObject::sceneObject() {
   this->objectType = 0;
   this->material = Material();
   this->picked = false;
}

sceneObject::sceneObject(unsigned int type, float pos[], float ori[], int matType){
   for(int i = 0; i < 3; ++i){
      this->position[i] = pos[i];
      this->orientation[i] = ori[i];}

   this->orientation[3] = ori[3];
   this->objectType = type;
   this->picked = true;

   if (matType == 0){
      this->material = Material();
   }
   else if (matType == 1){
      this->material = Material('r');
   }
   else if (matType == 2){
      this->material = Material('g');
   }
   else if (matType == 3){
      this->material = Material('b');
   }
   else if (matType == 4){
      this->material = Material('w');
   }   
}

void sceneObject::draw() {
   updateBounds();
   updateHandleBounds();
   	glColor3f(0.5, 0.5, 0.5);
      
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, this->getAmbient());
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, this->getDiffuse());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, this->getSpecular());
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, this->material.shininess);
   	
      glPushMatrix();
   	glTranslatef(position[0], position[1], position[2]);
      glScalef(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
   	glRotatef(orientation[0], orientation[1], orientation[2], orientation[3]);

   	switch (objectType) {
   		case 0:
   			glutSolidCube(1);
   			break;
   		case 1:
   			glutSolidSphere(0.5, 16, 16);
   			break;
   		case 2:
   			glutSolidCone(0.5, 0.5, 16, 16);
   			break;
   		case 3:
   			glutSolidTorus(0.20, 0.3, 16, 16);
   			break;
   		case 4:
            glPushMatrix();
            glScalef(0.8, 0.8, 0.8);
   			glutSolidTetrahedron();
            glPopMatrix();
   			break;
   	}
      if (this->picked){
         drawHandles();
         drawBoundingBox();
      }
   	glPopMatrix();
}

static GLfloat* generateColour(int i){
   GLfloat * colour = new GLfloat[4];
   if (i == 0){
      colour[0] = 0.5;
      colour[1] = 0;
      colour[2] = 0;
      colour[3] = 1;
   }
   else if (i == 1){
      colour[0] = 0;
      colour[1] = 0.5;
      colour[2] = 0;
      colour[3] = 1;
   }
   else if (i == 2){
      colour[0] = 0.25;
      colour[1] = 0.25;
      colour[2] = 0;
      colour[3] = 1;
   }
   else if (i == 3){
      colour[0] = 0;
      colour[1] = 0;
      colour[2] = 0.5;
      colour[3] = 1;
   }
   else {
      colour[0] = 0.5;
      colour[1] = 0.5;
      colour[2] = 0.5;
      colour[3] = 1;      
   }
   return colour;
}

static void setMaterials(GLfloat* colour){
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colour);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colour);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

void sceneObject::drawHandles(){
   GLfloat* colour = generateColour(0);
   setMaterials(colour);

   glPushMatrix();
   glTranslatef(0.75, 0, 0);
   glScalef(0.5, 0.10, 0.10);
   glutSolidCube(1);
   glPopMatrix();

   colour = generateColour(2);
   setMaterials(colour);

   glPushMatrix();
   glTranslatef(0.0, 0.75, 0);
   glScalef(0.10, 0.5, 0.10);
   glutSolidCube(1);
   glPopMatrix();

   colour = generateColour(3);
   setMaterials(colour);
   glPushMatrix();
   glTranslatef(0.0, 0, 0.75);
   glScalef(0.10, 0.10, 0.50);
   glutSolidCube(1);
   glPopMatrix();   
}

void sceneObject::updateHandleBounds(){
      handleBoundsX[0] = (0.5 * this->scaleFactor[0] + this->position[0]);// x1  -0.5
      handleBoundsX[1] = (-0.05 * this->scaleFactor[1] + this->position[1]);// y1  -0.5
      handleBoundsX[2] = (-0.05 * this->scaleFactor[2] + this->position[2]);// z1  -0.5
      handleBoundsX[3] = (1.05 * this->scaleFactor[0] + this->position[0]);     //  0.5
      handleBoundsX[4] = (0.05 * this->scaleFactor[1] + this->position[1]); //y2    0.5
      handleBoundsX[5] = (0.05 * this->scaleFactor[2] + this->position[2]); //z2    0.5

      handleBoundsY[0] = (-0.1 * this->scaleFactor[0] + this->position[0]);// x1  -0.5
      handleBoundsY[1] = (0.5 * this->scaleFactor[1] + this->position[1]);// y1  -0.5
      handleBoundsY[2] = (-0.1 * this->scaleFactor[2] + this->position[2]);// z1  -0.5
      handleBoundsY[3] = (0.1 * this->scaleFactor[0] + this->position[0]);     //  0.5
      handleBoundsY[4] = (1.05 * this->scaleFactor[1] + this->position[1]); //y2    0.5
      handleBoundsY[5] = (0.1 * this->scaleFactor[2] + this->position[2]); //z2    0.5

      handleBoundsZ[0] = (-0.05 * this->scaleFactor[0] + this->position[0]);// x1  -0.5
      handleBoundsZ[1] = (-0.05 * this->scaleFactor[1] + this->position[1]);// y1  -0.5
      handleBoundsZ[2] = (0.5 * this->scaleFactor[2] + this->position[2]);// z1  -0.5
      handleBoundsZ[3] = (0.05 * this->scaleFactor[0] + this->position[0]);     //  0.5
      handleBoundsZ[4] = (0.05 * this->scaleFactor[1] + this->position[1]); //y2    0.5
      handleBoundsZ[5] = (1.05 * this->scaleFactor[2] + this->position[2]); //z2    0.5
}
void sceneObject::updateBounds() {
   updatedBounds[0] = (-0.55 * this->scaleFactor[0] + this->position[0]);// x1  -0.5
   updatedBounds[1] = (-0.55 * this->scaleFactor[1] + this->position[1]);// y1  -0.5
   updatedBounds[2] = (-0.55 * this->scaleFactor[2] + this->position[2]);// z1  -0.5
   updatedBounds[3] = (0.55 * this->scaleFactor[0] + this->position[0]);     //  0.5
   updatedBounds[4] = (0.55 * this->scaleFactor[1] + this->position[1]); //y2    0.5
   updatedBounds[5] = (0.55 * this->scaleFactor[2] + this->position[2]); //z2    0.5
}

void sceneObject::drawBoundingBox() {
   GLfloat* colour = generateColour(1);
   setMaterials(colour);
   glLineWidth(1.0);
   int i = 1;
   while (i < 5){
      glBegin(GL_LINES);
      glVertex3f(bounds[0], bounds[i], bounds[2]);
      glVertex3f(bounds[3], bounds[i], bounds[2]);
      glEnd();

      glBegin(GL_LINES);
      glVertex3f(bounds[3], bounds[i], bounds[2]);
      glVertex3f(bounds[3], bounds[i], bounds[5]);
      glEnd();

      glBegin(GL_LINES);
      glVertex3f(bounds[3], bounds[i], bounds[5]);
      glVertex3f(bounds[0], bounds[i], bounds[5]);
      glEnd();

      glBegin(GL_LINES);
      glVertex3f(bounds[0], bounds[i], bounds[5]);
      glVertex3f(bounds[0], bounds[i], bounds[2]);
      glEnd();

      glBegin(GL_LINES);
      glVertex3f(bounds[0], bounds[i], bounds[5]);
      if (i == 1){
         glVertex3f(bounds[3], bounds[4], bounds[5]);   
      }
      else{
         glVertex3f(bounds[3], bounds[1], bounds[5]);
      }   
      glEnd();

      glBegin(GL_LINES);
         glVertex3f(bounds[3], bounds[i], bounds[2]);
      if (i == 1){
         glVertex3f(bounds[3], bounds[4], bounds[5]);   
      }
      else{
         glVertex3f(bounds[3], bounds[1], bounds[5]);
      }   
      glEnd();

      i += 3;
   }
   glPopMatrix();
}

void sceneObject::update(float x, float y, float z){
      this->position[0] += x;
      this->position[1] += y;
      this->position[2] += z; 
}

void sceneObject::rotate(float amount, float x, float y, float z){
      this->orientation[0] += amount;
      this->orientation[1] = x;
      this->orientation[2] = y;
      this->orientation[3] = z;
}

void sceneObject::changeMaterial(int matType){
   if (matType == 0){
      this->material = Material();
   }
   else if (matType == 1){
      this->material = Material('r');
   }
   else if (matType == 2){
      this->material = Material('g');
   }
   else if (matType == 3){
      this->material = Material('b');
   }
   else if (matType == 4){
      this->material = Material('w');
   }
}

float* sceneObject::getAmbient(){
   return this->material.ambient.getColour();
}
float* sceneObject::getDiffuse(){
   return this->material.diffuse.getColour();
}
float* sceneObject::getSpecular(){
   return this->material.specular.getColour();
}

float sceneObject::getBottom(){
   return updatedBounds[1];
}
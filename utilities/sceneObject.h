#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

// include dependencies
#include "material.h"
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>

class sceneObject 
{
public:
	int objectType;
    float position[3] = {0, 0, 0};
    float orientation[4] = {0, 0, 0, 0};
    float scaleFactor[3] = {1, 1, 1};
    Material material;
    bool picked;
    /* An array of 6 values: lower and upper x-plane bounds, lower and
    upper y-plane bounds, lower and upper z-plane bounds. */
    double bounds[6] = {-0.55, -0.55, -0.55, 0.55, 0.55, 0.55};
    double updatedBounds[6] = {-0.55, -0.55, -0.55, 0.55, 0.55, 0.55};
    
    double handleBoundsX[6] = {0.50, -0.05, -0.05, 1.05, 0.05, 0.05};
    double handleBoundsY[6] = {-0.05, 0.50, -0.05, 0.05, 1.05, 0.05};
    double handleBoundsZ[6] = {-0.05, -0.05, 0.50, 0.05, 0.05, 1.05};

    sceneObject();
    sceneObject(unsigned int type, float* pos, float* ori, int matType);
    void draw();
    void updateBounds();
    void drawBoundingBox();
    void update(float x, float y, float z);
    void rotate(float amount, float x, float y, float z);
    void changeMaterial(int matType);
    float* getAmbient();
    float* getDiffuse();
    float* getSpecular();
    float getBottom();
    void drawHandles();
    void updateHandleBounds();
};

#endif
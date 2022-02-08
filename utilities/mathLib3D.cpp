#include <math.h>
#include <iostream>
#include "mathLib3D.h"
using namespace std;

Point3D::Point3D(){
	this->x = float(rand() % 3);
	this->y = 0;
	this->z = float(rand() % 3);
}

Point3D::Point3D(float inX, float inY, float inZ){
	this->x = inX;
	this->y = inY;
	this->z = inZ;
}

float Point3D::distanceTo(Point3D other){
	 return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
}

float Point3D::fastDistanceTo(Point3D other) {
    return (pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
}

Vec3D::Vec3D(){
    this->x = float(rand() % 2);
    this->y = float(rand() % 5 + 2);
    this->z = float(rand() % 2);
};

Vec3D::Vec3D(float inX, float inY, float inZ) {
    this->x = inX;
    this->y = inY;
    this->z = inZ;
}

float Vec3D::length() {
    return abs(sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2)));
}

Vec3D Vec3D::normalize() {
    float x = this->x / this->length();
    float y = this->y / this->length();
    float z = this->z / this->length();
    return Vec3D(x, y, z);
}

Vec3D Vec3D::add(Vec3D other) {
    float x = this->x + other.x;
    float y = this->y + other.y;
    float z = this->z + other.z;
    return Vec3D(x, y, z);
}

Vec3D Vec3D::multiply(float scalar) {
    float x = this->x * scalar;
    float y = this->y * scalar;
    float z = this->z * scalar;
    return Vec3D(x, y, z);
}

void Vec3D::applyForce(Vec3D forceVector){
	this->x = this->x + forceVector.x;
	this->y = this->y + forceVector.y;
	this->z =this->z + forceVector.z;
}

// The vector values behave like speed, i.e. this->x means 
// movement of this particle in X direction is (this->x) units/frame.
// i.e. Every frame, thisParticle.movePoint(origin), using the particle's
// speed/direction vectors.
Point3D Vec3D::movePoint(Point3D currentOrigin) {
    float postMovementX = this->x + currentOrigin.x;
    float postMovementY = this->y + currentOrigin.y;
    float postMovementZ = this->z + currentOrigin.z;
    return Point3D(postMovementX, postMovementY, postMovementZ);
}

void Vec3D::reverse() {
    this->x = -x;
    this->y = -y;
    this->z = -z;
}

double magnitude (double* p1, double * p2){
    double value = sqrt(pow(p2[0]- p1[0], 2) + pow(p2[1]- p1[1], 2) + pow(p2[2]- p1[2], 2));
     return value;
}

double* createVector(double* p1, double* p2) {
    double* dir = new double[3];
    double mag = magnitude(p1, p2);
    dir[0] = (p2[0] - p1[0]) / mag;
    dir[1] = (p2[1] - p1[1]) / mag;
    dir[2] = (p2[2] - p1[2]) / mag;
    for (int i = 0; i < 3; i++){
        // as a string.
            double var = dir[i];
            char str[40];
            // Print in string the value of var
            // with two decimal point
            sprintf(str, "%.5lf", var);
            // scan string value in var
            sscanf(str, "%lf", &var);
            dir[i] = var;
    }
    return dir;
}
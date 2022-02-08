#ifndef MATHLIB_3D_H
#define MATHLIB_3D_H

class Point3D {
public:
    Point3D();
    Point3D(float inX, float inY, float inZ);
    float x;
    float y;
    float z;
    float distanceTo(Point3D other);
    float fastDistanceTo(Point3D other);
};

class Vec3D {
public:
    Vec3D();
    Vec3D(float inX, float inY, float inZ);
    float x;
    float y;
    float z;
    float length();
    Vec3D normalize();
    Vec3D add(Vec3D other);
    Vec3D multiply(float scalar);

    void applyForce(Vec3D forceVector);
    Point3D movePoint(Point3D source);
    void reverse();
};

double magnitude (double* p1, double* p2);
double* createVector(double* p1, double* p2);
#endif

/*Particle position
particle movement direction/vector
particle movement speed
particle rotation angle (optional)
particle size
particle colour
material
age*/
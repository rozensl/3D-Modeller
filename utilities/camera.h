#ifndef CAMERA_H
#define CAMERA_H

class CameraSystem
{
private:
    float rotation, radius;
    int height;

public:
    float rotX, rotY, rotZ;
    CameraSystem();
    void updateRadius(int r);
    void updateRotation(float rot);
    float getX();
    float getY();
    float getZ();
    void moveRight();
    void moveLeft();
    void zoomIn();
    void zoomOut();
    void reset();
    void cameraUp();
    void cameraDown();
    void tiltLeft();
    void tiltRight();
};

#endif

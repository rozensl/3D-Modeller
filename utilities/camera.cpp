#include <math.h>
#include "camera.h"

CameraSystem::CameraSystem()
{
    reset();
}
void CameraSystem::updateRadius(int r)
{
    if (r > 1)
    {
        radius = r;
    }
    else
    {
        radius = 2;
    }
}
void CameraSystem::updateRotation(float rot)
{
    rotation = fmodf(rot, 360);
}
float CameraSystem::getX()
{
    return (radius * sin(rotation));
}
float CameraSystem::getY()
{
    return height;
}
float CameraSystem::getZ()
{
    return (radius * cos(rotation));
}
void CameraSystem::moveRight()
{
    updateRotation(rotation + 0.1);
}
void CameraSystem::moveLeft()
{
    updateRotation(rotation - 0.1);
}
void CameraSystem::zoomIn()
{
    updateRadius(radius - 1);
}
void CameraSystem::zoomOut()
{
    updateRadius(radius + 1);
}
void CameraSystem::cameraUp()
{
    height += 1;
}
void CameraSystem::cameraDown()
{
    height -= 1;
}
void CameraSystem::reset()
{
    rotation = 0.785;
    height = 2;
    radius = 10;
    rotX = 0;
    rotY = 1;
    rotZ = 0;
}

void CameraSystem::tiltLeft()
{
    rotX += 0.01;
}
void CameraSystem::tiltRight()
{
    rotX -= 0.01;
}
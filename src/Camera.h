#pragma once
#include "types.h"

class Camera
{
private:
    Point position;
    Ray topLeft;
    Ray topRight;
    Ray bottomLeft;
    Ray bottomRight;
    float viewDistance;
    Ray direction;

public:
    Camera();
    void setPosition(float a, float b, float c);
    void move(float a, float b, float c);
    void setViewDistance(float d);
    void setTopLeft(float t, float f);
    void setTopRight(float t, float f);
    void setBottomLeft(float t, float f);
    void setBottomRight(float t, float f);
    Ray getDirection();
    Point getPosition();
    Ray getBottomLeft();
    Ray getBottomRight();
    Ray getTopRight();
    Ray getTopLeft();
    void changeDirection(float thetaOffset, float fiOffset);
};
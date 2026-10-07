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
    void absMove(float a, float b, float c);
    void relMove(Move m, float speed);
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
    void absChangeDirection(float thetaOffset, float fiOffset);
    void relChangeDirection(Direction dir, float sensitivity);
};
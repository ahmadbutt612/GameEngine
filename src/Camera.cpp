#include "Camera.h"
#include <cmath>

Camera::Camera()
{
    viewDistance = 50;
    position = Point(0, 0, 0);
    topLeft = Ray(position, Angle(0.f, 0.f), viewDistance);
    topRight = Ray(position, Angle(0.f, 0.f), viewDistance);
    bottomLeft = Ray(position, Angle(0.f, 0.f), viewDistance);
    bottomRight = Ray(position, Angle(0.f, 0.f), viewDistance);
    direction = Ray(position, Angle((topLeft.angle.theta + topRight.angle.theta) / 2, (bottomLeft.angle.fi = topLeft.angle.fi) / 2), viewDistance);
}

void Camera::setPosition(float a, float b, float c)
{
    position.x = a;
    position.y = b;
    position.z = c;
    topLeft.origin = Point(a, b, c);
    topRight.origin = Point(a, b, c);
    bottomLeft.origin = Point(a, b, c);
    bottomRight.origin = Point(a, b, c);
    direction.origin = Point(a, b, c);
}

void Camera::absMove(float a, float b, float c)
{
    setPosition(position.x + a, position.y + b, position.z + c);
}

void Camera::relMove(Move m, float speed)
{
    float angle = getDirection().angle.theta;
    switch (m)
    {
    case Move::Right:
        absMove(sin(angle * 0.0174533) * speed, -cos(angle * 0.0174533) * speed, 0.f);
        break;
    case Move::Left:
        absMove(-sin(angle * 0.0174533) * speed, cos(angle * 0.0174533) * speed, 0.f);
        break;
    case Move::Forward:
        absMove(cos(angle * 0.0174533) * speed, sin(angle * 0.0174533) * speed, 0.f);
        break;
    case Move::Backward:
        absMove(-cos(angle * 0.0174533) * speed, -sin(angle * 0.0174533) * speed, 0.f);
        break;
    }
}

void Camera::setViewDistance(float d)
{
    if (d < 1)
        d = 1;
    viewDistance = d;
    topLeft.length = viewDistance;
    topRight.length = viewDistance;
    bottomLeft.length = viewDistance;
    bottomRight.length = viewDistance;
    direction.length = viewDistance;
}

void Camera::setTopLeft(float t, float f)
{
    topLeft.angle.theta = t;
    topLeft.angle.fi = f;
    direction = Ray(position, Angle((topLeft.angle.theta + topRight.angle.theta) / 2, (bottomLeft.angle.fi + topLeft.angle.fi) / 2), viewDistance);
}

void Camera::setTopRight(float t, float f)
{
    topRight.angle.theta = t;
    topRight.angle.fi = f;
    direction = Ray(position, Angle((topLeft.angle.theta + topRight.angle.theta) / 2, (bottomLeft.angle.fi + topLeft.angle.fi) / 2), viewDistance);
}

void Camera::setBottomLeft(float t, float f)
{
    bottomLeft.angle.theta = t;
    bottomLeft.angle.fi = f;
    direction = Ray(position, Angle((topLeft.angle.theta + topRight.angle.theta) / 2, (bottomLeft.angle.fi + topLeft.angle.fi) / 2), viewDistance);
}

void Camera::setBottomRight(float t, float f)
{
    bottomRight.angle.theta = t;
    bottomRight.angle.fi = f;
    direction = Ray(position, Angle((topLeft.angle.theta + topRight.angle.theta) / 2, (bottomLeft.angle.fi + topLeft.angle.fi) / 2), viewDistance);
}

Ray Camera::getDirection()
{
    return direction;
}

Point Camera::getPosition()
{
    return position;
}

Ray Camera::getBottomLeft()
{
    return bottomLeft;
}

Ray Camera::getBottomRight()
{
    return bottomRight;
}

Ray Camera::getTopRight()
{
    return topRight;
}

Ray Camera::getTopLeft()
{
    return topLeft;
}

void Camera::absChangeDirection(float thetaOffset, float fiOffset)
{
    topRight.angle.theta = topRight.angle.theta + thetaOffset;
    topLeft.angle.theta = topLeft.angle.theta + thetaOffset;
    bottomRight.angle.theta = bottomRight.angle.theta + thetaOffset;
    bottomLeft.angle.theta = bottomLeft.angle.theta + thetaOffset;
    topRight.angle.fi = topRight.angle.fi + fiOffset;
    topLeft.angle.fi = topLeft.angle.fi + fiOffset;
    bottomRight.angle.fi = bottomRight.angle.fi + fiOffset;
    bottomLeft.angle.fi = bottomLeft.angle.fi + fiOffset;
    direction = Ray(position, Angle((topLeft.angle.theta + topRight.angle.theta) / 2, (bottomLeft.angle.fi + topLeft.angle.fi) / 2), viewDistance);
}

void Camera::relChangeDirection(Direction dir, float sensitivity)
{
    switch (dir)
    {
    case Direction::Right:
        absChangeDirection(-sensitivity, 0.f);
        break;
    case Direction::Left:
        absChangeDirection(sensitivity, 0.f);
        break;
    case Direction::Up:
        absChangeDirection(0.f, -sensitivity);
        break;
    case Direction::Down:
        absChangeDirection(0.f, sensitivity);
        break;
    }
}

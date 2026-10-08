#pragma once
#include <SFML/Graphics/Color.hpp>

enum class Move 
{
    Forward,
    Backward,
    Left,
    Right
};
enum class Direction
{
    Up,
    Down,
    Left,
    Right
};
struct Point
{
    float x;
    float y;
    float z;
    Point() : x(0), y(0), z(0) {}
    Point(float a, float b, float c) : x(a), y(b), z(c) {}
    Point operator-(const Point other) const
    {
        return Point{x - other.x, y - other.y, z - other.z};
    }
};
struct Angle
{
    float theta;
    float fi;
    Angle(): theta(0), fi(0) {}
    Angle(float t, float f): theta(t), fi(f) {}
};
struct Ray
{
    Point origin;
    Angle angle;
    float length;
    Ray(): origin(Point(0.f, 0.f, 0.f)), angle(Angle(0.f, 0.f)), length(0.f) {}
    Ray(Point p, Angle a, float l): origin(p), angle(a), length(l) {}
};

struct CollisionReturn
{
    float distance;
    sf::Color color;
};
struct GPUTriangle
{
    float p1[4];
    float p2[4];
    float p3[4];
    float color[4];
};
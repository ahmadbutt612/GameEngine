#pragma once
#include "types.h"
#include "Object.h"
#include "SFML/Graphics/Color.hpp"

struct Triangle : public Object
{
    Point p1;
    Point p2;
    Point p3;
    sf::Color color;
    void move(float x, float y, float z);
    Triangle();
    Triangle(Point a, Point b, Point c, sf::Color col);
    CollisionReturn getMinCollisionDistance(Ray r);
    void collectTriangles(std::vector<GPUTriangle> &out) override;
};
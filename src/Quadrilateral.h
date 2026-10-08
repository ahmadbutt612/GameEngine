#pragma once
#include "types.h"
#include "Object.h"
#include "Triangle.h"
#include "SFML/Graphics/Color.hpp"

class Quadrilateral: public Object
{
private:
    Triangle t1;
    Triangle t2;
public:
    Quadrilateral();
    Quadrilateral(Point a, Point b, Point c, Point d, sf::Color col);

    void move(float x, float y, float z);
    CollisionReturn getMinCollisionDistance(Ray r);
    void collectTriangles(std::vector<GPUTriangle> &out);

    void setPoints(Point a, Point b, Point c, Point d);
    void setColor(sf::Color col);

    ~Quadrilateral();
};
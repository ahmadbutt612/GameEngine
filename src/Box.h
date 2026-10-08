#pragma once
#include "types.h"
#include "Object.h"
#include "Quadrilateral.h"
#include "SFML/Graphics/Color.hpp"
#include <vector>

class Box: public Object
{
private:
    Quadrilateral faces[6];
    Point origin;
    float length;
    float width;
    float height;
public:
    Box();
    Box(Point org, float l, float w, float h, sf::Color col);
    void move(float x, float y, float z);
    CollisionReturn getMinCollisionDistance(Ray r);
    void collectTriangles(std::vector<GPUTriangle> &out);
    ~Box();
};
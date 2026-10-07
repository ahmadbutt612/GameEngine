#pragma once
#include "types.h"
#include "vector"

class Object
{
public:
    virtual void move(float x, float y, float z) = 0;
    virtual CollisionReturn getMinCollisionDistance(Ray r) = 0;
    virtual void collectTriangles(std::vector<GPUTriangle> &out) = 0;
    virtual ~Object() = default;
};
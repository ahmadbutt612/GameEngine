#pragma once
#include "Object.h"
#include <vector>

class GroupedObject : public Object
{
private:
    std::vector<Object *> arr;
public:
    void addObject(Object *obj);
    void move(float x, float y, float z);
    CollisionReturn getMinCollisionDistance(Ray r);
    void collectTriangles(std::vector<GPUTriangle> &out) override;
    ~GroupedObject();
};
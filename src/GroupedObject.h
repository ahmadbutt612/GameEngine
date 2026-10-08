#pragma once
#include "Object.h"
#include <vector>
#include <memory>

class GroupedObject : public Object
{
private:
    std::vector<std::shared_ptr<Object>> arr;
public:
    void addObject(std::shared_ptr<Object> obj);
    void move(float x, float y, float z);
    CollisionReturn getMinCollisionDistance(Ray r);
    void collectTriangles(std::vector<GPUTriangle> &out) override;
    ~GroupedObject();
};
#include "GroupedObject.h"
#include "types.h" //because I am using CollisionReturn explicitly

void GroupedObject::addObject(Object *obj)
{
    arr.push_back(obj);
}
void GroupedObject::move(float x, float y, float z)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        arr[i]->move(x, y, z);
    }
}
CollisionReturn GroupedObject::getMinCollisionDistance(Ray r)
{
    CollisionReturn minimum;
    minimum.color = sf::Color::Black;
    minimum.distance = r.length * 2;
    for (int i = 0; i < arr.size(); ++i)
    {
        CollisionReturn val = arr[i]->getMinCollisionDistance(r);
        if (val.distance < minimum.distance)
        {
            minimum = val;
        }
    }
    return minimum;
}
void GroupedObject::collectTriangles(std::vector<GPUTriangle> &out)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        arr[i]->collectTriangles(out);
    }
}
GroupedObject::~GroupedObject()
{
    for (int i = 0; i < arr.size(); ++i)
    {
        delete arr[i];
    }
    arr.clear();
}
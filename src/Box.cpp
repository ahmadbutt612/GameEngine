#include "Box.h"

Box::Box()
{
    for (int i=0; i<6; ++i)
    {
        Point p = Point(0.f, 0.f, 0.f);
        faces[i].setPoints(p, p, p, p);
        faces[i].setColor(sf::Color::Black);
    }
}

Box::Box(Point org, float l, float w, float h, sf::Color col)
{
    for (int i=0; i<6; ++i)
    {
        faces[i].setColor(col);
    }
    faces[0].setPoints(org, Point(org.x + l, org.y, org.z), Point(org.x, org.y + w, org.z), Point(org.x + l, org.y + w, org.z)); //Base
    faces[1].setPoints(org, Point(org.x, org.y, org.z + h), Point(org.x, org.y + w, org.z), Point(org.x, org.y + w, org.z + h)); //Side 1
    faces[2].setPoints(org, Point(org.x, org.y, org.z + h), Point(org.x + l, org.y, org.z), Point(org.x + l, org.y, org.z + h)); // Side 2
    faces[3].setPoints(Point(org.x + l, org.y, org.z), Point(org.x + l, org.y, org.z + h), Point(org.x + l, org.y + w, org.z), Point(org.x + l, org.y + w, org.z + h)); //Side 3
    faces[4].setPoints(Point(org.x + l, org.y + w, org.z), Point(org.x + l, org.y + w, org.z + h), Point(org.x, org.y + w, org.z), Point(org.x, org.y + w, org.z + h)); //Side 4
    faces[5].setPoints(Point(org.x, org.y, org.z + h), Point(org.x + l, org.y, org.z + h), Point(org.x, org.y + w, org.z + h), Point(org.x + l, org.y + w, org.z + h)); //Top
}

void Box::move(float x, float y, float z)
{
    for (int i=0; i<6; ++i)
    {
        faces[i].move(x, y, z);
    }
}

CollisionReturn Box::getMinCollisionDistance(Ray r)
{
    CollisionReturn minimum;
    minimum.color = sf::Color::Black;
    minimum.distance = r.length * 2;
    for (int i = 0; i < 6; ++i)
    {
        CollisionReturn val = faces[i].getMinCollisionDistance(r);
        if (val.distance < minimum.distance)
        {
            minimum = val;
        }
    }
    return minimum;
}

void Box::collectTriangles(std::vector<GPUTriangle> &out)
{
    for (int i=0; i<6; ++i)
    {
        faces[i].collectTriangles(out);
    }
}

Box::~Box()
{
}

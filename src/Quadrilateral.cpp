#include "Quadrilateral.h"

Quadrilateral::Quadrilateral()
{
    t1.p1 = Point(0.f, 0.f, 0.f);
    t1.p2 = Point(0.f, 0.f, 0.f);
    t1.p3 = Point(0.f, 0.f, 0.f);
    t1.color = sf::Color::Black;
    t2.p1 = Point(0.f, 0.f, 0.f);
    t2.p2 = Point(0.f, 0.f, 0.f);
    t2.p3 = Point(0.f, 0.f, 0.f);
    t2.color = sf::Color::Black;
}

Quadrilateral::Quadrilateral(Point a, Point b, Point c, Point d, sf::Color col)
{
    t1.p1 = a;
    t1.p2 = b;
    t1.p3 = c;
    t1.color = col;
    t2.p1 = b;
    t2.p2 = c;
    t2.p3 = d;
    t2.color = col;
}

void Quadrilateral::move(float x, float y, float z)
{
    t1.move(x, y, z);
    t2.move(x, y, z);
}

CollisionReturn Quadrilateral::getMinCollisionDistance(Ray r)
{
    CollisionReturn c1 = t1.getMinCollisionDistance(r);
    CollisionReturn c2 = t2.getMinCollisionDistance(r);
    if (c1.distance < c2.distance)
        return c1;
    else
        return c2;
}

void Quadrilateral::collectTriangles(std::vector<GPUTriangle> &out)
{
    t1.collectTriangles(out);
    t2.collectTriangles(out);
}

void Quadrilateral::setPoints(Point a, Point b, Point c, Point d)
{
    t1.p1 = a;
    t1.p2 = b;
    t1.p3 = c;
    t2.p1 = b;
    t2.p2 = c;
    t2.p3 = d;
}

void Quadrilateral::setColor(sf::Color col)
{
    t1.color = col;
    t2.color = col;
}

Quadrilateral::~Quadrilateral()
{

}

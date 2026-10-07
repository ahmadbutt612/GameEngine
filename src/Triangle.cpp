#include "Triangle.h"
#include <cmath>

void Triangle::move(float x, float y, float z)
{
    p1.x = p1.x + x;
    p2.x = p2.x + x;
    p3.x = p3.x + x;
    p1.y = p1.y + y;
    p2.y = p2.y + y;
    p3.y = p3.y + y;
    p1.z = p1.z + z;
    p2.z = p2.z + z;
    p3.z = p3.z + z;
}
Triangle::Triangle(Point a, Point b, Point c, sf::Color col) : p1(a), p2(b), p3(c), color(col) {}
CollisionReturn Triangle::getMinCollisionDistance(Ray r)
{
    CollisionReturn cr;
    cr.color = color;
    float theta = r.angle.theta * 0.0174533;
    float fi = r.angle.fi * 0.0174533;
    Point vectorD;
    vectorD.x = sin(fi) * cos(theta);
    vectorD.y = sin(fi) * sin(theta);
    vectorD.z = cos(fi);
    Point vectorE1 = p2 - p1;
    Point vectorE2 = p3 - p1;
    Point vectorP;
    vectorP.x = vectorD.y * vectorE2.z - vectorD.z * vectorE2.y;
    vectorP.y = vectorD.z * vectorE2.x - vectorD.x * vectorE2.z;
    vectorP.z = vectorD.x * vectorE2.y - vectorD.y * vectorE2.x;
    float det = vectorE1.x * vectorP.x + vectorE1.y * vectorP.y + vectorE1.z * vectorP.z;
    if (abs(det) < 0.000001)
    {
        cr.distance = r.length * 2;
        cr.color = sf::Color::Black;
        return cr;
    }
    Point T = r.origin - p1;
    float u = (T.x * vectorP.x + T.y * vectorP.y + T.z * vectorP.z) / det;
    if (u < 0 || u > 1)
    {
        cr.distance = r.length * 2;
        cr.color = sf::Color::Black;
        return cr;
    }
    Point vectorQ;
    vectorQ.x = T.y * vectorE1.z - T.z * vectorE1.y;
    vectorQ.y = T.z * vectorE1.x - T.x * vectorE1.z;
    vectorQ.z = T.x * vectorE1.y - T.y * vectorE1.x;
    float v = (vectorD.x * vectorQ.x + vectorD.y * vectorQ.y + vectorD.z * vectorQ.z) / det;
    if (v < 0 || u + v > 1)
    {
        cr.distance = r.length * 2;
        cr.color = sf::Color::Black;
        return cr;
    }
    cr.distance = (vectorE2.x * vectorQ.x + vectorE2.y * vectorQ.y + vectorE2.z * vectorQ.z) / det;
    return cr;
}
void Triangle::collectTriangles(std::vector<GPUTriangle> &out)
{
    out.push_back({{p1.x, p1.y, p1.z, 0.f},
                   {p2.x, p2.y, p2.z, 0.f},
                   {p3.x, p3.y, p3.z, 0.f},
                   {color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.f}});
}
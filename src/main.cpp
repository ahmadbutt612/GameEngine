#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>
#include <iostream>

const unsigned int screenWidth = 1280;
const unsigned int screenHeight = 720;

struct Point
{
    float x;
    float y;
    float z;
    Point() : x(0), y(0), z(0) {}
    Point(float a, float b, float c) : x(a), y(b), z(c) {}
    Point operator-(const Point& other) const
    {
        return Point{this->x - other.x, this->y - other.y, this->z - other.z};
    }
};
struct Angle
{
    float theta;
    float fi;
};
struct Ray
{
    Point origin;
    Angle angle;
    float length;
};

struct CollisionReturn
{
    float distance;
    sf::Color color;
};
class Object
{
public:
    virtual void move(float x, float y, float z) = 0;
    virtual CollisionReturn getMinCollisionDistance(Ray r) = 0;
};

struct Triangle : public Object
{
    Point p1;
    Point p2;
    Point p3;
    sf::Color color;
    void move(float x, float y, float z)
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
    Triangle(Point a, Point b, Point c, sf::Color col) : p1(a), p2(b), p3(c), color(col) {}
    CollisionReturn getMinCollisionDistance(Ray r)
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
        vectorP.x = vectorD.y*vectorE2.z - vectorD.z*vectorE2.y;
        vectorP.y = vectorD.z*vectorE2.x - vectorD.x*vectorE2.z;
        vectorP.z = vectorD.x*vectorE2.y - vectorD.y*vectorE2.x;
        float det = vectorE1.x*vectorP.x + vectorE1.y*vectorP.y + vectorE1.z*vectorP.z;
        if (abs(det) < 0.000001)
        {
            cr.distance = r.length*2;
            cr.color = sf::Color::Black;
            return cr;
        }
        Point T = r.origin - p1;
        float u = (T.x*vectorP.x + T.y*vectorP.y + T.z*vectorP.z) / det;
        if (u < 0 || u > 1)
        {
            cr.distance = r.length*2;
            cr.color = sf::Color::Black;
            return cr;
        }
        Point vectorQ;
        vectorQ.x = T.y*vectorE1.z - T.z*vectorE1.y;
        vectorQ.y = T.z*vectorE1.x - T.x*vectorE1.z;
        vectorQ.z = T.x*vectorE1.y - T.y*vectorE1.x;
        float v = (vectorD.x*vectorQ.x + vectorD.y*vectorQ.y + vectorD.z*vectorQ.z) / det;
        if (v<0 || u+v>1)
        {
            cr.distance = r.length*2;
            cr.color = sf::Color::Black;
            return cr;
        }
        cr.distance = (vectorE2.x*vectorQ.x + vectorE2.y*vectorQ.y + vectorE2.z*vectorQ.z) / det;
        return cr;
    }
};

class GroupedObject : public Object
{
private:
    std::vector<Object *> arr;
public:
    void addObject(Object *obj)
    {
        arr.push_back(obj);
    }
    void move(float x, float y, float z)
    {
        for (int i = 0; i < arr.size(); ++i)
        {
            arr[i]->move(x, y, z);
        }
    }
    CollisionReturn getMinCollisionDistance(Ray r)
    {
        CollisionReturn minimum;
        minimum.color = sf::Color::Black;
        minimum.distance = r.length*2;
        for (int i = 0; i< arr.size(); ++i)
        {
            CollisionReturn val = arr[i] -> getMinCollisionDistance(r);
            if (val.distance < minimum.distance)
            {
                minimum = val;
            }
        }
        return minimum;
    }
    ~GroupedObject()
    {
        for (int i = 0; i < arr.size(); ++i)
        {
            delete arr[i];
        }
        arr.clear();
    }
};

class Camera
{
private:
    Point position;
    Ray topLeft;
    Ray topRight;
    Ray bottomLeft;
    Ray bottomRight;
    float viewDistance;
public:
    Camera()
    {
        viewDistance = 50;
        position = Point(0, 0, 0);
        topLeft = Ray(position, Angle(0, 0), viewDistance);
        topRight = Ray(position, Angle(0, 0), viewDistance);
        bottomLeft = Ray(position, Angle(0, 0), viewDistance);
        bottomRight = Ray(position, Angle(0, 0), viewDistance);
    }
    void setPosition(float a, float b, float c)
    {
        position.x = a;
        position.y = b;
        position.z = c;
        topLeft.origin = Point(a, b, c);
        topRight.origin = Point(a, b, c);
        bottomLeft.origin = Point(a, b, c);
        bottomRight.origin = Point(a, b, c);
    }
    void setViewDistance(float d)
    {
        if (d < 1)
            d = 1;
        viewDistance = d;
    }
    void setTopLeft(float t, float f)
    {
        topLeft.angle.theta = t;
        topLeft.angle.fi = f;
    }
    void setTopRight(float t, float f)
    {
        topRight.angle.theta = t;
        topRight.angle.fi = f;
    }
    void setBottomLeft(float t, float f)
    {
        bottomLeft.angle.theta = t;
        bottomLeft.angle.fi = f;
    }
    void setBottomRight(float t, float f)
    {
        bottomRight.angle.theta = t;
        bottomRight.angle.fi = f;
    }
    Point getPosition()
    {
        return position;
    }
    Ray getBottomLeft()
    {
        return bottomLeft;
    }
    Ray getBottomRight()
    {
        return bottomRight;
    }
    Ray getTopRight()
    {
        return topRight;
    }
    Ray getTopLeft()
    {
        return topLeft;
    }
    void changeDirection(float thetaOffset, float fiOffset)
    {
        topRight.angle.theta = topRight.angle.theta + thetaOffset;
        topLeft.angle.theta = topLeft.angle.theta + thetaOffset;
        bottomRight.angle.theta = bottomRight.angle.theta + thetaOffset;
        bottomLeft.angle.theta = bottomLeft.angle.theta + thetaOffset;
        topRight.angle.fi = topRight.angle.fi + fiOffset;
        topLeft.angle.fi = topLeft.angle.fi + fiOffset;
        bottomRight.angle.fi = bottomRight.angle.fi + fiOffset;
        bottomLeft.angle.fi = bottomLeft.angle.fi + fiOffset;
    }
};

int getPixelIndex(int x, int y)
{
    return y * screenWidth + x;
}









// Flat quad = 2 triangles
static void addQuad(GroupedObject *g, Point a, Point b, Point c, Point d, sf::Color col)
{
    g->addObject(new Triangle(a, b, c, col));
    g->addObject(new Triangle(a, c, d, col));
}

// Axis-aligned box = 6 quads = 12 triangles, each face its own color
static void addBox(GroupedObject *g,
                   float x0, float y0, float z0,
                   float x1, float y1, float z1,
                   sf::Color front, sf::Color back, sf::Color left,
                   sf::Color right, sf::Color top, sf::Color bottom)
{
    addQuad(g, Point(x0,y0,z0), Point(x1,y0,z0), Point(x1,y1,z0), Point(x0,y1,z0), front);
    addQuad(g, Point(x0,y0,z1), Point(x1,y0,z1), Point(x1,y1,z1), Point(x0,y1,z1), back);
    addQuad(g, Point(x0,y0,z0), Point(x0,y0,z1), Point(x0,y1,z1), Point(x0,y1,z0), left);
    addQuad(g, Point(x1,y0,z0), Point(x1,y0,z1), Point(x1,y1,z1), Point(x1,y1,z0), right);
    addQuad(g, Point(x0,y0,z0), Point(x1,y0,z0), Point(x1,y0,z1), Point(x0,y0,z1), top);
    addQuad(g, Point(x0,y1,z0), Point(x1,y1,z0), Point(x1,y1,z1), Point(x0,y1,z1), bottom);
}

// Cone as a triangle fan (side triangles alternate between two colors)
static void addCone(GroupedObject *g, float cx, float cy, float cz, float r,
                    float apexY, int segments, sf::Color colA, sf::Color colB, sf::Color baseCol)
{
    const float TWO_PI = 6.28318530718f;
    for (int i = 0; i < segments; i++)
    {
        float a0 = TWO_PI * i / segments;
        float a1 = TWO_PI * (i + 1) / segments;
        Point p0(cx + r * std::cos(a0), cy, cz + r * std::sin(a0));
        Point p1(cx + r * std::cos(a1), cy, cz + r * std::sin(a1));
        g->addObject(new Triangle(p0, p1, Point(cx, apexY, cz), (i % 2 == 0) ? colA : colB));
        g->addObject(new Triangle(p0, p1, Point(cx, cy, cz), baseCol)); // base cap
    }
}

// ---------- scene ----------

GroupedObject *buildScene()
{
    GroupedObject *allObjects = new GroupedObject();

    // 0) Your original object (x 10-20, y 10-20)
    GroupedObject *obj1 = new GroupedObject();
    obj1->addObject(new Triangle(Point(10, 10, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Red));
    obj1->addObject(new Triangle(Point(20, 20, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Blue));
    GroupedObject *obj2 = new GroupedObject();
    obj2->addObject(obj1);
    obj2->addObject(new Triangle(Point(10, 10, 15), Point(10, 20, 15), Point(20, 10, 15), sf::Color::Yellow));
    allObjects->addObject(obj2);

    // 1) Cube (x 30-36, y 10-16, z 10-16)
    GroupedObject *cube = new GroupedObject();
    addBox(cube, 30, 10, 10, 36, 16, 16,
           sf::Color::Red, sf::Color::Green, sf::Color::Blue,
           sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan);
    allObjects->addObject(cube);

    // 2) Square pyramid (base x 45-53, z 10-18, apex up at y 10)
    GroupedObject *pyramid = new GroupedObject();
    {
        Point b0(45, 20, 10), b1(53, 20, 10), b2(53, 20, 18), b3(45, 20, 18);
        Point apex(49, 10, 14);
        pyramid->addObject(new Triangle(b0, b1, apex, sf::Color(230, 60, 60)));
        pyramid->addObject(new Triangle(b1, b2, apex, sf::Color(240, 160, 40)));
        pyramid->addObject(new Triangle(b2, b3, apex, sf::Color(240, 220, 60)));
        pyramid->addObject(new Triangle(b3, b0, apex, sf::Color(90, 190, 90)));
        addQuad(pyramid, b0, b1, b2, b3, sf::Color(120, 120, 120)); // base
    }
    allObjects->addObject(pyramid);

    // 3) Octahedron (center 70,15,13, radius 4)
    GroupedObject *octa = new GroupedObject();
    {
        Point T(70, 11, 13), B(70, 19, 13);
        Point L(66, 15, 13), R(74, 15, 13), F(70, 15, 9), K(70, 15, 17);
        octa->addObject(new Triangle(T, L, F, sf::Color::Red));
        octa->addObject(new Triangle(T, F, R, sf::Color::Green));
        octa->addObject(new Triangle(T, R, K, sf::Color::Blue));
        octa->addObject(new Triangle(T, K, L, sf::Color::Yellow));
        octa->addObject(new Triangle(B, L, F, sf::Color::Cyan));
        octa->addObject(new Triangle(B, F, R, sf::Color::Magenta));
        octa->addObject(new Triangle(B, R, K, sf::Color(255, 140, 0)));
        octa->addObject(new Triangle(B, K, L, sf::Color::White));
    }
    allObjects->addObject(octa);

    // 4) House (body x 10-18, y 34-40, z 10-16, roof ridge at y 29)
    GroupedObject *house = new GroupedObject();
    {
        sf::Color wall(220, 200, 160), wallSide(190, 170, 130), roof(170, 50, 50);
        // walls
        addQuad(house, Point(10,34,10), Point(18,34,10), Point(18,40,10), Point(10,40,10), wall);      // front
        addQuad(house, Point(10,34,16), Point(18,34,16), Point(18,40,16), Point(10,40,16), wall);      // back
        addQuad(house, Point(10,34,10), Point(10,34,16), Point(10,40,16), Point(10,40,10), wallSide);  // left
        addQuad(house, Point(18,34,10), Point(18,34,16), Point(18,40,16), Point(18,40,10), wallSide);  // right
        // gables
        house->addObject(new Triangle(Point(10,34,10), Point(18,34,10), Point(14,29,10), wall));
        house->addObject(new Triangle(Point(10,34,16), Point(18,34,16), Point(14,29,16), wall));
        // roof slopes
        addQuad(house, Point(10,34,10), Point(14,29,10), Point(14,29,16), Point(10,34,16), roof);
        addQuad(house, Point(18,34,10), Point(14,29,10), Point(14,29,16), Point(18,34,16), sf::Color(140, 40, 40));
        // door (slightly in front of the wall to avoid z-fighting)
        addQuad(house, Point(13,36,9.9f), Point(15,36,9.9f), Point(15,40,9.9f), Point(13,40,9.9f), sf::Color(90, 55, 30));
    }
    allObjects->addObject(house);

    // 5) Tree (trunk + cone foliage), around x 31-39
    GroupedObject *tree = new GroupedObject();
    {
        sf::Color brown(110, 70, 30), brownDark(85, 55, 25);
        addBox(tree, 34, 38, 12, 36, 43, 14, brown, brown, brownDark, brownDark, brown, brownDark);
        addCone(tree, 35, 38, 13, 4.5f, 28, 8, sf::Color(30, 140, 50), sf::Color(20, 110, 40), sf::Color(15, 90, 30));
    }
    allObjects->addObject(tree);

    // 6) Staircase (4 steps going up in +x, x 50-62)
    GroupedObject *stairs = new GroupedObject();
    for (int i = 0; i < 4; i++)
    {
        float x0 = 50 + 3 * i, x1 = x0 + 3;
        float y0 = 40 - 2 * (i + 1), y1 = 40;
        sf::Color c = (i % 2 == 0) ? sf::Color(180, 180, 190) : sf::Color(140, 140, 160);
        addBox(stairs, x0, y0, 10, x1, y1, 15, c, c, sf::Color(110,110,130), sf::Color(110,110,130), sf::Color::White, c);
    }
    allObjects->addObject(stairs);

    return allObjects;
}

GroupedObject* getRectangle(Point origin, float length, float width);
GroupedObject* initWorld();

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    sf::RenderWindow window(
        sf::VideoMode({screenWidth, screenHeight}),
        "3D Game Engine",
        sf::Style::Default,
        sf::State::Windowed,
        contextSettings);
    window.setVerticalSyncEnabled(true);

    if (!window.setActive(true))
    {
        std::cerr << "Failed to activate window OpenGL context!" << std::endl;
        return 1;
    }

    sf::VertexArray points(sf::PrimitiveType::Points, screenWidth * screenHeight);
    for (int i = 0; i < screenHeight; ++i)
    {
        for (int j = 0; j < screenWidth; ++j)
        {
            points[i * screenWidth + j].position.x = j;
            points[i * screenWidth + j].position.y = i;
            points[i * screenWidth + j].color = sf::Color::Black;
        }
    }

    Camera camera;
    camera.setPosition(0, 0, 0);
    camera.setViewDistance(100);
    camera.setTopLeft(82, 50);
    camera.setTopRight(1, 50);
    camera.setBottomLeft(82, 90);
    camera.setBottomRight(1, 90);

    // GroupedObject *obj1 = new GroupedObject();
    // obj1->addObject(new Triangle(Point(10, 10, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Red));
    // obj1->addObject(new Triangle(Point(20, 20, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Blue));
    // GroupedObject *obj2 = new GroupedObject();
    // obj2->addObject(obj1);
    // obj2->addObject(new Triangle(Point(10, 10, 15), Point(10, 20, 15), Point(20, 10, 15), sf::Color::Yellow));

    GroupedObject* objects = buildScene();
    int frameCount = 0;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        Ray start = camera.getTopRight();
        float thetaOffset = (camera.getTopLeft().angle.theta - camera.getTopRight().angle.theta)/screenWidth;
        float fiOffset = (camera.getBottomLeft().angle.fi - camera.getTopLeft().angle.fi)/screenHeight;
        for (int i = 0; i < screenHeight; ++i)
        {  
            for (int j = 0; j < screenWidth; ++j)
            {
                CollisionReturn ret = objects->getMinCollisionDistance(start);
                points[i * screenWidth + j].color = ret.color;
                start.angle.theta = start.angle.theta + thetaOffset;
            }
            start.angle.fi = start.angle.fi + fiOffset;
            start.angle.theta = camera.getTopRight().angle.theta;
        }
        camera.changeDirection(1.f, 0);
        camera.setPosition(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z+0.1);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.draw(points);
        window.display();
       std::cout << ++frameCount << std::endl;
    }

    delete objects;

    return 0;
}

// GroupedObject *getBox(Point origin, float length, float width, float height)
// {
//     GroupedObject o1;
//     //o1.addObject(new Triangle());
//     return ;
// }

// GroupedObject *initWorld()
// {
//     GroupedObject allObjects;
//     allObjects.addObject();
// }

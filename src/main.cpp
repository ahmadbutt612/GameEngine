#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "types.h"
#include "Camera.h"
#include "Object.h"
#include "Triangle.h"
#include "GroupedObject.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

const unsigned int screenWidth = 1280;
const unsigned int screenHeight = 720;

std::string loadFile(const char *path)
{
    std::ifstream f(path);
    if (!f)
    {
        std::cerr << "Cannot open " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}
GLuint makeComputeProgram(const char *path)
{
    std::string src = loadFile(path);
    const char *c = src.c_str();
    GLuint sh = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(sh, 1, &c, nullptr);
    glCompileShader(sh);
    GLint ok;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[4096];
        glGetShaderInfoLog(sh, 4096, nullptr, log);
        std::cerr << log << "\n";
    }
    GLuint prog = glCreateProgram();
    glAttachShader(prog, sh);
    glLinkProgram(prog);
    glDeleteShader(sh);
    return prog;
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
    addQuad(g, Point(x0, y0, z0), Point(x1, y0, z0), Point(x1, y1, z0), Point(x0, y1, z0), front);
    addQuad(g, Point(x0, y0, z1), Point(x1, y0, z1), Point(x1, y1, z1), Point(x0, y1, z1), back);
    addQuad(g, Point(x0, y0, z0), Point(x0, y0, z1), Point(x0, y1, z1), Point(x0, y1, z0), left);
    addQuad(g, Point(x1, y0, z0), Point(x1, y0, z1), Point(x1, y1, z1), Point(x1, y1, z0), right);
    addQuad(g, Point(x0, y0, z0), Point(x1, y0, z0), Point(x1, y0, z1), Point(x0, y0, z1), top);
    addQuad(g, Point(x0, y1, z0), Point(x1, y1, z0), Point(x1, y1, z1), Point(x0, y1, z1), bottom);
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

GroupedObject *initWorld()
{
    GroupedObject *allObjects = new GroupedObject();

    // // 0) Your original object (x 10-20, y 10-20)
    // GroupedObject *obj1 = new GroupedObject();
    // obj1->addObject(new Triangle(Point(10, 10, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Red));
    // obj1->addObject(new Triangle(Point(20, 20, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Blue));
    // GroupedObject *allObjects = new GroupedObject();
    // allObjects->addObject(obj1);
    // allObjects->addObject(new Triangle(Point(10, 10, 15), Point(10, 20, 15), Point(20, 10, 15), sf::Color::Yellow));
    // allObjects->addObject(allObjects);

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
        addQuad(house, Point(10, 34, 10), Point(18, 34, 10), Point(18, 40, 10), Point(10, 40, 10), wall);     // front
        addQuad(house, Point(10, 34, 16), Point(18, 34, 16), Point(18, 40, 16), Point(10, 40, 16), wall);     // back
        addQuad(house, Point(10, 34, 10), Point(10, 34, 16), Point(10, 40, 16), Point(10, 40, 10), wallSide); // left
        addQuad(house, Point(18, 34, 10), Point(18, 34, 16), Point(18, 40, 16), Point(18, 40, 10), wallSide); // right
        // gables
        house->addObject(new Triangle(Point(10, 34, 10), Point(18, 34, 10), Point(14, 29, 10), wall));
        house->addObject(new Triangle(Point(10, 34, 16), Point(18, 34, 16), Point(14, 29, 16), wall));
        // roof slopes
        addQuad(house, Point(10, 34, 10), Point(14, 29, 10), Point(14, 29, 16), Point(10, 34, 16), roof);
        addQuad(house, Point(18, 34, 10), Point(14, 29, 10), Point(14, 29, 16), Point(18, 34, 16), sf::Color(140, 40, 40));
        // door (slightly in front of the wall to avoid z-fighting)
        addQuad(house, Point(13, 36, 9.9f), Point(15, 36, 9.9f), Point(15, 40, 9.9f), Point(13, 40, 9.9f), sf::Color(90, 55, 30));
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
        addBox(stairs, x0, y0, 10, x1, y1, 15, c, c, sf::Color(110, 110, 130), sf::Color(110, 110, 130), sf::Color::White, c);
    }
    allObjects->addObject(stairs);

    return allObjects;
}

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 3;

    sf::RenderWindow window(
        sf::VideoMode({screenWidth, screenHeight}),
        "3D Game Engine",
        sf::Style::Default,
        sf::State::Windowed,
        contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    if (!window.setActive(true))
    {
        std::cerr << "Failed to activate window OpenGL context!" << std::endl;
        return 1;
    }
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction))
    {
        std::cerr << "Failed to load OpenGL functions\n";
        return 1;
    }

    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return 1;
    }
    sf::Texture texture(sf::Vector2u(screenWidth, screenHeight));
    sf::Sprite sprite(texture);
    GLuint program = makeComputeProgram("raytrace.comp");

    // sf::VertexArray points(sf::PrimitiveType::Points, screenWidth * screenHeight);
    // for (int i = 0; i < screenHeight; ++i)
    // {
    //     for (int j = 0; j < screenWidth; ++j)
    //     {
    //         points[i * screenWidth + j].position.x = j;
    //         points[i * screenWidth + j].position.y = i;
    //         points[i * screenWidth + j].color = sf::Color::Black;
    //     }
    // }

    Camera camera;
    camera.setPosition(0, 0, 5);
    camera.setViewDistance(300);
    camera.setTopLeft(92, 50);
    camera.setTopRight(12, 50);
    camera.setBottomLeft(92, 100);
    camera.setBottomRight(12, 100);

    GroupedObject *objects = initWorld();
    GroupedObject *floor = new GroupedObject();
    floor->addObject(new Triangle(Point(0, 0, 0), Point(100, 100, 0), Point(100, 0, 0), sf::Color::White));
    floor->addObject(new Triangle(Point(0, 0, 0), Point(100, 100, 0), Point(0, 100, 0), sf::Color::White));
    objects->addObject(floor);
    std::vector<GPUTriangle> tris;
    objects->collectTriangles(tris);

    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, tris.size() * sizeof(GPUTriangle), tris.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
    size_t gpuCapacity = tris.size();

    GLint locTriCount = glGetUniformLocation(program, "triCount");
    GLint locCamPos = glGetUniformLocation(program, "camPos");
    GLint locStart = glGetUniformLocation(program, "startAngle");
    GLint locStep = glGetUniformLocation(program, "angleStep");
    GLint locLen = glGetUniformLocation(program, "rayLength");

    int frameCount = 0;
    const float movementSpeed = 1.f;
    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    sf::Vector2i oldMousePos = sf::Mouse::getPosition(window);
    const float aimSensitivity = 0.2;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
            else if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButton->button == sf::Mouse::Button::Left)
                {
                    // Left mouse button clicked!
                    // Get position: mouseButton->position.x, mouseButton->position.y
                }
            }
        }
        float dir = camera.getDirection().angle.theta;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            camera.move(-sin(dir * 0.0174533) * movementSpeed, cos(dir * 0.0174533), 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            camera.move(sin(dir * 0.0174533) * movementSpeed, -cos(dir * 0.0174533), 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            camera.move(cos(dir * 0.0174533) * movementSpeed, sin(dir * 0.0174533), 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            camera.move(-cos(dir * 0.0174533) * movementSpeed, -sin(dir * 0.0174533), 0.f);
        }
        Ray start = camera.getTopRight();
        float thetaOffset = (camera.getTopLeft().angle.theta - camera.getTopRight().angle.theta) / screenWidth;
        float fiOffset = (camera.getBottomLeft().angle.fi - camera.getTopLeft().angle.fi) / screenHeight;

        tris.clear();
        objects->collectTriangles(tris);
        // 3. Upload it
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        if (tris.size() > gpuCapacity)
        {
            // scene grew: reallocate
            glBufferData(GL_SHADER_STORAGE_BUFFER, tris.size() * sizeof(GPUTriangle), tris.data(), GL_DYNAMIC_DRAW);
            gpuCapacity = tris.size();
        }
        else
        {
            // same size or smaller: overwrite in place
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, tris.size() * sizeof(GPUTriangle), tris.data());
        }
        Point cp = camera.getPosition();
        glUseProgram(program);
        glUniform1i(locTriCount, (GLint)tris.size());
        glUniform3f(locCamPos, cp.x, cp.y, cp.z);
        glUniform2f(locStart, start.angle.theta, start.angle.fi);
        glUniform2f(locStep, thetaOffset, fiOffset);
        glUniform1f(locLen, start.length);

        glBindImageTexture(0, texture.getNativeHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
        glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
        glUseProgram(0);
        // for (int i = 0; i < screenHeight; ++i)
        // {
        //     for (int j = 0; j < screenWidth; ++j)
        //     {
        //         CollisionReturn ret = objects->getMinCollisionDistance(start);
        //         points[i * screenWidth + j].color = ret.color;
        //         start.angle.theta = start.angle.theta + thetaOffset;
        //     }
        //     start.angle.fi = start.angle.fi + fiOffset;
        //     start.angle.theta = camera.getTopRight().angle.theta;
        // }
        // camera.changeDirection(1.f, 0);

        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2i mouseOffset = newMousePos - oldMousePos;
        camera.changeDirection(mouseOffset.x * aimSensitivity, mouseOffset.y * aimSensitivity);
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
        oldMousePos = sf::Vector2i(screenWidth / 2, screenHeight / 2);

        //objects->move(0.2, 0.2, 0);

        window.resetGLStates(); // SFML caches GL state, so tell it we touched it
        window.clear();
        window.draw(sprite);
        window.display();
        std::cout << ++frameCount << std::endl;
    }

    delete objects;
    glDeleteBuffers(1, &ssbo);
    glDeleteProgram(program);
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
//     GroupedObject *obj1 = new GroupedObject();
//     obj1->addObject(new Triangle(Point(10, 10, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Red));
//     obj1->addObject(new Triangle(Point(20, 20, 10), Point(10, 20, 10), Point(20, 10, 10), sf::Color::Blue));
// GroupedObject *floor = new GroupedObject();
// floor->addObject(new Triangle(Point(0, 0, 0), Point(100, 100, 0), Point(100, 0, 0), sf::Color::White));
// floor->addObject(new Triangle(Point(0, 0, 0), Point(100, 100, 0), Point(0, 100, 0), sf::Color::White));
//     GroupedObject *allObjects = new GroupedObject();
//     allObjects->addObject(obj1);
//     allObjects->addObject(new Triangle(Point(10, 10, 15), Point(10, 20, 15), Point(20, 10, 15), sf::Color::Yellow));
//     allObjects->addObject(floor);
//     return allObjects;
// }

#pragma once
#include <glad/glad.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Camera.h"
#include "GroupedObject.h"
#include "types.h"

class CameraRenderer
{
public:
    CameraRenderer(sf::Vector2u position, sf::Vector2u viewSize, const char *shaderPath);
    ~CameraRenderer();
    CameraRenderer(const CameraRenderer &) = delete;
    CameraRenderer &operator=(const CameraRenderer &) = delete;

    void setViewport(sf::Vector2u position, sf::Vector2u viewSize);
    void render(GroupedObject &scene, Camera &camera);
    void draw(sf::RenderTarget &target) { target.draw(sprite); }

private:
    void uploadScene(GroupedObject &scene);

    sf::Vector2u size;      // declared before texture and sprite on purpose
    sf::Texture texture;
    sf::Sprite sprite;
    GLuint program = 0;
    GLuint ssbo = 0;
    size_t gpuCapacity = 0;
    std::vector<GPUTriangle> tris;

    GLint locTriCount = -1;
    GLint locCamPos = -1;
    GLint locStart = -1;
    GLint locStep = -1;
    GLint locLen = -1;
};
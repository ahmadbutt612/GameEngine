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
    CameraRenderer(unsigned int width, unsigned int height, const char *shaderPath);
    ~CameraRenderer();
    CameraRenderer(const CameraRenderer &) = delete;            // owns GL handles, so no copying
    CameraRenderer &operator=(const CameraRenderer &) = delete;

    void render(GroupedObject &scene, Camera &camera);
    const sf::Texture &getTexture() const { return texture; }

private:
    void uploadScene(GroupedObject &scene);

    unsigned int width;
    unsigned int height;
    sf::Texture texture;
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
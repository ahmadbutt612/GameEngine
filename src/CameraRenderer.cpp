#include "CameraRenderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string loadFile(const char *path)
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

static GLuint makeComputeProgram(const char *path)
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
        std::cerr << "Shader compile error:\n"
                  << log << "\n";
    }
    GLuint prog = glCreateProgram();
    glAttachShader(prog, sh);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[4096];
        glGetProgramInfoLog(prog, 4096, nullptr, log);
        std::cerr << "Program link error:\n"
                  << log << "\n";
    }
    glDeleteShader(sh);
    return prog;
}

CameraRenderer::CameraRenderer(sf::Vector2u position, sf::Vector2u viewSize, const char *shaderPath)
    : size(viewSize), texture(viewSize), sprite(texture)
{
    sprite.setPosition(sf::Vector2f(position));

    program = makeComputeProgram(shaderPath);
    glGenBuffers(1, &ssbo);

    locTriCount = glGetUniformLocation(program, "triCount");
    locCamPos = glGetUniformLocation(program, "camPos");
    locStart = glGetUniformLocation(program, "startAngle");
    locStep = glGetUniformLocation(program, "angleStep");
    locLen = glGetUniformLocation(program, "rayLength");
}

CameraRenderer::~CameraRenderer()
{
    glDeleteBuffers(1, &ssbo);
    glDeleteProgram(program);
}

void CameraRenderer::uploadScene(GroupedObject &scene)
{
    tris.clear();
    scene.collectTriangles(tris);
    if (tris.empty())
        return;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    if (tris.size() > gpuCapacity)
    {
        // scene grew (or first upload): reallocate
        glBufferData(GL_SHADER_STORAGE_BUFFER, tris.size() * sizeof(GPUTriangle), tris.data(), GL_DYNAMIC_DRAW);
        gpuCapacity = tris.size();
    }
    else
    {
        // same size or smaller: overwrite in place
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, tris.size() * sizeof(GPUTriangle), tris.data());
    }
}

void CameraRenderer::setViewport(sf::Vector2u position, sf::Vector2u viewSize)
{
    if (viewSize != size)
    {
        if (!texture.resize(viewSize))
        {
            std::cerr << "Failed to resize render texture\n";
            return;
        }
        size = viewSize;
        sprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(size)));
    }
    sprite.setPosition(sf::Vector2f(position));
}

void CameraRenderer::render(GroupedObject &scene, Camera &camera)
{
    uploadScene(scene);

    Ray start = camera.getTopLeft();
    float thetaOffset = (camera.getTopRight().angle.theta - camera.getTopLeft().angle.theta) / size.x;
    float fiOffset = (camera.getBottomLeft().angle.fi - camera.getTopLeft().angle.fi) / size.y;
    Point cp = camera.getPosition();

    glUseProgram(program);
    glUniform1i(locTriCount, (GLint)tris.size());
    glUniform3f(locCamPos, cp.x, cp.y, cp.z);
    glUniform2f(locStart, start.angle.theta, start.angle.fi);
    glUniform2f(locStep, thetaOffset, fiOffset);
    glUniform1f(locLen, start.length);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
    glBindImageTexture(0, texture.getNativeHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glDispatchCompute((size.x + 15) / 16, (size.y + 15) / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
    glUseProgram(0);
}
#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

#include "glm.hpp"

#include "Image.hpp"

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct WindowParameters
{

    uint32_t width;
    uint32_t height;
    const char *title;

    WindowParameters(uint32_t width, uint32_t height, const char *title) : width(width), height(height), title(title) {}
};

std::vector<glm::vec3> smoothPath(std::vector<glm::vec3> track_points, float smooth_range, int smooth_points);
std::vector<glm::vec3> rightTrack(std::vector<glm::vec3> points, float offset);
std::vector<glm::vec3> leftTrack(std::vector<glm::vec3> points, float offset);
GLuint makeVAO(int vertex_count, const ShapeVertex* data_ptr);
GLuint makeTexture(const glimac::FilePath fp);
void SetUniforms(glm::mat4 MVMatrix, glm::mat4 projMatrix, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix);
void SetLightUniforms(glm::vec4 projected_light_dir, GLuint uLightDir_vs, GLuint uLightIntensity, GLuint uKd, GLuint uKs, float uShininess);

}

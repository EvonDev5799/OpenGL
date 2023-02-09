#pragma once

#include <vector>
#include <array>

#include "common.hpp"

namespace glimac
{

class Rail
{
    public:
    Rail(std::vector<glm::vec3> points, float radius);
    
    const ShapeVertex* getDataPointer() const;
    GLsizei getVertexCount() const;

    private:
    void build(std::vector<glm::vec3> points, float radius);
    void makeFace(std::array<glm::vec3, 4> square);
    void makeFaces(std::array<glm::vec3, 4> start,std::array<glm::vec3, 4> arrival);

    int m_vertex_count;
    std::vector<ShapeVertex> m_vertices;

};
    
} // namespace glimac

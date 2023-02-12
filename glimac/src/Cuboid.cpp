#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cuboid.hpp"

namespace glimac {

glm::vec3 axisVec(int index)
{
    if(index == 0)
        return glm::vec3(1.f, 0.f, 0.f);
    if(index == 1)
        return glm::vec3(0.f, 1.f, 0.f);
    if(index == 2)
        return glm::vec3(0.f, 0.f, 1.f);
}

void Cuboid::makeFace(std::array<glm::vec3, 4> square)
{
    glm::vec3 norm1 = glm::normalize(glm::cross(square[0]-square[1], square[1]-square[2]));
    glm::vec3 norm2 = glm::normalize(glm::cross(square[0]-square[2], square[2]-square[3]));

    m_Vertices.push_back({square[0], norm1, glm::vec2(1,0)});
    m_Vertices.push_back({square[1], norm1, glm::vec2(1,1)});
    m_Vertices.push_back({square[2], norm1, glm::vec2(0,1)});

    m_Vertices.push_back({square[0], norm2, glm::vec2(1,0)});
    m_Vertices.push_back({square[2], norm2, glm::vec2(0,1)});
    m_Vertices.push_back({square[3], norm2, glm::vec2(0,0)});
}

void Cuboid::build(glm::vec3 diagonal) {

    m_Vertices = std::vector<ShapeVertex>();

    glm::vec3 base1 = -.5f * diagonal; 
    glm::vec3 base2 = .5f * diagonal; 
    
    for(int axis(0); axis < 3; axis++) {
        
        std::array<glm::vec3, 4> square;
        square[0] = base1;
        square[1] = base1 + diagonal * axisVec((axis + 1)%3);
        square[2] = base1 + diagonal * (axisVec(axis) + axisVec((axis + 1)%3));
        square[3] = base1 + diagonal * axisVec(axis);
        makeFace(square);

        square[0] = base2;
        square[1] = base2 - diagonal * axisVec(axis);
        square[2] = base2 - diagonal * (axisVec(axis) + axisVec((axis + 1)%3));
        square[3] = base2 - diagonal * axisVec((axis + 1)%3);
        makeFace(square);
    }
    m_nVertexCount = m_Vertices.size();
}


Cuboid::Cuboid(glm::vec3 diagonal)
{
    build(diagonal);
}

const ShapeVertex* Cuboid::getDataPointer() const {
    return &m_Vertices[0];
}
    
// Renvoit le nombre de vertex
GLsizei Cuboid::getVertexCount() const {
    return m_nVertexCount;
}

}

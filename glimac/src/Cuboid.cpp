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

void Cuboid::build(glm::vec3 diagonal) {

    m_Vertices = std::vector<ShapeVertex>();

    glm::vec3 base_global = -.5f * diagonal; 
    
    for(int axis(0); axis < 3; axis++) {

        ShapeVertex vertex;
        glm::vec3 faceNormal = -axisVec((axis + 2)%3);
        glm::vec3 base_local = base_global;

        vertex.position = base_local;
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(0.f, 0.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * axisVec(axis);
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(1.f, 0.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * (axisVec(axis) + axisVec((axis + 1)%3));
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(1.f, 1.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local;
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(0.f, 0.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * axisVec((axis + 1)%3);
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(0.f, 1.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * (axisVec(axis) + axisVec((axis + 1)%3));
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(1.f, 1.f);
        m_Vertices.push_back(vertex);

        faceNormal = -faceNormal;
        base_local += diagonal * axisVec((axis + 2)%3);

        vertex.position = base_local;
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(0.f, 0.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * axisVec(axis);
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(1.f, 0.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * (axisVec(axis) + axisVec((axis + 1)%3));
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(1.f, 1.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local;
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(0.f, 0.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * axisVec((axis + 1)%3);
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(0.f, 1.f);
        m_Vertices.push_back(vertex);

        vertex.position = base_local + diagonal * (axisVec(axis) + axisVec((axis + 1)%3));
        vertex.normal = faceNormal;
        vertex.texCoords = glm::vec2(1.f, 1.f);
        m_Vertices.push_back(vertex);


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

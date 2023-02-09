#include "glimac/Rails.hpp"

namespace glimac
{
Rail::Rail(std::vector<glm::vec3> points, float radius)
{
    build(points, radius);
    m_vertex_count =  m_vertices.size();
}

const ShapeVertex* Rail::getDataPointer() const
{
    return &(m_vertices[0]);
}

GLsizei Rail::getVertexCount() const
{
    return m_vertex_count;
}

void Rail::makeFace(std::array<glm::vec3, 4> square)
{
    glm::vec3 norm1 = glm::normalize(glm::cross(square[0]-square[1], square[1]-square[2]));
    glm::vec3 norm2 = glm::normalize(glm::cross(square[0]-square[2], square[2]-square[3]));

    m_vertices.push_back({square[0], norm1, glm::vec2(1,0)});
    m_vertices.push_back({square[1], norm1, glm::vec2(1,1)});
    m_vertices.push_back({square[2], norm1, glm::vec2(0,1)});

    m_vertices.push_back({square[0], norm2, glm::vec2(1,0)});
    m_vertices.push_back({square[2], norm2, glm::vec2(0,1)});
    m_vertices.push_back({square[3], norm2, glm::vec2(0,0)});
}

void Rail::makeFaces(std::array<glm::vec3, 4> start,std::array<glm::vec3, 4> arrival)
{
    std::array<glm::vec3, 4> square;
    for (int i(0); i < 4; i++)
    {
        square[0] = start[(i+1)%4];
        square[1] = start[i];
        square[2] = arrival[i];
        square[3] = arrival[(i+1)%4];
        makeFace(square);
    }
}

void Rail::build(std::vector<glm::vec3> points, float radius)
{
    int n = points.size();
    std::array<glm::vec3, 4> oldsquare, square;
    glm::vec3 p1, p2, p3, dir1, dir2, side, local_up, base;
    glm::vec3 global_up(0.f, 1.f, 0.f);

    p1 = points[n-1];
    p2 = points[0];
    p3 = points[1];

    dir1 = glm::normalize(p2 - p1);
    dir2 = glm::normalize(p2 - p3);
    
    float angle = glm::acos(glm::dot(dir1, dir2))/2;

    side = glm::normalize(glm::cross(dir1, global_up));
    local_up = glm::cross(side, dir1);

    oldsquare[0] = p2 + side * radius + local_up*radius;
    oldsquare[1] = p2 - side * radius + local_up*radius;
    oldsquare[2] = p2 - side * radius - local_up*radius;
    oldsquare[3] = p2 + side * radius - local_up*radius;

    for (int i(0); i < n; i++)
    {
        p1 = points[i];
        p2 = points[(i+1)%n];
        p3 = points[(i+2)%n];

        dir1 = glm::normalize(p2 - p1);
        dir2 = glm::normalize(p2 - p3);
        
        angle = glm::acos(glm::dot(dir1, dir2))/2;
    
        side = glm::normalize(glm::cross(dir1, global_up));
        local_up = glm::cross(side, dir1);

        square[0] = p2 + side * radius + local_up*radius;
        square[1] = p2 - side * radius + local_up*radius;
        square[2] = p2 - side * radius - local_up*radius;
        square[3] = p2 + side * radius - local_up*radius;
        
        makeFaces(oldsquare, square);
        oldsquare = square;        
    }
}


    
} // namespace glimac

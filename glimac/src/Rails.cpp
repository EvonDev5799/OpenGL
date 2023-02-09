#include "glimac/Rails.hpp"

#include <iostream>

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
    std::array<glm::vec3, 4> oldsquare, square1, square2;
    glm::vec3 p1, p2, p3, dir1, dir2, side1, side2, up1, up2, base1, base2;
    glm::vec3 global_up(0.f, 1.f, 0.f);

    p1 = points[n-1];
    p2 = points[0];
    p3 = points[1];

    dir1 = glm::normalize(p2 - p1);
    dir2 = glm::normalize(p2 - p3);
    
    float angle = glm::acos(glm::dot(dir1, dir2))/2;
    std::cout << angle << std::endl;

    if (angle == 0)
    {
        side1 = glm::normalize(glm::cross(dir1, global_up));
        up1 = glm::cross(side1, dir1);

        oldsquare[0] = p2 + side1 * radius + up1*radius;
        oldsquare[1] = p2 - side1 * radius + up1*radius;
        oldsquare[2] = p2 - side1 * radius - up1*radius;
        oldsquare[3] = p2 + side1 * radius - up1*radius;
    }
    else
    {

        side2 = glm::normalize(glm::cross(global_up, dir2));
        up2 = glm::cross(dir2, side2);

        float offset = glm::tan(angle) * glm::sqrt(2.f) * radius;

        base2 = p2 - dir2*offset;

        oldsquare[0] = base2 + side2 * radius + up2*radius;
        oldsquare[1] = base2 - side2 * radius + up2*radius;
        oldsquare[2] = base2 - side2 * radius - up2*radius;
        oldsquare[3] = base2 + side2 * radius - up2*radius;
    }

    for (int i(0); i < n; i++)
    {
        p1 = points[i];
        p2 = points[(i+1)%n];
        p3 = points[(i+2)%n];

        dir1 = glm::normalize(p2 - p1);
        dir2 = glm::normalize(p2 - p3);
        
        angle = glm::acos(glm::dot(dir1, dir2))/2;
        std::cout << "angle" << angle << std::endl;

        if (angle == 0)
        {
            side1 = glm::normalize(glm::cross(dir1, global_up));
            up1 = glm::cross(side1, dir1);

            square1[0] = p2 + side1 * radius + up1*radius;
            square1[1] = p2 - side1 * radius + up1*radius;
            square1[2] = p2 - side1 * radius - up1*radius;
            square1[3] = p2 + side1 * radius - up1*radius;
            
            makeFaces(oldsquare, square1);
            oldsquare = square1;
        }
        else
        {
            side1 = glm::normalize(glm::cross(dir1, global_up));
            std::cout << "side1" << side1 << std::endl;
            up1 = glm::cross(side1, dir1);
            std::cout << "up1" << up1 << std::endl;

            side2 = glm::normalize(glm::cross(global_up, dir2));
            std::cout << "side2" << side2 << std::endl;
            up2 = glm::cross(dir2, side2);
            std::cout << "side2" << side2 << std::endl;

            float offset = glm::tan(angle) * glm::sqrt(2.f) * radius;
            std::cout << "offset" << offset << std::endl;

            base1 = p2 - dir1*offset;
            std::cout << "base1" << base1 << std::endl;
            base2 = p2 - dir2*offset;
            std::cout << "base2" << base2 << std::endl;

            square1[0] = base1 + side1 * radius + up1*radius;
            square1[1] = base1 - side1 * radius + up1*radius;
            square1[2] = base1 - side1 * radius - up1*radius;
            square1[3] = base1 + side1 * radius - up1*radius;

            square2[0] = base2 + side2 * radius + up2*radius;
            square2[1] = base2 - side2 * radius + up2*radius;
            square2[2] = base2 - side2 * radius - up2*radius;
            square2[3] = base2 + side2 * radius - up2*radius;
            

            makeFaces(oldsquare,square1);
            makeFaces(square1, square2);
            oldsquare = square2;
        }
    }
}


    
} // namespace glimac

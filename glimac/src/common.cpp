#include "glimac/common.hpp"

namespace glimac {

std::vector<glm::vec3> smoothPath(std::vector<glm::vec3> track_points, float smooth_range, int smooth_points)
{
    if(smooth_range > 0.5f || smooth_range < 0.f)
        std::cout << "smooth_range value out of [0 , 0.5]" << std::endl;

    std::vector<glm::vec3> r;

    int source_size = track_points.size();
    for(int i(0); i < source_size; i++ )
    {
        glm::vec3 prev = track_points[i];
        glm::vec3 center = track_points[(i + 1)%source_size];
        glm::vec3 next = track_points[(i + 2)%source_size];

        glm::vec3 toprev = prev - center;
        glm::vec3 tonext = next - center;

        glm::vec3 point1 = center  + smooth_range * toprev;
        glm::vec3 point2 = center;
        glm::vec3 point3 = center  + smooth_range * tonext;

        for(int j(0); j < smooth_points + 2; j++)
        {
            float t = ((float)j)/(smooth_points + 1);
            glm::vec3 intermediate1 = (1-t)*point1 + t*point2;
            glm::vec3 intermediate2 = (1-t)*point2 + t*point3;
            glm::vec3 final = (1-t)*intermediate1 + t*intermediate2;

            r.push_back(final);
        }

    }
    return r;
}

std::vector<glm::vec3> rightTrack(std::vector<glm::vec3> points, float offset)
{
    std::vector<glm::vec3> r;
    glm::vec3 a, b, c, dir1, dir2, side1, side2;
    glm::vec3 up(0.f, 1.f, 0.f);
    int n = points.size();

    for(int i(0); i < n; i++)
    {
        a = points[i];
        b = points[(i+1)%n];
        c = points[(i+2)%n];

        dir1 = glm::normalize(b-a);
        dir2 = glm::normalize(c-b);

        side1 = glm::normalize(glm::cross(dir1, up));
        side2 = glm::normalize(glm::cross(dir2, up));
        float curve = glm::dot(dir2, side1) - glm::dot(dir1, side1);
        if ( curve >= 0)
        {
            float frac1, frac2;
            frac1 = offset/(glm::dot(side2,-dir1));
            frac2 = offset/(glm::dot(side1,dir2));

            r.push_back(b - dir1*frac1 + dir2*frac2);
        }
        else
        {
            r.push_back(b + side1 * offset);
            r.push_back(b + side2 * offset);
        }
    }
    return r;
}

std::vector<glm::vec3> leftTrack(std::vector<glm::vec3> points, float offset)
{
    std::vector<glm::vec3> r;
    glm::vec3 a, b, c, dir1, dir2, side1, side2;
    glm::vec3 up(0.f, 1.f, 0.f);
    int n = points.size();

    for(int i(0); i < n; i++)
    {
        a = points[i];
        b = points[(i+1)%n];
        c = points[(i+2)%n];

        dir1 = glm::normalize(b-a);
        dir2 = glm::normalize(c-b);

        side1 = glm::normalize(glm::cross(up, dir1));
        side2 = glm::normalize(glm::cross(up, dir2));
        float curve = glm::dot(dir2, side1) - glm::dot(dir1, side1);
        if ( curve >= 0)
        {
            float frac1, frac2;
            frac1 = offset/(glm::dot(side2,-dir1));
            frac2 = offset/(glm::dot(side1,dir2));

            r.push_back(b - dir1*frac1 + dir2*frac2);
        }
        else
        {
            r.push_back(b + side1 * offset);
            r.push_back(b + side2 * offset);
        }
    }
    return r;
}

GLuint makeVAO(int vertex_count, const ShapeVertex* data_ptr)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex) * vertex_count, data_ptr, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEX_COORD = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEX_COORD);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEX_COORD, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void *)offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vao;
}

GLuint makeTexture(const glimac::FilePath fp)
{
    std::unique_ptr<Image> pTexture = loadImage(fp);
    if (pTexture == NULL)
    {
        std::cerr << "image not fount at " << fp << std::endl;
        return EXIT_FAILURE;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTexture->getWidth(), pTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, pTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void SetUniforms(glm::mat4 MVMatrix, glm::mat4 projMatrix, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix)
{
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));
}

void SetLightUniforms(glm::vec4 projected_light_dir, GLuint uLightDir_vs, GLuint uLightIntensity, GLuint uKd, GLuint uKs, float uShininess)
{
    glUniform3f(uLightDir_vs, projected_light_dir.x, projected_light_dir.y, projected_light_dir.z);
    glUniform3f(uLightIntensity, 1.f, 1.f, 1.f);
    glUniform3f(uKd, .5f,.5f,.5f);
    glUniform3f(uKs, .5f,.5f,.5f);
    glUniform1f(uShininess, 50.f);
}

}

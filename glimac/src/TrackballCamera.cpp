#include "glimac/TrackballCamera.hpp"

namespace glimac
{
    TrackballCamera::TrackballCamera() : m_fDistance(-5.f), m_fAngleX(0.f), m_fAngleY(0.f) {}
    
    void TrackballCamera::moveFront(float delta)
    {
        m_fDistance += delta;
    }

    void TrackballCamera::rotateLeft(float degrees)
    {
        m_fAngleX += glm::radians(degrees);
    }

    void TrackballCamera::rotateUp(float degrees)
    {
        m_fAngleY += glm::radians(degrees);
    }

    glm::mat4 TrackballCamera::getViewMatrix() const
    {
        glm::mat4 viewMatrix = glm::mat4(1.f);
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, m_fDistance));
        viewMatrix = glm::rotate(viewMatrix, m_fAngleY, glm::vec3(1.f, 0.f, 0.f));
        viewMatrix = glm::rotate(viewMatrix, m_fAngleX,glm::vec3(0.f, 1.f, 0.f));
        return viewMatrix;
    }

} // namespace glimac

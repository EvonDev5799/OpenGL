#pragma once

#include "common.hpp"

namespace glimac {

class TrackballCamera {

    public:
    TrackballCamera();

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;


    private:
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;
};

}
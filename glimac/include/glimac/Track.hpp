#pragma once

#include <vector>
#include "common.hpp"

namespace glimac
{
    
class Track
{
public:
    Track(std::vector<glm::vec3> track);

    glm::vec3 currentLocation();
    glm::vec3 currentDirection();
    glm::mat4 currentTransform();
    void advance(float distance);


private:
    std::vector<glm::vec3> track;
    std::vector<glm::vec3> paths;
    std::vector<float> lengths;
    float distance;
    int total_count;
    int index;

    void computeSegData();
    void nextIndex();
    void prevIndex();
};

} // namespace glimac

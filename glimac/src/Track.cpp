#include "glimac/Track.hpp"

namespace glimac
{
    void Track::computeSegData()
    {
        lengths = std::vector<float>(total_count) ;
        paths = std::vector<glm::vec3>(total_count);

        glm::vec3 start, arrival, path;
        float length;

        for(int i(0); i < (total_count - 1); i++)
        {
            start = track[i];
            arrival = track[i+1];
            path = arrival - start;
            length = glm::length(path);

            paths[i] = path;
            lengths[i] = length;
            total_length += length;
        }
        start = track[total_count-1];
        arrival = track[0];
        path = arrival - start;
        length = glm::length(path);

        paths[total_count-1] = path;
        lengths[total_count-1] = glm::length(path);
        total_length += length;
    }

    void Track::nextIndex()
    {
        index = (index + 1)%total_count;
    }

    void Track::prevIndex()
    {
        index = (index - 1 + total_count)%total_count;        
    }

    Track::Track(std::vector<glm::vec3> track) : track(track), distance(0.f), index(0), total_count(track.size()), total_length(0.f)
    {
        computeSegData();
    }

    void Track::advance(float amount)
    {
        distance += amount;

        while(distance > lengths[index])
        {
            distance -= lengths[index];
            nextIndex();
        }

        while(distance < 0.f)
        {
            prevIndex();
            distance += lengths[index];
        }
    }


    float Track::lenght()
    {
        return total_length;
    }

    glm::vec3 Track::currentLocation()
    {
        glm::vec3 start = track[index];
        glm::vec3 path = paths[index];
        float length = lengths[index];

        return( start + (path * distance)/length);
    }

    glm::vec3 Track::currentDirection()
    {
        return(glm::normalize(paths[index]));
    }

    glm::mat4 Track::currentTransform()
    {
        glm::vec3 global_up(0.f, 1.f, 0.f);
        glm::vec3 direction = currentDirection();
        std::cout << direction << std::endl;
        float verticalAngle = -glm::acos(glm::dot(direction, global_up)) - glm::pi<float>()/2;
        std::cout << verticalAngle << std::endl;

        glm::vec3 horizontalVec(direction.x, 0.f, direction.z);
        horizontalVec = glm::normalize(horizontalVec);
        std::cout << horizontalVec << std::endl;
        std::cout << glm::dot(horizontalVec, glm::vec3(1.f, 0.f, 0.f)) << std::endl;
        std::cout << glm::acos(glm::dot(horizontalVec, glm::vec3(1.f, 0.f, 0.f))) << std::endl;
        float sign = direction.z == 0 ? 1 : -glm::sign(direction.z);
        float horizontalAngle = sign * glm::acos(glm::dot(horizontalVec, glm::vec3(1.f, 0.f, 0.f)));
        std::cout << horizontalAngle << std::endl << std::endl;



        glm::mat4 transform = glm::mat4(1.f);
        transform = glm::translate(transform, currentLocation());
        transform = glm::rotate(transform, horizontalAngle, glm::vec3(0.f, 1.f, 0.f));
        transform = glm::rotate(transform, verticalAngle, glm::vec3(0.f, 0.f, 1.f));

        return transform;
    }
    
} // namespace glimac

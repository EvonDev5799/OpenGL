#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Cuboid {

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Cuboid(glm::vec3 diagonal);

    // Renvoit le pointeur vers les données
    const ShapeVertex* getDataPointer() const;
    
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const;

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets

    // Alloue et construit les données (implantation dans le .cpp)
    void build(glm::vec3 diagonal);
};
    
}
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Primitives.h"

class Mesh {

    glm::vec3 location;

public:

    int ID;
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<Polygon> faces;
    
    Mesh(std::string name) : name(name) {}
};

#endif
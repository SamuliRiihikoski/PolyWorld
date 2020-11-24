#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Primitives.h"

struct HEdge;
struct Face;

struct Vert {
    float position[3];
    HEdge* edge;

    Vert(float x, float y, float z) {
        position[0] = x;
        position[1] = y;
        position[2] = z;
    }
};

struct HEdge {
    Vert* vertex;
    Face* face;
    HEdge* next;
    HEdge* prev;
    HEdge* opposite;
};

struct Face {
    HEdge* edge;
};

class Mesh {

    glm::vec3 location;

public:

    int ID;
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<Polygon> faces;

    std::vector<Vert> VertexList;
    std::vector<Face> FaceList;
    std::vector<HEdge> HEdgeList;
    
    Mesh(std::string name) : name(name) {}
};

#endif
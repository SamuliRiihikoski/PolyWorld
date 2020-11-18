#ifndef PRIM_H
#define PRIM_H

#include <vector>


struct Vertex {
    float pos[3];

    Vertex(float x, float y, float z) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
    }
};

struct Polygon {
    int id;
    std::vector<unsigned int> verticesID;

    Polygon(int id) {
        id = id;
    }
};

#endif
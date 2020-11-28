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
    Vertex() {
        pos[0] = 0.0f;
        pos[1] = 0.0f;
        pos[2] = 0.0f;
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
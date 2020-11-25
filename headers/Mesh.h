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
        edge = nullptr;
    };
};

struct HEdge {
    Vert* vertex;
    Face* face;
    HEdge* next;
    HEdge* prev;
    HEdge* opposite;

    HEdge(Vert* vertex) {
        this->vertex = vertex;
        face = nullptr;
        next = nullptr;
        prev = nullptr;
        opposite = nullptr;
    };
};

struct Face {
    HEdge* edge;
    Face(HEdge* edge) {
        this->edge = edge;
    };
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
    
    Mesh(std::string name) : name(name) 
    {   
        
        // VERTEXS

        // FRONT
        VertexList.push_back(Vert(-1.0f, -1.0f, 1.0f)); // 0
        VertexList.push_back(Vert( 1.0f, -1.0f, 1.0f)); // 1
        VertexList.push_back(Vert(-1.0f,  1.0f, 1.0f)); // 2
        VertexList.push_back(Vert( 1.0f,  1.0f, 1.0f)); // 3

        // BACK
        VertexList.push_back(Vert(-1.0f,  1.0f, -1.0f)); // 4
        VertexList.push_back(Vert( 1.0f,  1.0f, -1.0f)); // 5
        VertexList.push_back(Vert(-1.0f, -1.0f, -1.0f)); // 6
        VertexList.push_back(Vert( 1.0f, -1.0f, -1.0f)); // 7
        
        // HALF-EDGES

        HEdgeList.push_back(HEdge(&VertexList[1])); // 0
        HEdgeList.push_back(HEdge(&VertexList[3])); // 1
        HEdgeList.push_back(HEdge(&VertexList[2])); // 2
        HEdgeList.push_back(HEdge(&VertexList[0])); // 3

        HEdgeList.push_back(HEdge(&VertexList[3])); // 4
        HEdgeList.push_back(HEdge(&VertexList[5])); // 5
        HEdgeList.push_back(HEdge(&VertexList[4])); // 6
        HEdgeList.push_back(HEdge(&VertexList[2])); // 7

        HEdgeList.push_back(HEdge(&VertexList[5])); // 8
        HEdgeList.push_back(HEdge(&VertexList[7])); // 9
        HEdgeList.push_back(HEdge(&VertexList[6])); // 10
        HEdgeList.push_back(HEdge(&VertexList[4])); // 11

        HEdgeList.push_back(HEdge(&VertexList[7])); // 12
        HEdgeList.push_back(HEdge(&VertexList[1])); // 13
        HEdgeList.push_back(HEdge(&VertexList[0])); // 14
        HEdgeList.push_back(HEdge(&VertexList[6])); // 15

        HEdgeList.push_back(HEdge(&VertexList[5])); // 16
        HEdgeList.push_back(HEdge(&VertexList[3])); // 17
        HEdgeList.push_back(HEdge(&VertexList[1])); // 18
        HEdgeList.push_back(HEdge(&VertexList[7])); // 19

        HEdgeList.push_back(HEdge(&VertexList[6])); // 20
        HEdgeList.push_back(HEdge(&VertexList[0])); // 21
        HEdgeList.push_back(HEdge(&VertexList[2])); // 22
        HEdgeList.push_back(HEdge(&VertexList[4])); // 23
        
        // HALF-EDGES NEXTS

        HEdgeList[0].next = &HEdgeList[1]; // 0
        HEdgeList[1].next = &HEdgeList[2]; // 1
        HEdgeList[2].next = &HEdgeList[3]; // 2
        HEdgeList[3].next = &HEdgeList[0]; // 3

        HEdgeList[4].next = &HEdgeList[5]; // 4
        HEdgeList[5].next = &HEdgeList[6]; // 5
        HEdgeList[6].next = &HEdgeList[7]; // 6
        HEdgeList[7].next = &HEdgeList[4]; // 7

        HEdgeList[8].next = &HEdgeList[9]; // 8
        HEdgeList[9].next = &HEdgeList[10]; // 9
        HEdgeList[10].next = &HEdgeList[11]; // 10
        HEdgeList[11].next = &HEdgeList[8]; // 11

        HEdgeList[12].next = &HEdgeList[13]; // 12
        HEdgeList[13].next = &HEdgeList[14]; // 13
        HEdgeList[14].next = &HEdgeList[15]; // 14
        HEdgeList[15].next = &HEdgeList[12]; // 15

        HEdgeList[16].next = &HEdgeList[17]; // 16
        HEdgeList[17].next = &HEdgeList[18]; // 17
        HEdgeList[18].next = &HEdgeList[19]; // 18
        HEdgeList[19].next = &HEdgeList[16]; // 19

        HEdgeList[20].next = &HEdgeList[21]; // 20
        HEdgeList[21].next = &HEdgeList[22]; // 21
        HEdgeList[22].next = &HEdgeList[23]; // 22
        HEdgeList[23].next = &HEdgeList[20]; // 23

        // HALF-EDGES PREVS

        HEdgeList[0].prev = &HEdgeList[3]; // 0
        HEdgeList[1].prev = &HEdgeList[0]; // 1
        HEdgeList[2].prev = &HEdgeList[1]; // 2
        HEdgeList[3].prev = &HEdgeList[2]; // 3

        HEdgeList[4].prev = &HEdgeList[7]; // 4
        HEdgeList[5].prev = &HEdgeList[4]; // 5
        HEdgeList[6].prev = &HEdgeList[5]; // 6
        HEdgeList[7].prev = &HEdgeList[6]; // 7

        HEdgeList[8].prev = &HEdgeList[11]; // 8
        HEdgeList[9].prev = &HEdgeList[8]; // 9
        HEdgeList[10].prev = &HEdgeList[9]; // 10
        HEdgeList[11].prev = &HEdgeList[10]; // 11

        HEdgeList[12].prev = &HEdgeList[15]; // 12
        HEdgeList[13].prev = &HEdgeList[12]; // 13
        HEdgeList[14].prev = &HEdgeList[13]; // 14
        HEdgeList[15].prev = &HEdgeList[14]; // 15

        HEdgeList[16].prev = &HEdgeList[19]; // 16
        HEdgeList[17].prev = &HEdgeList[16]; // 17
        HEdgeList[18].prev = &HEdgeList[17]; // 18
        HEdgeList[19].prev = &HEdgeList[18]; // 19

        HEdgeList[20].prev = &HEdgeList[23]; // 20
        HEdgeList[21].prev = &HEdgeList[20]; // 21
        HEdgeList[22].prev = &HEdgeList[21]; // 22
        HEdgeList[23].prev = &HEdgeList[22]; // 23

        // HALF-EDGES OPPOSITE

        HEdgeList[0].opposite = &HEdgeList[14]; // 0
        HEdgeList[1].opposite = &HEdgeList[18]; // 1
        HEdgeList[2].opposite = &HEdgeList[4]; // 2
        HEdgeList[3].opposite = &HEdgeList[22]; // 3

        HEdgeList[4].opposite = &HEdgeList[2]; // 4
        HEdgeList[5].opposite = &HEdgeList[17]; // 5
        HEdgeList[6].opposite = &HEdgeList[8]; // 6
        HEdgeList[7].opposite = &HEdgeList[23]; // 7

        HEdgeList[8].opposite = &HEdgeList[6]; // 8
        HEdgeList[9].opposite = &HEdgeList[16]; // 9
        HEdgeList[10].opposite = &HEdgeList[12]; // 10
        HEdgeList[11].opposite = &HEdgeList[20]; // 11

        HEdgeList[12].opposite = &HEdgeList[10]; // 12
        HEdgeList[13].opposite = &HEdgeList[19]; // 13
        HEdgeList[14].opposite = &HEdgeList[0]; // 14
        HEdgeList[15].opposite = &HEdgeList[21]; // 15

        HEdgeList[16].opposite = &HEdgeList[9]; // 16
        HEdgeList[17].opposite = &HEdgeList[5]; // 17
        HEdgeList[18].opposite = &HEdgeList[1]; // 18
        HEdgeList[19].opposite = &HEdgeList[13]; // 19

        HEdgeList[20].opposite = &HEdgeList[11]; // 20
        HEdgeList[21].opposite = &HEdgeList[15]; // 21
        HEdgeList[22].opposite = &HEdgeList[3]; // 22
        HEdgeList[23].opposite = &HEdgeList[7]; // 23

        // FACES
        
        FaceList.push_back(Face(&HEdgeList[0])); // 0
        FaceList.push_back(Face(&HEdgeList[4])); // 1
        FaceList.push_back(Face(&HEdgeList[8])); // 2
        FaceList.push_back(Face(&HEdgeList[12])); // 3
        FaceList.push_back(Face(&HEdgeList[16])); // 4
        FaceList.push_back(Face(&HEdgeList[20])); // 5
        
        // VERTEX HEDGE

        VertexList[0].edge = &HEdgeList[0]; // 0
        VertexList[1].edge = &HEdgeList[1]; // 1
        VertexList[2].edge = &HEdgeList[3]; // 2
        VertexList[3].edge = &HEdgeList[2]; // 3

        VertexList[4].edge = &HEdgeList[8]; // 4
        VertexList[5].edge = &HEdgeList[9]; // 5
        VertexList[6].edge = &HEdgeList[11]; // 6
        VertexList[7].edge = &HEdgeList[10]; // 7
    }
};

#endif
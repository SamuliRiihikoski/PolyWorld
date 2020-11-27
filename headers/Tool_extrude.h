#ifndef TOOL_EXTRUDE_H
#define TOOL_EXTRUDE_H

#include "Tool.h"
#include <iostream>
#include <cmath>

class Tool_extrude : public Tool 
{
    // Mesh TMesh; <- main container

    glm::vec3 initPos[4];
    double initXpos;
    double initYpos;
    float mouseDistance;

public:
    void LMB_Press();
    void RMB_Press();
    void Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh);
    void onMouseMove(double& xpos, double& ypos);
    Tool_extrude();
};

inline Tool_extrude::Tool_extrude() 
{
    for (int i = 0; i < 4; i++)
        TMesh.VertexList.push_back(Vert());

    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[1])); // 0
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[2])); // 0
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[3])); // 0
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[0])); // 0

    TMesh.FaceList.push_back(Face(&TMesh.HEdgeList[0]));

    TMesh.HEdgeList[0].next = &TMesh.HEdgeList[1];
    TMesh.HEdgeList[1].next = &TMesh.HEdgeList[2];
    TMesh.HEdgeList[2].next = &TMesh.HEdgeList[3];
    TMesh.HEdgeList[3].next = &TMesh.HEdgeList[0];

    TMesh.HEdgeList[0].prev = &TMesh.HEdgeList[3];
    TMesh.HEdgeList[1].prev = &TMesh.HEdgeList[0];
    TMesh.HEdgeList[2].prev = &TMesh.HEdgeList[1];
    TMesh.HEdgeList[3].prev = &TMesh.HEdgeList[2];
}

inline void Tool_extrude::LMB_Press()
{
   
    std::cout << "LMB_Press: Extrude" << std::endl;
}

inline void Tool_extrude::RMB_Press()
{
    TMesh.FaceList.clear();
    TMesh.VertexList.clear();
    TMesh.HEdgeList.clear();
   

    updateTMesh();

    std::cout << "RMB_Press: Cancel Extrude" << std::endl;
}

inline void Tool_extrude::Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh)
{
    state = State::running;
    
    initXpos = xpos;
    initYpos = ypos;

    HEdge* first = mesh->FaceList[polyID].edge;
    
    for (int i = 0; i < 4; i++) {
        TMesh.VertexList[i].position[0] = first->vertex->position[0];
        TMesh.VertexList[i].position[1] = first->vertex->position[1];
        TMesh.VertexList[i].position[2] = first->vertex->position[2];

        initPos[i].x = first->vertex->position[0];
        initPos[i].y = first->vertex->position[1];
        initPos[i].z = first->vertex->position[2];

        first = first->next;
    }
    
    
    /*
    do {
        std::cout << "x: " << next->vertex->position[0] << "y: " << next->vertex->position[1] << "z: " << next->vertex->position[2] << std::endl;
        next = next->next;
    } while (edge != next);

    */

    normalID = polyIdNormal(polyID, mesh);
    
    updateTMesh();

}

inline void Tool_extrude::onMouseMove(double& xpos, double& ypos)
{
    if (state == State::running) 
    {
        double Xdiff = abs(initXpos - xpos);
        double Ydiff = abs(initYpos - ypos);

        double powAndSum = pow(Xdiff, 2) + pow(Ydiff, 2);
        mouseDistance = (float) sqrt(powAndSum);
        
        for (int i = 0; i < 4; i++) 
        {
            TMesh.VertexList[i].position[0] =  initPos[i].x + normalID.x * mouseDistance * 0.002;
            TMesh.VertexList[i].position[1] =  initPos[i].y + normalID.y * mouseDistance * 0.002;
            TMesh.VertexList[i].position[2] =  initPos[i].z + normalID.z * mouseDistance * 0.002;
        }
        
        std::cout << "Dx: " << TMesh.VertexList[0].position[0] << 
                     " Dy: " << TMesh.VertexList[0].position[1] <<
                     " Dz: " << TMesh.VertexList[0].position[2] << std::endl;

        updateTMesh();
           
    }
    else
        mouseDistance = 0;
    
        
}
#endif
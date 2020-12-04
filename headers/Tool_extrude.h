#ifndef TOOL_EXTRUDE_H
#define TOOL_EXTRUDE_H

#include "Tool.h"
#include <iostream>
#include <cmath>

class Tool_extrude : public Tool 
{
    // Mesh TMesh; <- main container
    double initXpos;
    double initYpos;
    float mouseDistance;

public:
    glm::vec3 initPos[4];
    void LMB_Click();
    void RMB_Click();
    void Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh);
    void onMouseMove(double& xpos, double& ypos);
    void mergeIntoMaster(Mesh* mesh, CommandInfo commandInfo);
    
    CommandInfo isReadyToExecute();
    Tool_extrude();
};

inline Tool_extrude::Tool_extrude() 
{
    for (int i = 0; i < 8; i++)
        TMesh.VertexList.push_back(Vert());

   

    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[0])); // 0
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[1])); // 0
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[2])); // 0
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[3])); // 0

    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[4])); // 1
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[0])); // 1
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[3])); // 1
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[7])); // 1

    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[7])); // 2
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[3])); // 2
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[2])); // 2
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[6])); // 2

    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[6])); // 3
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[2])); // 3
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[1])); // 3
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[5])); // 3

    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[5])); // 5
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[1])); // 5
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[0])); // 5
    TMesh.HEdgeList.push_back(HEdge(&TMesh.VertexList[4])); // 5

    TMesh.FaceList.push_back(Face(&TMesh.HEdgeList[0]));
    TMesh.FaceList.push_back(Face(&TMesh.HEdgeList[7]));
    TMesh.FaceList.push_back(Face(&TMesh.HEdgeList[11]));
    TMesh.FaceList.push_back(Face(&TMesh.HEdgeList[15]));
    TMesh.FaceList.push_back(Face(&TMesh.HEdgeList[19]));


    // TOP FACE 

    TMesh.HEdgeList[0].next = &TMesh.HEdgeList[1];
    TMesh.HEdgeList[1].next = &TMesh.HEdgeList[2];
    TMesh.HEdgeList[2].next = &TMesh.HEdgeList[3];
    TMesh.HEdgeList[3].next = &TMesh.HEdgeList[0];

    // SIDE FACES

    TMesh.HEdgeList[4].next = &TMesh.HEdgeList[5];
    TMesh.HEdgeList[5].next = &TMesh.HEdgeList[6];
    TMesh.HEdgeList[6].next = &TMesh.HEdgeList[7];
    TMesh.HEdgeList[7].next = &TMesh.HEdgeList[4];

    TMesh.HEdgeList[8].next = &TMesh.HEdgeList[9];
    TMesh.HEdgeList[9].next = &TMesh.HEdgeList[10];
    TMesh.HEdgeList[10].next = &TMesh.HEdgeList[11];
    TMesh.HEdgeList[11].next = &TMesh.HEdgeList[8];

    TMesh.HEdgeList[12].next = &TMesh.HEdgeList[13];
    TMesh.HEdgeList[13].next = &TMesh.HEdgeList[14];
    TMesh.HEdgeList[14].next = &TMesh.HEdgeList[15];
    TMesh.HEdgeList[15].next = &TMesh.HEdgeList[12];

    TMesh.HEdgeList[16].next = &TMesh.HEdgeList[17];
    TMesh.HEdgeList[17].next = &TMesh.HEdgeList[18];
    TMesh.HEdgeList[18].next = &TMesh.HEdgeList[19];
    TMesh.HEdgeList[19].next = &TMesh.HEdgeList[16];

}

inline void Tool_extrude::LMB_Click()
{
   
    std::cout << "LMB_Press: Extrude" << std::endl;
    if (state == State::running) {
        state = State::execute;
    }
}

inline void Tool_extrude::RMB_Click()
{
    TMesh.FaceList.clear();
    TMesh.VertexList.clear();
    TMesh.HEdgeList.clear();
   
    state = State::init;
    updateToolMesh();

    std::cout << "RMB_Press: Cancel Extrude" << std::endl;
}

inline void Tool_extrude::Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh)
{
    if (state == State::init)
        state = State::running;
    
    runningPolyID = polyID;
    
    initXpos = xpos;
    initYpos = ypos;

    HEdge* first = mesh->FaceList[polyID].edge;
    
    for (int i = 0; i < 8; i++) {
        TMesh.VertexList[i].position[0] = first->vertex->position[0];
        TMesh.VertexList[i].position[1] = first->vertex->position[1];
        TMesh.VertexList[i].position[2] = first->vertex->position[2];

        if (i < 4) {
            initPos[i].x = first->vertex->position[0];
            initPos[i].y = first->vertex->position[1];
            initPos[i].z = first->vertex->position[2];
        }

        first = first->next;
    }

    normalID = polyIdNormal(polyID, mesh);
    normalID = glm::normalize(normalID);
    
    updateToolMesh();

}

inline void Tool_extrude::onMouseMove(double& xpos, double& ypos)
{
    if (state == State::running) 
    {
        double Xdiff = abs(initXpos - xpos);
        double Ydiff = abs(initYpos - ypos);

        double powAndSum = pow(Xdiff, 2) + pow(Ydiff, 2);
        mouseDistance = (float) sqrt(powAndSum);

        
        HEdge* first = TMesh.FaceList[0].edge;
    
        for (int i = 0; i < 4; i++) 
        {
            first->vertex->position[0] = initPos[i].x + normalID.x * mouseDistance * 0.01;
            first->vertex->position[1] = initPos[i].y + normalID.y * mouseDistance * 0.01;
            first->vertex->position[2] = initPos[i].z + normalID.z * mouseDistance * 0.01;
            first = first->next;
        }
        updateToolMesh();       
    }
    else
        mouseDistance = 0;
    
        
}

inline CommandInfo Tool_extrude::isReadyToExecute() 
{
    CommandInfo toolInfo;
    
    if(state == State::execute) 
    {
        toolInfo.amount = mouseDistance;
        toolInfo.meshID = 0;
        toolInfo.polyID = runningPolyID;
        toolInfo.ready = true;
        toolInfo.toolName = "EXTRUDE";
        toolInfo.order = "FIRST";
    }
    else
    {
        toolInfo.amount = mouseDistance;
        toolInfo.meshID = 0;
        toolInfo.polyID = runningPolyID;
        toolInfo.ready = false;
        toolInfo.toolName = "EXTRUDE";
        toolInfo.order = "FIRST";
    }
    

    return toolInfo;
}

inline void Tool_extrude::mergeIntoMaster(Mesh* mesh, CommandInfo commandInfo)
{
    float kerroin = commandInfo.amount * 0.01;
    glm::vec3 normalID = polyIdNormal(commandInfo.polyID, mesh);
    std::cout << "AAA" << std::endl;

    normalID = glm::normalize(normalID);
    glm::vec3 tempVec;
    HEdge* start = mesh->FaceList[commandInfo.polyID].edge;
    HEdge* loopEdge = start;
    

    do {

        tempVec.x = loopEdge->vertex->position[0]  + normalID.x * kerroin;
        tempVec.y = loopEdge->vertex->position[1] + normalID.y * kerroin; 
        tempVec.z = loopEdge->vertex->position[2]  + normalID.z * kerroin; 

        mesh->VertexList.push_back(Vert(tempVec.x, tempVec.y, tempVec.z));
        mesh->HEdgeList.push_back(HEdge(&mesh->VertexList.back()));
        
        loopEdge = loopEdge->next;

    } while (loopEdge != start);
          

    mesh->HEdgeList[mesh->HEdgeList.size()-4].next = &mesh->HEdgeList[mesh->HEdgeList.size()-3];
    mesh->HEdgeList[mesh->HEdgeList.size()-3].next = &mesh->HEdgeList[mesh->HEdgeList.size()-2];
    mesh->HEdgeList[mesh->HEdgeList.size()-2].next = &mesh->HEdgeList[mesh->HEdgeList.size()-1];
    mesh->HEdgeList[mesh->HEdgeList.size()-1].next = &mesh->HEdgeList[mesh->HEdgeList.size()-4];

    mesh->FaceList.push_back(Face(&mesh->HEdgeList[mesh->HEdgeList.size()-1]));

    // SIDES
    
    HEdge* edge = mesh->FaceList[commandInfo.polyID].edge;

    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 3]));
    mesh->HEdgeList.push_back(HEdge(edge->next->vertex));
    mesh->HEdgeList.push_back(HEdge(edge->next->next->vertex));
    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 2]));

    mesh->HEdgeList[mesh->HEdgeList.size()-4].next = &mesh->HEdgeList[mesh->HEdgeList.size()-3];
    mesh->HEdgeList[mesh->HEdgeList.size()-3].next = &mesh->HEdgeList[mesh->HEdgeList.size()-2];
    mesh->HEdgeList[mesh->HEdgeList.size()-2].next = &mesh->HEdgeList[mesh->HEdgeList.size()-1];
    mesh->HEdgeList[mesh->HEdgeList.size()-1].next = &mesh->HEdgeList[mesh->HEdgeList.size()-4];    
    
    mesh->FaceList.push_back(Face(&mesh->HEdgeList[mesh->HEdgeList.size()-1]));

    edge = edge->next; //NEXT SIDE

    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 2]));
    mesh->HEdgeList.push_back(HEdge(edge->next->vertex));
    mesh->HEdgeList.push_back(HEdge(edge->next->next->vertex));
    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 1]));

    mesh->HEdgeList[mesh->HEdgeList.size()-4].next = &mesh->HEdgeList[mesh->HEdgeList.size()-3];
    mesh->HEdgeList[mesh->HEdgeList.size()-3].next = &mesh->HEdgeList[mesh->HEdgeList.size()-2];
    mesh->HEdgeList[mesh->HEdgeList.size()-2].next = &mesh->HEdgeList[mesh->HEdgeList.size()-1];
    mesh->HEdgeList[mesh->HEdgeList.size()-1].next = &mesh->HEdgeList[mesh->HEdgeList.size()-4];    
    
    mesh->FaceList.push_back(Face(&mesh->HEdgeList[mesh->HEdgeList.size()-1]));

    edge = edge->next; //NEXT SIDE

    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 1]));
    mesh->HEdgeList.push_back(HEdge(edge->next->vertex));
    mesh->HEdgeList.push_back(HEdge(edge->next->next->vertex));
    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 4]));

    mesh->HEdgeList[mesh->HEdgeList.size()-4].next = &mesh->HEdgeList[mesh->HEdgeList.size()-3];
    mesh->HEdgeList[mesh->HEdgeList.size()-3].next = &mesh->HEdgeList[mesh->HEdgeList.size()-2];
    mesh->HEdgeList[mesh->HEdgeList.size()-2].next = &mesh->HEdgeList[mesh->HEdgeList.size()-1];
    mesh->HEdgeList[mesh->HEdgeList.size()-1].next = &mesh->HEdgeList[mesh->HEdgeList.size()-4];    
    
    mesh->FaceList.push_back(Face(&mesh->HEdgeList[mesh->HEdgeList.size()-1]));

    edge = edge->next; //NEXT SIDE

    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 4]));
    mesh->HEdgeList.push_back(HEdge(edge->next->vertex));
    mesh->HEdgeList.push_back(HEdge(edge->next->next->vertex));
    mesh->HEdgeList.push_back(HEdge(&mesh->VertexList[mesh->VertexList.size() - 3]));

    mesh->HEdgeList[mesh->HEdgeList.size()-4].next = &mesh->HEdgeList[mesh->HEdgeList.size()-3];
    mesh->HEdgeList[mesh->HEdgeList.size()-3].next = &mesh->HEdgeList[mesh->HEdgeList.size()-2];
    mesh->HEdgeList[mesh->HEdgeList.size()-2].next = &mesh->HEdgeList[mesh->HEdgeList.size()-1];
    mesh->HEdgeList[mesh->HEdgeList.size()-1].next = &mesh->HEdgeList[mesh->HEdgeList.size()-4];    
    
    mesh->FaceList.push_back(Face(&mesh->HEdgeList[mesh->HEdgeList.size()-1]));
    
    mesh->FaceList[commandInfo.polyID].edge = nullptr;

    TMesh.FaceList.clear();
    TMesh.VertexList.clear();
    TMesh.HEdgeList.clear();
    
    updateToolMesh();
}

inline void mergeExternalCommand()
{

}

#endif
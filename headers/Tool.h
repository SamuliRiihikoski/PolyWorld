#ifndef TOOL_H
#define TOOL_H
#include "App.h"
#include <string>

struct CommandInfo 
{
    string toolName;
    unsigned int meshID;
    unsigned int polyID;
    float amount;
    bool ready;
};

class Tool 
{
public:

    Mesh TMesh;
    unsigned int runningPolyID;
    vector<Vertex> TEdges{ Vertex(0.0f, 0.0f, 0.0f), Vertex(0.0f, 0.0f, 0.0f) };

    unsigned int meshVBO;
    unsigned int edgeVBO;
    enum class State { init, running, execute};
    State state = State::init;
    CommandInfo commandInfo;
    glm::vec3 normalID; 

    virtual void LMB_Press() {};
    virtual void RMB_Press() {};
    virtual void Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh) {};
    virtual void onMouseMove(double& xpos, double& ypos) {};

    virtual CommandInfo isReadyToExecute() {}; // this is run in main.. listeting if tool needs to be executed.
    virtual void mergeIntoMaster(Mesh* mesh, CommandInfo commandInfo) {};

    void stateToInit();

    void RenderMESH();
    void RenderEDGES();
    void updateToolMesh();
    void renderTool();
    
    static glm::vec3 polyIdNormal(unsigned int polyID, Mesh* mesh);
    
        
    Tool();
    ~Tool();
};

inline Tool::Tool()
{   
    glGenBuffers(1, &meshVBO);
    glGenBuffers(1, &edgeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, (24*3) * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
    glBufferData(GL_ARRAY_BUFFER, (40*3) * sizeof(float), NULL, GL_DYNAMIC_DRAW);
}

inline Tool::~Tool() 
{
    glDeleteBuffers(1, &meshVBO);
    glDeleteBuffers(1, &edgeVBO);
}

inline glm::vec3 Tool::polyIdNormal(unsigned int polyId, Mesh* mesh)
{
    HEdge* edge = mesh->FaceList[polyId].edge;    
    glm::vec3 p1(edge->vertex->position[0], edge->vertex->position[1], edge->vertex->position[2]);   
    edge = edge->next;
    glm::vec3 p2(edge->vertex->position[0], edge->vertex->position[1], edge->vertex->position[2]);
    edge = edge->next;
    glm::vec3 p3(edge->vertex->position[0], edge->vertex->position[1], edge->vertex->position[2]);  

    glm::vec3 n1, n2;
    n1 = p1 - p2;
    n2 = p1 - p3;

    glm::vec3 cross = glm::cross(n1, n2);

    return cross;
}

inline void Tool::RenderMESH()
{
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, TMesh.FaceList.size() * 6);
}

inline void Tool::RenderEDGES()
{
    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_LINES, 0, 50);
}

inline void Tool::updateToolMesh()
{
    vector<Vertex> vboMesh;
    TEdges.clear();

    HEdge* first;
    HEdge* next;
    Vert* v;

    for (int i = 0; i < TMesh.FaceList.size(); i++)
    {
        first = TMesh.FaceList[i].edge;
        v = first->vertex;
        
        for (int u = 0; u < 5; u++) {
            
            vboMesh.push_back(Vertex(v->position[0], v->position[1], v->position[2]));
        
            if (u == 2)
                vboMesh.push_back(Vertex(v->position[0], v->position[1], v->position[2]));
            first = first->next;
            v = first->vertex;
        }

    }

    for (int i = 0; i < TMesh.FaceList.size(); i++)
    {
        first = TMesh.FaceList[i].edge;
        next = first->next;

        for (int t = 0; t < 4; t++)
        {
            TEdges.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
            TEdges.push_back(Vertex(next->vertex->position[0], next->vertex->position[1], next->vertex->position[2]));
            first = first->next;
            next = first->next;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, vboMesh.size()*3*sizeof(float), vboMesh.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
    glBufferData(GL_ARRAY_BUFFER, TEdges.size()*3*sizeof(float), TEdges.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glEnableVertexAttribArray(0);
    
}

inline void Tool::stateToInit()
{
    state = State::init;
}

#endif
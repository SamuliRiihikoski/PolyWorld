#ifndef TOOL_H
#define TOOL_H
#include "App.h"
class Tool 
{
public:
    unsigned int ToolVBO;
    enum class State { init, running};
    State state = State::init; 

    virtual void LMB_Press() {};
    virtual void RMB_Press() {};
    virtual void Render() {};
    virtual void Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh) {};
    virtual void onMouseMove(double& xpos, double& ypos) {};
    
    glm::vec3 polyIdNormal(unsigned int polyID, Mesh* mesh);
    glm::vec3 normalID;    
    
    Tool();
    ~Tool();
};

inline Tool::Tool()
{
    glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
    glBufferData(GL_ARRAY_BUFFER, (24*3) * sizeof(float), NULL, GL_DYNAMIC_DRAW);
}

inline Tool::~Tool() 
{
    glDeleteBuffers(1, &ToolVBO);
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

#endif
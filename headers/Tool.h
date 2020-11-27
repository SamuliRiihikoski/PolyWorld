#ifndef TOOL_H
#define TOOL_H
#include "App.h"
class Tool 
{
public:

    Mesh TMesh;

    unsigned int ToolVBO;
    enum class State { init, running};
    State state = State::init; 

    virtual void LMB_Press() {};
    virtual void RMB_Press() {};
    virtual void Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh) {};
    virtual void onMouseMove(double& xpos, double& ypos) {};
    
    void Render();
    void updateTMesh();
    void renderTool();
    
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

inline void Tool::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, TMesh.FaceList.size() * 6);
}

inline void Tool::updateTMesh()
{
    vector<Vertex> vboMesh;
    HEdge* first;
    
    for (int i = 0; i < TMesh.FaceList.size(); i++)
    {
        first = TMesh.FaceList[i].edge;
        
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;

        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        
        first = first->next;

        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        
        first = first->next;
        
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
    }
     
    glGenBuffers(1, &ToolVBO);

    glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
    glBufferData(GL_ARRAY_BUFFER, vboMesh.size()*3*sizeof(float), vboMesh.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    /*
    for (int i = 0; i < TMesh.FaceList.size(); i++) {
        
        HEdge* first = TMesh.FaceList[i].edge;
        HEdge* next = TMesh.FaceList[i].edge;

        do {    
            std::cout << i << "  x: " << next->vertex->position[0] << 
                              "  y: " << next->vertex->position[1] << 
                              "  z: " << next->vertex->position[2] << std::endl;

            next = next->next;
        } 
        while (first != next);
    
    }
    */
    
    
}
#endif
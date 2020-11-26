#ifndef TOOL_EXTRUDE_H
#define TOOL_EXTRUDE_H

#include "Tool.h"
#include <iostream>
#include <cmath>

class Tool_extrude : public Tool 
{
    vector<Vertex> toolMesh;
    glm::vec3 sPos[3];
    double initXpos;
    double initYpos;
    float mouseDistance;

public:
    void LMB_Press();
    void RMB_Press();
    void Render();
    void Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh);
    void onMouseMove(double& xpos, double& ypos);
};

inline void Tool_extrude::LMB_Press()
{
    std::cout << "LMB_Press: Extrude" << std::endl;
}

inline void Tool_extrude::RMB_Press()
{
    std::cout << "RMB_Press: Cancel Extrude" << std::endl;
}

inline void Tool_extrude::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

inline void Tool_extrude::Execute(double& xpos, double& ypos, unsigned int polyID, Mesh* mesh)
{
    state = State::running;
    
    initXpos = xpos;
    initYpos = ypos;


    toolMesh.push_back(Vertex(-2.0f, -1.0f, 1.0f));
    toolMesh.push_back(Vertex( 2.0f, -1.0f, 1.0f));
    toolMesh.push_back(Vertex( 2.0f,  3.0f, 1.0f));

    sPos[0] = glm::vec3(toolMesh[0].pos[0], toolMesh[0].pos[1], toolMesh[0].pos[2]);
    sPos[1] = glm::vec3(toolMesh[1].pos[0], toolMesh[1].pos[1], toolMesh[1].pos[2]);
    sPos[2] = glm::vec3(toolMesh[2].pos[0], toolMesh[2].pos[1], toolMesh[2].pos[2]);

    normalID = polyIdNormal(polyID, mesh);
    
    glGenBuffers(1, &ToolVBO);

    glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
    glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(float), toolMesh.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    
}

inline void Tool_extrude::onMouseMove(double& xpos, double& ypos)
{
    if (state == State::running) 
    {
        
        double Xdiff = abs(initXpos - xpos);
        double Ydiff = abs(initYpos - ypos);

        double powAndSum = pow(Xdiff, 2) + pow(Ydiff, 2);
        mouseDistance = (float) sqrt(powAndSum);
        
        for (int i = 0; i < toolMesh.size(); i++) 
        {
        toolMesh[i].pos[0] =  sPos[i].x + normalID.x * mouseDistance * 0.001;
        toolMesh[i].pos[1] =  sPos[i].y + normalID.y * mouseDistance * 0.001;
        toolMesh[i].pos[2] =  sPos[i].z + normalID.z * mouseDistance * 0.001;
        }
        
        //std::cout << "Distance: " << (float)mouseDistance << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
        glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(float), toolMesh.data(), GL_DYNAMIC_DRAW);  
           
    }
    else
        mouseDistance = 0;
}
#endif
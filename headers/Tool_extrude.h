#ifndef TOOL_EXTRUDE_H
#define TOOL_EXTRUDE_H

#include "Tool.h"
#include <iostream>
#include <cmath>

class Tool_extrude : public Tool 
{

    vector<Vertex> toolMesh;
    float savedPos;
    double initXpos;
    double initYpos;
    double mouseDistance;

    enum class State { init, running};
    State state = State::init; 

public:
    void LMB_Down();
    void Render();
    void Execute(double& xpos, double& ypos);
    void onMouseMove(double& xpos, double& ypos);

};

inline void Tool_extrude::LMB_Down()
{
    std::cout << "LMB_Down: Extrude" << std::endl;
}

inline void Tool_extrude::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

inline void Tool_extrude::Execute(double& xpos, double& ypos)
{
    state = State::running;

    initXpos = xpos;
    initYpos = ypos;

    toolMesh.push_back(Vertex(-2.0f, -1.0f, 1.0f));
    toolMesh.push_back(Vertex( 2.0f, -1.0f, 1.0f));
    toolMesh.push_back(Vertex( 2.0f,  3.0f, 1.0f));

    savedPos = toolMesh[0].pos[0];
    
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
        mouseDistance = sqrt(powAndSum);
        toolMesh[0].pos[0] =  savedPos + (float) mouseDistance * 0.01;

        std::cout << "Distance: " << (float)mouseDistance << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, ToolVBO);
        glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(float), toolMesh.data(), GL_DYNAMIC_DRAW);       
    }
    else
        mouseDistance = 0;
}
#endif
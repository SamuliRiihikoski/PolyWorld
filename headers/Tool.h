#ifndef TOOL_H
#define TOOL_H

class Tool 
{
   

public:
    virtual void LMB_Down() {};
    virtual void Render() {};
    virtual void Execute(double& xpos, double& ypos) {};
    virtual void onMouseMove(double& xpos, double& ypos) {};
    
    unsigned int ToolVBO;
    
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

#endif
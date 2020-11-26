#ifndef TOOL_LOOPCUT_H
#define TOOL_LOOPCUT_H

#include "Tool.h"
#include <iostream>

class Tool_loopcut : public Tool 
{
public:
    void LMB_Down();
    void Render();
    void Execute();
    void onMouseMove(double& xpos, double& ypos);
};

inline void Tool_loopcut::LMB_Down()
{
    std::cout << "LMB_Down: Loopcut" << std::endl;
}

inline void Tool_loopcut::Render()
{
    
}

inline void Tool_loopcut::Execute()
{
     std::cout << "Start loopcut" << std::endl;
}

inline void Tool_loopcut::onMouseMove(double& xpos, double& ypos)
{

}
#endif
#ifndef TOOL_LOOPCUT_H
#define TOOL_LOOPCUT_H

#include "Tool.h"
#include <iostream>

class Tool_loopcut : public Tool 
{
public:
    void LMB_Press();
    void RMB_Press();
    void Execute(double& xpos, double& ypos, unsigned int polyID);
    void onMouseMove(double& xpos, double& ypos);
};

inline void Tool_loopcut::LMB_Press()
{
    std::cout << "LMB_Press: Loopcut" << std::endl;
}

inline void Tool_loopcut::RMB_Press()
{
    std::cout << "RMB_Press: Loopcut" << std::endl;
}

inline void Tool_loopcut::Execute(double& xpos, double& ypos, unsigned int polyID)
{
     std::cout << "Start loopcut" << std::endl;
}

inline void Tool_loopcut::onMouseMove(double& xpos, double& ypos)
{

}
#endif
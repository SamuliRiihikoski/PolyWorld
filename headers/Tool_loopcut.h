#ifndef TOOL_LOOPCUT_H
#define TOOL_LOOPCUT_H

#include "Tool.h"
#include <iostream>

class Tool_loopcut : public Tool 
{
public:
    void LMB_Down();
};

inline void Tool_loopcut::LMB_Down()
{
    std::cout << "LMB_Down: Loopcut" << std::endl;
}

#endif
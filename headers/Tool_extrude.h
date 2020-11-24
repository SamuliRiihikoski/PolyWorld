#ifndef TOOL_EXTRUDE_H
#define TOOL_EXTRUDE_H

#include "Tool.h"
#include <iostream>

class Tool_extrude : public Tool 
{
public:
    void LMB_Down();
};

inline void Tool_extrude::LMB_Down()
{
    std::cout << "LMB_Down: Extrude" << std::endl;
}

#endif
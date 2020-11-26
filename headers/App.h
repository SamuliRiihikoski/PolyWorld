#ifndef APP_H
#define APP_H

#include <vector>
#include "Scene.h"
#include "Tool.h"
#include "Tool_extrude.h"
#include "Tool_loopcut.h"


class App
{
private:
    std::vector<Scene> scenes;

public:
    void newScene();
    Scene getScene(int index);
    unsigned int activePolyID;
    unsigned int firstClickPolyID;
    Tool* activeTool;
    void newActiveTool(string newToolName);
};

inline void App::newScene()
{
    scenes.push_back(Scene("Scene"));
    activePolyID = -1;
    activeTool = nullptr;
}

inline Scene App::getScene(int index)
{
    return scenes[index];
}

inline void App::newActiveTool(string newToolName) 
{
    if (activeTool != nullptr) {
        delete activeTool;
        activeTool = nullptr;
    }

    if (newToolName == "EXTRUDE")
        activeTool = new Tool_extrude;
    else if (newToolName == "LOOPCUT")
        activeTool = new Tool_loopcut;
}

#endif

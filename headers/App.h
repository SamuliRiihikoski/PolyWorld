#ifndef APP_H
#define APP_H

#include <vector>
#include "Scene.h"
#include "Tool.h"
#include "Tool_extrude.h"
#include "Tool_loopcut.h"

class App
{
    std::vector<Scene> scenes;
public:
    void newScene();
    Scene getScene(int index);
    unsigned int activePolyID;
    unsigned int firstClickPolyID;
    
    Tool* activeTool;
    void executeTool();
    void newActiveTool(string newToolName);
    void updateMasterMesh();
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

inline void App::executeTool() 
{
    CommandInfo toolInfo = activeTool->isReadyToExecute();   
    
    if(toolInfo.ready == false)
        return;

    Mesh mesh = scenes[0].getMesh(0);
    glm::vec3 normal = Tool::polyIdNormal(toolInfo.polyID, &mesh);
    normal = glm::normalize(normal);

    HEdge* edge = mesh.FaceList[toolInfo.polyID].edge;
    
    std::cout << "x: " << normal.x << " y: " << normal.y << " z: " << normal.z << std::endl;
    

    for (int i = 0; i < 4; i++) 
    {

    }

    activeTool->stateToInit();
}


#endif

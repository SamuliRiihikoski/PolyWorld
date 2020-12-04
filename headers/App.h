#ifndef APP_H
#define APP_H

#include <vector>
#include <string>
#include "Scene.h"
#include "Tool.h"
#include "Tool_extrude.h"
#include "Tool_loopcut.h"
#include "SocketManager.h"

enum class Mode {object, edit};
enum class SelMode {vertex, edge, face, multi};

class App
{
    std::vector<Scene> scenes;  
    Mode mainMode = Mode::object;
    SelMode selMode = SelMode::face;
 

public:
    void newScene();
    Scene getScene(int index);
    unsigned int activePolyID;
    float rayPolyDistance = -1;
    unsigned int firstClickPolyID;
    bool updateScene = false;
    bool disableRay = false;
    
    Mode getMode();
    SocketManager socketManager;
    
    Tool* activeTool; // users active tool
    Tool* externalTool; // online users tool pointer for command line
    void executeTool();
    void executeExternalTool();
    void newActiveTool(Tool* tool, string newToolName);
    void updateMasterMesh(unsigned int VBO[]);
    void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods, unsigned int hoverPolyID);

    
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

inline void App::newActiveTool(Tool* tool, string newToolName) 
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
    CommandInfo commandData = activeTool->isReadyToExecute();   
    
    if(commandData.ready == false)
        return;

    Mesh* mesh = scenes[0].getMeshPointer(0);
    activeTool->mergeIntoMaster(mesh, commandData);
    socketManager.sendCommand(commandData);
    updateScene = true;

    activeTool->stateToInit();
}

inline void App::executeExternalTool()
{
    
     if (externalTool != nullptr) {
        delete externalTool;
        externalTool = nullptr;
    }

    if (externalCommand.toolName == "EXTRUDE")
        externalTool = new Tool_extrude;
    else if (externalCommand.toolName == "LOOPCUT")
        externalTool = new Tool_loopcut;
    Mesh* mesh = scenes[0].getMeshPointer(0);
    externalTool->mergeIntoMaster(mesh, externalCommand);
    
    updateScene = true;

    if(activeTool && (externalCommand.polyID == activeTool->runningPolyID))
        activeTool->RMB_Click();

    if (externalTool != nullptr) {
        delete externalTool;
        externalTool = nullptr;
    }
    

}

inline void App::updateMasterMesh(unsigned int VBO[])
{
    Mesh mesh = getScene(0).getMesh(0);
    vector<Vertex> vboMesh;
    vector<Vertex> vboEdge;
    HEdge* first;
    for (int i = 0; i < mesh.FaceList.size(); i++)
    {
        if(mesh.FaceList[i].edge == nullptr)
            continue;

        first = mesh.FaceList[i].edge;

        for (int u = 0; u < 5; u++) { // TODO SUPPORTS ONLY QUAD POLYGONS
            vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
            if(u==2)
                vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));

            first = first->next;
        } 
    }

    for (int i = 0; i < mesh.FaceList.size(); i++)
    {
        if(mesh.FaceList[i].edge == nullptr)
            continue;

        first = mesh.FaceList[i].edge;
        Vert* v = first->vertex;
        for (int u = 0; u < 5; u++) {
            vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
            first = first->next;
            vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        }
    }

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, (vboMesh.size()*3) * sizeof(float), vboMesh.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,  18 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (vboEdge.size()*3) * sizeof(float), vboEdge.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glBindVertexArray(0);
}

void App::keyInput(GLFWwindow* window, int key, int scancode, int action, int mods, unsigned int hoverPolyID)
{
    string newToolName;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        if (mainMode == Mode::object) {
            mainMode = Mode::edit;
            std::cout << "EDIT MODE" << std::endl;
        }
        else {
            mainMode = Mode::object;
            std::cout << "OBJECT MODE" << std::endl;

        }
        
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) 
    {
        newToolName = "EXTRUDE";
        newActiveTool(activeTool, newToolName);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if(hoverPolyID != -1) {
            activeTool->Execute(xpos, ypos, hoverPolyID, getScene(0).getMeshPointer(0));
        }
        
    }

     if (key == GLFW_KEY_C && action == GLFW_PRESS) 
    {
        Mesh mesh = getScene(0).getMesh(0);

        for (int i = 0; i < mesh.FaceList.size(); i++) {
        
            std::cout << "Face " << i << std::endl;
            HEdge* first = mesh.FaceList[i].edge;

            if(first == nullptr)
                continue;

            HEdge* edge = first;

            for (int hh = 0; hh < 4; hh++) {
                std::cout << "x: " << edge->vertex->position[0] << " y: " << edge->vertex->position[1] << " z: " << edge->vertex->position[2] << std::endl;
                edge = edge->next;
            } 
        }
    }
}

inline Mode App::getMode()
{
    return mainMode;
}

#endif

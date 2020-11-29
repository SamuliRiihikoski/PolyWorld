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
    float rayPolyDistance = -1;
    unsigned int firstClickPolyID;
    bool updateScene = false;
    
    Tool* activeTool;
    void executeTool();
    void newActiveTool(string newToolName);
    void updateMasterMesh(unsigned int VBO[]);
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
    CommandInfo commandData = activeTool->isReadyToExecute();   
    
    if(commandData.ready == false)
        return;

    Mesh* mesh = scenes[0].getMeshPointer(0);

    activeTool->mergeIntoMaster(mesh, commandData);
    updateScene = true;

    activeTool->stateToInit();
}

inline void App::updateMasterMesh(unsigned int VBO[])
{
    Mesh mesh = getScene(0).getMesh(0);
    vector<Vertex> vboMesh;
    vector<Vertex> vboEdge;
    
    for (int i = 0; i < mesh.FaceList.size(); i++)
    {
        if(mesh.FaceList[i].edge == nullptr)
            continue;

        HEdge* first = mesh.FaceList[i].edge;
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

        HEdge* first = mesh.FaceList[i].edge;
        Vert* v = first->vertex;

        for (int u = 0; u < 5; u++) {
            vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
            first = first->next;
            vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        }

      
    }

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, (vboMesh.size()*3) * sizeof(float), vboMesh.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,  18 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (vboEdge.size()*3) * sizeof(float), vboEdge.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glBindVertexArray(0);
}


#endif

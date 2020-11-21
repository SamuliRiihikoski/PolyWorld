#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Mesh.h"
#include <string>
#include <iostream>
#include "Primitives.h"

class Scene {

    int ID;
    string name;
    std::vector<Mesh> meshes;
    
public:
     
    Scene(std::string name) : name(name) {
        std::cout << "Created new scene: " <<  name << std::endl;
        initScene();
    }

    void initScene();
    void addScene();
    void removeScene();
    void updateScene();
    Mesh getMesh(int index);

};

inline void Scene::initScene()
{
    std::cout << "Initializing scene..." << std::endl;

    meshes.push_back(Mesh("Cube"));

    meshes[0].vertices.push_back(Vertex(-1.0f, 0.0f, 0.0f));
	meshes[0].vertices.push_back(Vertex(1.0f, 0.0f, 0.0f));
	meshes[0].vertices.push_back(Vertex(0.0f, 2.0f, 0.0f));

    meshes[0].faces.push_back(Polygon(0));
    meshes[0].faces[0].verticesID.push_back(0);
    meshes[0].faces[0].verticesID.push_back(1);
    meshes[0].faces[0].verticesID.push_back(2);

    updateScene();

    std::cout << "Initializing done!" << std::endl;
}

inline void Scene::addScene()
{
    std::cout << "Add scene..." << std::endl;
}

inline void Scene::removeScene()
{
    std::cout << "Removing scene..." << std::endl;
}

inline void Scene::updateScene()
{

}

inline Mesh Scene::getMesh(int index)
{
    return meshes[index];
}

#endif
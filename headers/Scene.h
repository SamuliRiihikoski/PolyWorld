#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Mesh.h"
#include <string>
#include <iostream>
#include "Primitives.h"

struct Users 
{
    int ID;
};

class Scene {

    int ID;
    string name;
    std::vector<Mesh> meshes;
    std::vector<Users> users;
    
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
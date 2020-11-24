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

    meshes[0].vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f)); // P0
	meshes[0].vertices.push_back(Vertex(1.0f, -1.0f, 1.0f)); // P1
	meshes[0].vertices.push_back(Vertex(1.0f, 1.0f, 1.0f)); // P2
    meshes[0].vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f)); // P3

    meshes[0].vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f)); // P4
	meshes[0].vertices.push_back(Vertex(1.0f, -1.0f, -1.0f)); // P5
	meshes[0].vertices.push_back(Vertex(1.0f, 1.0f, -1.0f)); // P6
    meshes[0].vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f)); // P7

    meshes[0].faces.push_back(Polygon(0));
    
    // front
    meshes[0].faces[0].verticesID.push_back(0);
    meshes[0].faces[0].verticesID.push_back(1);
    meshes[0].faces[0].verticesID.push_back(2);

    meshes[0].faces[0].verticesID.push_back(2);
    meshes[0].faces[0].verticesID.push_back(3);
    meshes[0].faces[0].verticesID.push_back(0);

    // right
    meshes[0].faces[0].verticesID.push_back(1);
    meshes[0].faces[0].verticesID.push_back(5);
    meshes[0].faces[0].verticesID.push_back(6);

    meshes[0].faces[0].verticesID.push_back(6);
    meshes[0].faces[0].verticesID.push_back(2);
    meshes[0].faces[0].verticesID.push_back(1);

    // back
    meshes[0].faces[0].verticesID.push_back(7);
    meshes[0].faces[0].verticesID.push_back(6);
    meshes[0].faces[0].verticesID.push_back(5);

    meshes[0].faces[0].verticesID.push_back(5);
    meshes[0].faces[0].verticesID.push_back(4);
    meshes[0].faces[0].verticesID.push_back(7);

    // left
    meshes[0].faces[0].verticesID.push_back(4);
    meshes[0].faces[0].verticesID.push_back(0);
    meshes[0].faces[0].verticesID.push_back(3);

    meshes[0].faces[0].verticesID.push_back(3);
    meshes[0].faces[0].verticesID.push_back(7);
    meshes[0].faces[0].verticesID.push_back(4);

    // bottom
    meshes[0].faces[0].verticesID.push_back(4);
    meshes[0].faces[0].verticesID.push_back(5);
    meshes[0].faces[0].verticesID.push_back(1);

    meshes[0].faces[0].verticesID.push_back(1);
    meshes[0].faces[0].verticesID.push_back(0);
    meshes[0].faces[0].verticesID.push_back(4);

    // top
    meshes[0].faces[0].verticesID.push_back(3);
    meshes[0].faces[0].verticesID.push_back(2);
    meshes[0].faces[0].verticesID.push_back(6);

    meshes[0].faces[0].verticesID.push_back(6);
    meshes[0].faces[0].verticesID.push_back(7);
    meshes[0].faces[0].verticesID.push_back(3);




    
    
    

    meshes.push_back(Mesh("Test"));

    meshes[1].vertices.push_back(Vertex(-1.0f, 0.0f, 0.0f));
	meshes[1].vertices.push_back(Vertex(1.0f, 0.0f, 0.0f));
	meshes[1].vertices.push_back(Vertex(-1.0f, 2.0f, 0.0f));

    meshes[1].faces.push_back(Polygon(0));
    meshes[1].faces[0].verticesID.push_back(0);
    meshes[1].faces[0].verticesID.push_back(1);
    meshes[1].faces[0].verticesID.push_back(2);

    //Halfedge-Based

    meshes[0].VertexList.push_back(Vert(-1.0f, 0.0f, 0.0f));
    meshes[0].VertexList.push_back(Vert(1.0f, 0.0f, 0.0f));
    meshes[0].VertexList.push_back(Vert(0.0f, 2.0f, 0.0f));


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
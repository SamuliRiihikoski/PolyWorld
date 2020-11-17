#include <vector>
//#include "Mesh.h" NEXT
#include <string>
#include <iostream>

struct Vertex {
    float pos[3];

    Vertex(float x, float y, float z) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
    }
};

struct Polygon {
    int id;
    std::vector<unsigned int> verticesID;

    Polygon(int id) {
        id = id;
    }
};

class Scene {

    std::string name;
   
    //std::vector<Mesh> meshes; NEXT

public:

    std::vector<Vertex> vertices;
    std::vector<Polygon> faces;

    void initScene();
    
    Scene(std::string name) : name(name) {
        std::cout << "Created new scene: " <<  name << std::endl;
        initScene();
    }

   
    void addScene();
    void removeScene();
    void updateScene();

};

inline void Scene::initScene()
{
    std::cout << "Initializing scene..." << std::endl;

    vertices.push_back(Vertex(-1.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(1.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(0.0f, 2.0f, 0.0f));

    faces.push_back(Polygon(0));
    faces[0].verticesID.push_back(0);
    faces[0].verticesID.push_back(1);
    faces[0].verticesID.push_back(2);

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
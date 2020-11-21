#ifndef APP_H
#define APP_H

#include <vector>
#include "Scene.h"

class App
{
private:
    std::vector<Scene> scenes;

public:
    void newScene();
    Scene getScene(int index);
};

inline void App::newScene()
{
    scenes.push_back(Scene("Scene"));
}

inline Scene App::getScene(int index)
{
    return scenes[index];
}

#endif

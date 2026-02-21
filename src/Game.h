#ifndef __GAME_H__
#define __GAME_H__

#include <string>
#include <vector>

#include "Scene/Scene.h"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    unsigned int width;
    unsigned int height;
    std::string title;

    std::vector<Scene*> scenes;

    int currentScene;
};

#endif

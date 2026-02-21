#ifndef __SCENE_H__
#define __SCENE_H__

#include <raylib.h>

#include <vector>

#include "../Entity/Entity.h"
#include "../Entity/Player/Player.h"

class Scene {
public:
    Scene(const std::string& name, int width, int height,
        Texture2D& wallTexture);
    virtual ~Scene();

    std::string getName();

    void run();
    bool isWall(int x, int y) const;
    void addEntity(Entity* entity);

protected:
    virtual void events() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() const = 0;
    void drawMap() const;

    virtual void init() = 0;

    virtual void reset() = 0;

    std::vector<Entity*> entities;
    Player* player;
    bool isRunning;
    int width;
    int height;

    Texture2D wallTexture;

private:
    std::string name;

protected:
    std::vector<std::string> map;
};

#endif
#include "Scene.h"

#include <raylib.h>

#include "../utils/utils.h"

Scene::Scene(const std::string& name, int width, int height,
    Texture2D& wallTexture)
    : name(name)
    , width(width)
    , height(height)
    , wallTexture(wallTexture)
{
    SetWindowTitle(this->name.c_str());
}

Scene::~Scene() { }

void Scene::run()
{
    if (!this->isRunning) {
        this->init();
    }

    while (!WindowShouldClose() && this->isRunning) {
        float dt = GetFrameTime();
        this->events();
        this->draw();
        this->update(dt);
        // DrawFPS(10, 10);
    }
}

std::string
Scene::getName()
{
    return this->name;
}

bool Scene::isWall(int x, int y) const
{
    if (x < 0 || x >= this->map.size() || y < 0 || y >= this->map[0].length()) {
        return true;
    }
    return this->map[x][y] == '1';
}

void Scene::addEntity(Entity* entity)
{
    this->entities.push_back(entity);
}

void Scene::drawMap() const
{
    const float size = 5.0f;

    for (int x = 0; x < this->map.size(); x++) {
        const std::string& row = this->map[x];

        for (int y = 0; y < row.length(); y++) {
            Vector3 pos = { (float)x * size, 0, (float)y * size };
            char tileType = row[y];

            if (tileType == '1') {
                DrawCubeTexture(this->wallTexture, pos, size, size * 2, size,
                    WHITE);
            } else {
                DrawPlane({ pos.x, -size / 2, pos.z }, { size, size }, GREEN);
            }
        }
    }
}
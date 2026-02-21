#include "Game.h"

#include <raylib.h>

#include "Entity/Entities.h"
#include "Entity/Flute/Flute.h"
#include "Entity/Player/Player.h"
#include "Entity/PlayerEnemy/PlayerEnemy.h"
#include "Scene/BabyScene/BabyScene.h"
#include "Scene/PlayerScene/PlayerScene.h"
#include "Scene/VictoryBabyScene/VictoryBabyScene.h"
#include "Scene/VictoryBabySceneWeird/VictoryBabySceneWeird.h"
#include "Scene/VictoryPlayerScene/VictoryPlayerScene.h"
#include "Scene/VictoryPlayerSceneWeird/VictoryPlayerSceneWeird.h"

void loadAssets()
{
    PlayerScene::load();
    BabyScene::load();
    FireBall::load();
    Explosion::load();
    Flute::load();
    PlayerEnemy::load();
    Baby::load();
    PizzaMan::load();
    BabyMan::load();
    Player::load();
}

void unloadAssets()
{
    PlayerScene::unload();
    BabyScene::unload();
    FireBall::unload();
    Explosion::unload();
    PlayerEnemy::unload();
    Flute::unload();
    Baby::unload();
    PizzaMan::unload();
    BabyMan::unload();
    Player::unload();
}

Game::Game()
    : title("")
    , currentScene(0)
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(0, 0, "");
    DisableCursor();

    int curr = GetCurrentMonitor();

    this->width = GetMonitorWidth(curr);
    this->height = GetMonitorHeight(curr);

    InitAudioDevice();

    loadAssets();

    this->scenes.push_back(new PlayerScene(width, height, this->currentScene));
    this->scenes.push_back(
        new VictoryPlayerScene(width, height, this->currentScene));
    this->scenes.push_back(
        new VictoryPlayerSceneWeird(width, height, this->currentScene));
    this->scenes.push_back(new BabyScene(width, height, this->currentScene));
    this->scenes.push_back(
        new VictoryBabyScene(width, height, this->currentScene));
    this->scenes.push_back(
        new VictoryBabySceneWeird(width, height, this->currentScene));
}

Game::~Game()
{
    unloadAssets();
    CloseWindow();
}

void Game::run()
{
    while (!WindowShouldClose()) {
        this->scenes[currentScene]->run();
    }
}

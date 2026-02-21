#include "VictoryBabyScene.h"

#include <raylib.h>
#include <raymath.h>

#include <vector>

#include "../../Entity/Entities.h"

VictoryBabyScene::VictoryBabyScene(int width, int height, int& currScene)
    : Scene("Victory", width, height, wallTexture)
    , currScene(currScene)
{
    this->init();
}

VictoryBabyScene::~VictoryBabyScene()
{
    if (player) {
        delete player;
        player = nullptr;
    }
}

void VictoryBabyScene::init()
{
    this->currentDialogue = new Dialogue({ "Good...",
                                             "Very well...",
                                             "Thanks to your determination, the flutes are finally secured.",
                                             "The world will never know the hell this demon had in store for it...",
                                             "Our lineage can finally rest in peace. Adios." },
        BabyMan::tex, BabyMan::dialogMusic);

    this->isRunning = true;
    this->dialogueFinished = false;

    this->player = new Player("Player", { 0.0f, 0.0f, 0.0f });
}

void VictoryBabyScene::events()
{
    if (this->currentDialogue) {
        this->currentDialogue->handleInput();
        return;
    }

    if (this->dialogueFinished) {
        if (IsKeyPressed(KEY_E)) {
            this->currScene = 3;
            this->isRunning = false;
        }
        if (IsKeyPressed(KEY_Q)) {
            CloseWindow();
        }
    }
}

void VictoryBabyScene::update(float dt)
{
    if (this->currentDialogue) {
        this->currentDialogue->update(dt);
        if (this->currentDialogue->isFinished()) {
            this->currentDialogue->reset();
            this->currentDialogue = nullptr;
            this->player->canMove = true;
            this->dialogueFinished = true;
        }
    }
}

void VictoryBabyScene::draw() const
{
    BeginDrawing();
    ClearBackground(BLACK);

    if (this->currentDialogue) {
        this->currentDialogue->draw(this->width, this->height);
    } else if (this->dialogueFinished) {
        int fontSize = 40;
        int startY = this->height / 3;
        int centerX = this->width / 2;

        DrawText("What is your decision?", centerX - 250, startY, fontSize,
            GOLD);

        DrawText("E: Restart", centerX - 250, startY + 60, fontSize, WHITE);
        DrawText("Q: Quit", centerX - 250, startY + 120, fontSize, WHITE);

        DrawText("Select a key to continue...", centerX - 250,
            startY + 200, 20, GRAY);
    }

    EndDrawing();
}

void VictoryBabyScene::reset()
{
    delete this->player;
    this->player = nullptr;
}

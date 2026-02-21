#include "VictoryPlayerScene.h"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../Entity/Entities.h"

VictoryPlayerScene::VictoryPlayerScene(int width, int height, int& currScene)
    : Scene("Victory", width, height, wallTexture)
    , currScene(currScene)
{
    this->init();
}

VictoryPlayerScene::~VictoryPlayerScene()
{
    if (player) {
        delete player;
        player = nullptr;
    }
}

void VictoryPlayerScene::init()
{
    this->currentDialogue = new Dialogue({ "And so your labor comes to an end... The flutes are mine.",
                                             "You have been a docile instrument, ignorant of the chaos you unleashed.",
                                             "Here is your reward: I grant you the privilege of being the first witness to my glory.",
                                             "The world will finally know its true melody...",
                                             "Farewell, mortal. We shall not meet again in this reality." },
        PizzaMan::tex, PizzaMan::dialogMusic);

    this->isRunning = true;
    this->dialogueFinished = false;

    this->player = new Player("Player", { 0.0f, 0.0f, 0.0f });
}

void VictoryPlayerScene::events()
{
    if (this->currentDialogue) {
        this->currentDialogue->handleInput();
        return;
    }

    if (this->dialogueFinished) {
        if (IsKeyPressed(KEY_E)) {
            this->currScene = 0;
            this->isRunning = false;
        }
        if (IsKeyPressed(KEY_Q)) {
            CloseWindow();
        }
    }
}

void VictoryPlayerScene::update(float dt)
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

void VictoryPlayerScene::draw() const
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

void VictoryPlayerScene::reset()
{
    delete this->player;
    this->player = nullptr;
}

#include "VictoryPlayerSceneWeird.h"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../Entity/Entities.h"

VictoryPlayerSceneWeird::VictoryPlayerSceneWeird(int width, int height,
    int& currScene)
    : Scene("Victory", width, height, wallTexture)
    , currScene(currScene)
{
    this->init();
}

VictoryPlayerSceneWeird::~VictoryPlayerSceneWeird()
{
    if (player) {
        delete player;
        player = nullptr;
    }
}

void VictoryPlayerSceneWeird::init()
{
    this->currentDialogue = new Dialogue({ "This is... unexpected.",
                                             "Silence has fallen over the nursery... You actually took down their leader.",
                                             "Without my flutes, my power remains incomplete, but you've done the hardest part.",
                                             "You are a far darker creature than I imagined for a mere mortal.",
                                             "Go... We shall meet again when the time comes to claim my throne.",
                                             "Your pizza awaits you. Go join it" },
        PizzaMan::tex, PizzaMan::dialogMusic);

    this->isRunning = true;
    this->dialogueFinished = false;

    this->player = new Player("Player", { 0.0f, 0.0f, 0.0f });
}

void VictoryPlayerSceneWeird::events()
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

void VictoryPlayerSceneWeird::update(float dt)
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

void VictoryPlayerSceneWeird::draw() const
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

void VictoryPlayerSceneWeird::reset()
{
    delete this->player;
    this->player = nullptr;
}

#include "VictoryBabySceneWeird.h"

#include <raylib.h>
#include <raymath.h>

#include <vector>

#include "../../Entity/Entities.h"

VictoryBabySceneWeird::VictoryBabySceneWeird(int width, int height, int& currScene)
    : Scene("Victory", width, height, wallTexture)
    , currScene(currScene)
{
    this->init();
}

VictoryBabySceneWeird::~VictoryBabySceneWeird()
{
    if (player) {
        delete player;
        player = nullptr;
    }
}

void VictoryBabySceneWeird::init()
{
    this->currentDialogue = new Dialogue({ "The deed is done...",
                                             "The usurper will never torment the living with his presence again.",
                                             "Through your courage, you have broken the cycle of his millennial tyranny.",
                                             "Know that your name will remain engraved in the annals of our order.",
                                             "Millions of lives breathe today thanks to you." },
        BabyMan::tex, BabyMan::dialogMusic);

    this->isRunning = true;
    this->dialogueFinished = false;

    this->player = new Player("Player", { 0.0f, 0.0f, 0.0f });
}

void VictoryBabySceneWeird::events()
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

void VictoryBabySceneWeird::update(float dt)
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

void VictoryBabySceneWeird::draw() const
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

void VictoryBabySceneWeird::reset()
{
    delete this->player;
    this->player = nullptr;
}

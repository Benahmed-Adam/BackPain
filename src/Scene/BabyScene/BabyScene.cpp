#include "BabyScene.h"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../Entity/Entities.h"

Texture2D BabyScene::wallTex;
Sound BabyScene::successSound;
Sound BabyScene::reviveSound;
Music BabyScene::theme;

void BabyScene::load()
{
    BabyScene::wallTex = LoadTexture("assets/wallTextureBaby.png");
    BabyScene::successSound = LoadSound("assets/Success.mp3");
    BabyScene::reviveSound = LoadSound("assets/Revive.mp3");
    BabyScene::theme = LoadMusicStream("assets/BabySceneTheme.mp3");
}

void BabyScene::unload()
{
    UnloadTexture(BabyScene::wallTex);
    UnloadSound(BabyScene::successSound);
    UnloadSound(BabyScene::reviveSound);
    UnloadMusicStream(BabyScene::theme);
}

BabyScene::BabyScene(int width, int height, int& currScene)
    : Scene("Baby ?", width, height, BabyScene::wallTex)
    , currScene(currScene)
{
    this->isRunning = false;
    this->pathfinder = new PathFinder(*this);
    this->currentDialogue = nullptr;
    this->map = {
        "11111111111111111111111111111111111111111111111111111111111111111",
        "1000000000000000000000000100000j000001000000000000000000000000001",
        "10000000000000000000000001000000000001000000000000000000000000001",
        "100000000000000011111111111B0000000011111111111000000000000000001",
        "11111111111100001111111111111110111111111111111000011111111111111",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "100f0000000000000000000000000000000000000000000000000000000000001",
        "10000000111111100001111111111111111111111111100001111111000000001",
        "11111000111111100001111111111111111111111111100001111111000111111",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "10000000000000000000000f00000000000000000000000000000000000000001",
        "10000000111111111100000000000000000000000000001111111111000000001",
        "11111000111111111100000111111100011111110000001111111111000111111",
        "10000000000000000000000111111100011111110000000000000000000000001",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "10011111110000111111000000000000000000000000111111000011111110001",
        "10011111110000111111000000000000000000000000111111000011111110001",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "10000000000000000000000111111111111111000000000000000000000000101",
        "11111000111111111100000111111111111111000000011111111110001111111",
        "11111000111111111100000000000000000000000000011111111110001111111",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "100000000J000000000000000000000000000000000000000000000000J000001",
        "100000001111111111000001111111111111110000001111111111f0000000001",
        "10011111111111111100000111111111111111000000111111111111111100001",
        "10000000000000000000000000000111000000000000000000000000000000001",
        "100f0000000000000000000000000111000000000000J00000000000000111101",
        "100000001111111111110000000001110000000011011111111111111111p0001",
        "10000000000000000000000J0000001110000000000000000000000000000J0001",
        "11111111111111111111111111111111111111111111111111111111111111111",
    };
}

BabyScene::~BabyScene()
{
    for (Entity* e : this->entities) {
        delete e;
    }

    entities.clear();

    if (player) {
        delete player;
        player = nullptr;
    }

    delete this->pathfinder;
    StopMusicStream(BabyScene::theme);
}

void BabyScene::init()
{
    entities.clear();

    this->isRunning = true;

    PlaySound(BabyScene::reviveSound);

    PlayMusicStream(BabyScene::theme);
    SetMusicVolume(BabyScene::theme, 0.75);

    this->player = new Player("Player", { 0.0f, 0.0f, 0.0f });
    this->player->role = 1;

    const float size = 5.0f;

    int currDialogueBebe = 0;
    std::vector<std::vector<std::string>> dialoguesBebe = { { "Soldier, the hour is grave. The usurper is at our gates.",
        "This demon disguised as a delivery man seeks to regain control of the melody.",
        "His magic flutes are the pillars that maintain the balance of this world.",
        "If he retrieves them, he will unleash a power that no one can contain.",
        "Retrieve the remaining relics. The salvation of our lineage depends on it." } };

    int currDialoguePizza = 0;
    std::vector<std::vector<std::string>> dialoguesPizza = { { "You again... Miserable creature. You dare stand in my way?",
        "Your kind stole what belongs to me by divine right.",
        "I will stop at nothing to purge this world of your presence.",
        "Every second you breathe is an insult to my power.",
        "Return my instruments to me, or suffer the consequences of your arrogance.",
        "The human will come help me in any case..." } };

    for (int x = 0; x < this->map.size(); x++) {
        const std::string& row = this->map[x];

        for (int y = 0; y < row.length(); y++) {
            Vector3 pos = { (float)x * size, 1, (float)y * size };
            char tileType = row[y];
            if (tileType == 'p') {
                if (currDialoguePizza >= dialoguesPizza.size()) {
                    std::cout << "Not enough dialogue for pizzamen"
                              << std::endl;
                } else {
                    this->entities.push_back(
                        new PizzaMan(pos, { 5.0f, 5.0f },
                            dialoguesPizza[currDialoguePizza++]));
                }
            } else if (tileType == 'B') {
                if (currDialogueBebe >= dialoguesBebe.size()) {
                    std::cout << "Not enough dialogue for babies"
                              << std::endl;
                } else {
                    this->entities.push_back(new BabyMan(
                        pos, { 5.0f, 5.0f }, dialoguesBebe[currDialogueBebe++]));
                }
            } else if (tileType == 'j') {
                this->player->setPosition(pos);
            } else if (tileType == 'f') {
                this->entities.push_back(new Flute(pos, { 2.0, 5.0 }));
            } else if (tileType == 'J') {
                this->entities.push_back(new PlayerEnemy(pos, { 5.0, 5.0 }, this->pathfinder, this->player, this));
            }
        }
    }
}

void BabyScene::events()
{
    if (this->currentDialogue) {
        this->currentDialogue->handleInput();
        return;
    }

    // if (IsKeyPressed(KEY_R)) {
    //     this->reset();
    //     this->init();
    // }

    if (IsKeyPressed(KEY_E)) {
        for (Entity* e : this->entities) {
            if (e->getName().substr(0, 8) == "PizzaMan" && e->isActive()) {
                if (this->player->distanceTo(e->getPosition()) < 5) {
                    PizzaMan* pizzaman = static_cast<PizzaMan*>(e);

                    Vector3 playerPos = this->player->getPosition();
                    Vector3 pizzamanPos = pizzaman->getPosition();
                    Vector3 direction = Vector3Normalize(
                        Vector3Subtract(pizzamanPos, playerPos));

                    float yaw = atan2f(direction.x, direction.z);

                    this->player->setRotation(yaw, 0);
                    this->currentDialogue = pizzaman->dial;
                    this->player->canMove = false;
                    break;
                }
            }

            if (e->getName().substr(0, 7) == "BabyMan" && e->isActive()) {
                if (this->player->distanceTo(e->getPosition()) < 5) {
                    BabyMan* babymanman = static_cast<BabyMan*>(e);

                    Vector3 playerPos = this->player->getPosition();
                    Vector3 babymanPos = babymanman->getPosition();
                    Vector3 direction = Vector3Normalize(
                        Vector3Subtract(babymanPos, playerPos));

                    float yaw = atan2f(direction.x, direction.z);

                    this->player->setRotation(yaw, 0);
                    this->currentDialogue = babymanman->dial;
                    this->player->canMove = false;
                    break;
                }
            }

            if (e->getName().substr(0, 5) == "Flute" && e->isActive()) {
                if (this->player->distanceTo(e->getPosition()) < 5) {
                    e->setActive(false);
                    PlaySound(BabyScene::successSound);
                    if (this->player->score++ == 3) {
                        StopMusicStream(BabyScene::theme);
                        this->isRunning = false;
                        this->currScene = 4;
                    }
                    break;
                }
            }
        }
    }
}

void BabyScene::update(float dt)
{
    UpdateMusicStream(BabyScene::theme);

    if (this->player->health == 0) {
        StopMusicStream(BabyScene::theme);
        this->currScene = 0;
        this->isRunning = false;
        return;
    }

    Vector3 oldPos = this->player->getPosition();
    this->player->update(dt);

    if (this->currentDialogue) {
        this->currentDialogue->update(dt);
        if (this->currentDialogue->isFinished()) {
            this->currentDialogue->reset();
            this->currentDialogue = nullptr;
            this->player->canMove = true;
        }
        return;
    }

    if (this->player->canMove) {
        Vector3 newPos = this->player->getPosition();

        const float size = 5.0f;
        const float playerRadius = 0.4f;

        int mapX_XCheck = static_cast<int>(roundf(
            (newPos.x + (newPos.x > oldPos.x ? playerRadius : -playerRadius))
            / size));
        int mapZ_XCheck = static_cast<int>(roundf(oldPos.z / size));

        if (mapX_XCheck < 0
            || static_cast<size_t>(mapX_XCheck) >= this->map.size()
            || mapZ_XCheck < 0
            || static_cast<size_t>(mapZ_XCheck)
                >= this->map[mapX_XCheck].length()
            || this->map[mapX_XCheck][mapZ_XCheck] == '1') {
            newPos.x = oldPos.x;
        }

        int mapX_ZCheck = static_cast<int>(roundf(newPos.x / size));
        int mapZ_ZCheck = static_cast<int>(roundf(
            (newPos.z + (newPos.z > oldPos.z ? playerRadius : -playerRadius))
            / size));

        if (mapX_ZCheck < 0
            || static_cast<size_t>(mapX_ZCheck) >= this->map.size()
            || mapZ_ZCheck < 0
            || static_cast<size_t>(mapZ_ZCheck)
                >= this->map[mapX_ZCheck].length()
            || this->map[mapX_ZCheck][mapZ_ZCheck] == '1') {
            newPos.z = oldPos.z;
        }

        this->player->setPosition(newPos);
    }

    std::vector<Entity*> newEntities;

    for (Entity* e : this->entities) {
        e->update(dt);

        if (e->getName().substr(0, 8) == "FireBall" && e->isActive()) {
            bool collided = false;

            const float size = 5.0f;
            Vector3 pos = e->getPosition();
            int mapX = static_cast<int>(roundf(pos.x / size));
            int mapZ = static_cast<int>(roundf(pos.z / size));

            if (mapX >= 0 && static_cast<size_t>(mapX) < this->map.size()
                && mapZ >= 0
                && static_cast<size_t>(mapZ) < this->map[mapX].length()
                && this->map[mapX][mapZ] == '1') {
                newEntities.push_back(
                    new Explosion(e->getPosition(), { 5.0f, 5.0f }));
                collided = true;
            } else {
                if (CheckCollisionSpheres(e->getPosition(), e->getRadius(), this->player->getPosition(), this->player->getRadius())) {
                    this->player->takeDamage(25);
                    newEntities.push_back(new Explosion(this->player->getPosition(), { 5.0f, 5.0f }));
                    collided = true;
                } else {
                    for (Entity* target : this->entities) {
                        if (target != e && target->isActive()
                            && target->getName().substr(0, 9) != "Explosion"
                            && target->getName().substr(0, 8) != "FireBall"
                            && target->getName().substr(0, 5) != "Flute"
                            && target->getName().substr(0, 11) != "PlayerEnemy") {
                            if (CheckCollisionSpheres(e->getPosition(),
                                    e->getRadius() * 0.5,
                                    target->getPosition(),
                                    target->getRadius() * 0.5)) {
                                target->setActive(false);
                                newEntities.push_back(new Explosion(
                                    target->getPosition(), { 5.0f, 5.0f }));
                                collided = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (collided) {
                e->setActive(false);
            }
        }
    }

    for (size_t i = 0; i < this->entities.size();) {
        if (!this->entities[i]->isActive()) {
            if (this->entities[i]->getName().substr(0, 8) == "PizzaMan") {
                StopMusicStream(BabyScene::theme);
                this->currScene = 5;
                this->isRunning = false;
            }
            delete this->entities[i];
            this->entities.erase(this->entities.begin() + i);
        } else {
            ++i;
        }
    }

    this->entities.insert(this->entities.end(), newEntities.begin(),
        newEntities.end());
}

void BabyScene::draw() const
{
    BeginDrawing();
    ClearBackground(WHITE);

    const Camera3D camera = this->player->getCamera();
    BeginMode3D(camera);
    this->drawMap();
    // DrawGrid(20, 1.0f);

    std::vector<Entity*> sortedEntities = this->entities;
    std::sort(sortedEntities.begin(), sortedEntities.end(),
        [&camera](Entity* a, Entity* b) {
            return a->distanceTo(camera.position)
                > b->distanceTo(camera.position);
        });

    for (Entity* e : sortedEntities)
        e->draw(camera);

    EndMode3D();

    this->player->drawUI();

    if (this->currentDialogue) {
        this->currentDialogue->draw(this->width, this->height);
    } else {
        for (Entity* e : this->entities) {
            if (e->getName().substr(0, 8) == "PizzaMan" && e->isActive()
                || e->getName().substr(0, 7) == "BabyMan" && e->isActive()
                || e->getName().substr(0, 5) == "Flute" && e->isActive()) {
                if (this->player->distanceTo(e->getPosition()) < 5) {
                    const char* text = "Press E to interact";
                    float fontSize = GetScreenHeight() / 20.0f;
                    int textWidth = MeasureText(text, fontSize);
                    DrawText(text, (GetScreenWidth() - textWidth) / 2,
                        GetScreenHeight() * 0.2f, fontSize, BLACK);
                }
            }
        }
    }

    EndDrawing();
}

void BabyScene::reset()
{
    for (Entity* e : this->entities) {
        delete e;
    }
    this->entities.erase(this->entities.begin(), this->entities.end());
    delete this->player;
    this->player = nullptr;
}

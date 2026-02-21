#include "PlayerScene.h"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../Entity/Entities.h"

Texture2D PlayerScene::wallTex;
Sound PlayerScene::successSound;
Sound PlayerScene::reviveSound;
Music PlayerScene::theme;

void PlayerScene::load()
{
    PlayerScene::wallTex = LoadTexture("assets/wallTexture.jpg");
    PlayerScene::successSound = LoadSound("assets/Success.mp3");
    PlayerScene::reviveSound = LoadSound("assets/Revive.mp3");
    PlayerScene::theme = LoadMusicStream("assets/PlayerSceneTheme.mp3");
}

void PlayerScene::unload()
{
    UnloadTexture(PlayerScene::wallTex);
    UnloadSound(PlayerScene::successSound);
    UnloadSound(PlayerScene::reviveSound);
    UnloadMusicStream(PlayerScene::theme);
}

PlayerScene::PlayerScene(int width, int height, int& currScene)
    : Scene("Player", width, height, PlayerScene::wallTex)
    , currScene(currScene)
{
    this->isRunning = false;
    this->pathfinder = new PathFinder(*this);
    this->currentDialogue = nullptr;
    this->damageCooldown = 0.0f;
    this->fireballCooldown = 0.0f;
    this->map = {
        "11111111111111111111111111111111111111111111111111111111111111111",
        "100000000001Bf000000000000000000000000000000000000000000000000001",
        "1j00p000000111111111111111000000000000000000000000000000000000001",
        "10000000000000001111111111000000000001111111111000000000000000001",
        "11111111111100001111111111000000000001111111111000011111111111111",
        "10000000000000000001000000000000000000000000000000000000000000001",
        "10000000000000000001000000000000000000000000000000000000000000001",
        "10000000111111100001111111111111111111111111100001111111000000001",
        "11111000111111100001111111111111111111111111100001111111000111111",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "10000000000000000000000f00000000000000000000000000000000000000001",
        "10000000111111111100000000000000000000000000001111111111000000001",
        "11111000111111111100000111111100011111110000001111111111000111111",
        "10000000000000000000000111111100011111110000000000000000000000001",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "1001111111000011111100000000000000b00000000111111000011111110001",
        "1001111111000011111100000000000000000000000111111000011111110001",
        "10000000000000000000000000000000000000000000000000000000000000001",
        "1000000000000b000000000111111111111111000000000000b00000000000001",
        "1111100011111111110000011111111111111100000011111111110001111111",
        "1111100011111111110000000000000000000000000011111111110001111111",
        "1000000000000000000b000000000000b000000000b0000000000000000000001",
        "100b0000000b000000000000000000000b0000000000000000b0000000000f001",
        "1000000011111111110000011111111111111100000111111111100000000001",
        "10011111111111111100000100000000000001100000111111111111111100001",
        "10000000000000000b00000111111111011111bb0000000b000000b0000000001",
        "100f000000000000000000b0000b000b00000b000000000000b00000000000001",
        "10000b0011111111111100000000011000000000111111111111111111100001",
        "1000000000000000000b000b0000011000b000000000000000000000000b00001",
        "11111111111111111111111111111111111111111111111111111111111111111",
    };
}

PlayerScene::~PlayerScene()
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
    StopMusicStream(PlayerScene::theme);
}

void PlayerScene::init()
{
    entities.clear();

    this->isRunning = true;

    PlaySound(PlayerScene::reviveSound);

    PlayMusicStream(PlayerScene::theme);
    SetMusicVolume(PlayerScene::theme, 0.75);

    this->player = new Player("Player", { 0.0f, 0.0f, 0.0f });

    const float size = 5.0f;

    int currDialoguePizza = 0;
    std::vector<std::vector<std::string>> dialoguesPizza = { { "What a catastrophe... My fate is sealed if you don't help me!",
        "Babies with more than dubious intentions stole my flutes.",
        "Without them, I cannot soothe my back pain",
        "Retrieve them and I'll offer you this magnificent pizza!",
        "Hurry, every second counts for my survival (my back)." } };

    int currDialogueBebe = 0;
    std::vector<std::vector<std::string>> dialoguesBebe = { { "Step aside, mortal. You don't understand the magnitude of your actions.",
        "These flutes must never return to the Delivery Man's hands.",
        "You are blinded by promises of trinkets and food.",
        "Silence is our only defense against his chaos.",
        "Leave us... before it's too late for this world." } };

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
            } else if (tileType == 'b') {
                this->entities.push_back(new Baby(
                    pos, { 5.0, 5.0 }, this->pathfinder, this->player));
            } else if (tileType == 'f') {
                this->entities.push_back(new Flute(pos, { 2.0, 5.0 }));
            }
        }
    }
}

void PlayerScene::events()
{
    if (this->currentDialogue) {
        this->currentDialogue->handleInput();
        return;
    }

    // if (IsKeyPressed(KEY_R)) {
    //     this->reset();
    //     this->init();
    // }

    if (this->player->canMove) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && this->fireballCooldown <= 0.0f) {
            const Camera3D& cam = this->player->getCamera();
            Vector3 dir = this->player->getViewDirection();
            Vector3 spawnPos
                = Vector3Add(cam.position, Vector3Scale(dir, 0.2f));
            this->entities.push_back(
                new FireBall(dir, spawnPos, { 10.0f, 10.0f }));
            this->fireballCooldown = 1.5f;
        }
    }

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
                    PlaySound(PlayerScene::successSound);
                    if (this->player->score++ == 3) {
                        StopMusicStream(PlayerScene::theme);
                        this->isRunning = false;
                        this->currScene = 1;
                    }
                    break;
                }
            }
        }
    }
}

void PlayerScene::update(float dt)
{
    UpdateMusicStream(PlayerScene::theme);

    if (this->player->health == 0) {
        StopMusicStream(PlayerScene::theme);
        this->currScene = 3;
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

    if (this->damageCooldown > 0) {
        this->damageCooldown -= dt;
    }

    if (this->fireballCooldown > 0.0f) {
        this->fireballCooldown -= dt;
    }

    std::vector<Entity*> newEntities;

    for (Entity* e : this->entities) {
        e->update(dt);

        if (e->getName().substr(0, 4) == "Baby" && e->isActive()) {
            if (this->damageCooldown <= 0.0f
                && CheckCollisionSpheres(this->player->getPosition(),
                    this->player->getRadius(),
                    e->getPosition(),
                    e->getRadius())) {
                this->player->takeDamage(10.0f);
                this->damageCooldown = 0.1f;
            }
        }

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
                for (Entity* target : this->entities) {
                    if (target != e && target->isActive()
                        && target->getName().substr(0, 9) != "Explosion"
                        && target->getName().substr(0, 8) != "FireBall"
                        && target->getName().substr(0, 5) != "Flute") {
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

            if (collided) {
                e->setActive(false);
            }
        }
    }

    for (size_t i = 0; i < this->entities.size();) {
        if (!this->entities[i]->isActive()) {
            if (this->entities[i]->getName().substr(0, 7) == "BabyMan") {
                StopMusicStream(PlayerScene::theme);
                this->currScene = 2;
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

void PlayerScene::draw() const
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

void PlayerScene::reset()
{
    for (Entity* e : this->entities) {
        delete e;
    }
    this->entities.erase(this->entities.begin(), this->entities.end());
    delete this->player;
    this->player = nullptr;
    this->damageCooldown = 0.0f;
}

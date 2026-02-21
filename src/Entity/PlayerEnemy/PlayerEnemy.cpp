#include "PlayerEnemy.h"

#include <cmath>

#include "../../utils/utils.h"
#include "../AnimatedEntity/FireBall/FireBall.h"
#include "raymath.h"

Texture2D PlayerEnemy::tex;

void PlayerEnemy::load()
{
    PlayerEnemy::tex = LoadTexture("assets/PlayerEnemy.png");
}

void PlayerEnemy::unload()
{
    UnloadTexture(PlayerEnemy::tex);
}

PlayerEnemy::PlayerEnemy(const Vector3& pos, const Vector2& scale,
    PathFinder* pathfinder, Player* player, Scene* scene)
    : Entity("PlayerEnemy_", pos, PlayerEnemy::tex, scale)
    , pathfinder(pathfinder)
    , player(player)
    , scene(scene)
    , shootTimer(0.f)

{
    this->speed = rand() % 5 + 10;
}

PlayerEnemy::~PlayerEnemy() { }

void PlayerEnemy::draw(const Camera3D& camera) const
{
    DrawBillboard(camera, this->texture, this->position, this->scale.x, WHITE);
}

void PlayerEnemy::update(float dt)
{
    if (this->pathfinder && this->player
        && this->distanceTo(this->player->getPosition())) {
        if (this->pathfinder->hasLineOfSight(
                this->position, this->player->getPosition())) {
            this->shootTimer += dt;
            if (this->shootTimer >= this->shootInterval) {
                this->shootTimer = 0.f;
                Vector3 direction = Vector3Normalize(
                    Vector3Subtract(this->player->getPosition(), this->position));
                FireBall* fireBall
                    = new FireBall(direction, this->position, { 5, 5 });
                this->scene->addEntity(fireBall);
            }
        }

        Vector3 nextPos = this->pathfinder->getNextPosition(
            this->position, this->player->getPosition());
        Vector3 direction
            = Vector3Normalize(Vector3Subtract(nextPos, this->position));
        this->position = Vector3Add(
            this->position, Vector3Scale(direction, this->speed * dt));
    }
}
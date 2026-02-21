#include "Baby.h"

#include <raymath.h>

#include <cstdlib>
#include <string>

#include "../../../utils/utils.h"

Texture2D Baby::tex;

void Baby::load()
{
    Baby::tex = LoadTexture("assets/BabySpriteSheet.png");
}

void Baby::unload()
{
    UnloadTexture(Baby::tex);
}

Baby::Baby(const Vector3& pos, const Vector2& scale, PathFinder* pathfinder,
    Player* player)
    : AnimatedEntity("Baby_", pos, Baby::tex, 133, 211, 6, 0.05f, scale)
    , pathfinder(pathfinder)
    , player(player)
{
    this->speed = rand() % 15 + 5;
}

Baby::~Baby() { }

void Baby::update(float dt)
{
    AnimatedEntity::update(dt);

    if (this->pathfinder && this->player && this->distanceTo(this->player->getPosition()) < 100.0f) {
        Vector3 nextPos = this->pathfinder->getNextPosition(
            this->position, this->player->getPosition());
        Vector3 direction
            = Vector3Normalize(Vector3Subtract(nextPos, this->position));
        this->position = Vector3Add(this->position,
            Vector3Scale(direction, this->speed * dt));
    }
}
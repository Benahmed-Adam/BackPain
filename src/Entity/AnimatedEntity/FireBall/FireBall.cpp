#include "FireBall.h"

#include <raymath.h>

#include <iostream>
#include <string>

#include "../../../utils/utils.h"

float FireBall::distanceBeforeDeath = 500.0f;
float FireBall::speed = 500.0f;
Texture2D FireBall::tex;

void FireBall::load()
{
    FireBall::tex = LoadTexture("assets/firesheet5x5.png");
}

void FireBall::unload()
{
    UnloadTexture(FireBall::tex);
}

FireBall::FireBall(const Vector3& direction, const Vector3& startPos,
    const Vector2& scale)
    : AnimatedEntity("FireBall_", startPos, FireBall::tex, 100, 100, 25,
          0.025f, scale)
{
    this->distance = 0.0f;
    this->setVelocity(direction * FireBall::speed);
}

FireBall::~FireBall() { }

void FireBall::update(float dt)
{
    AnimatedEntity::update(dt);
    this->distance += FireBall::speed * dt;
    if (this->distance > FireBall::distanceBeforeDeath) {
        this->active = false;
    }
}
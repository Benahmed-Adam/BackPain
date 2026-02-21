#include "Flute.h"

#include <cmath>

#include "../../utils/utils.h"

Texture2D Flute::tex;

void Flute::load()
{
    Flute::tex = LoadTexture("assets/flute.png");
}

void Flute::unload()
{
    UnloadTexture(Flute::tex);
}

Flute::Flute(const Vector3& position, const Vector2& scale)
    : Entity("Flute_", position, Flute::tex, scale)
{
    this->time = 0.f;
    this->initialY = this->position.y;
}

Flute::~Flute() { }

void Flute::draw(const Camera3D& camera) const
{
    DrawBillboard(camera, this->texture, this->position, this->scale.x, WHITE);
}

void Flute::update(float dt)
{
    this->time += dt;
    this->position.y = this->initialY + sin(this->time * 2.f);
}
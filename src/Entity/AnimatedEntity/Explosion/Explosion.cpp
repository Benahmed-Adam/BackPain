#include "Explosion.h"

#include <raymath.h>

#include <string>

#include "../../../utils/utils.h"

Texture2D Explosion::tex;
Sound Explosion::explosionSound;

void Explosion::load()
{
    Explosion::tex = LoadTexture("assets/explosionSpriteSheet.png");
    Explosion::explosionSound = LoadSound("assets/ExplosionSound.wav");
}

void Explosion::unload()
{
    UnloadTexture(Explosion::tex);
    UnloadSound(Explosion::explosionSound);
}

Explosion::Explosion(const Vector3& pos, const Vector2& scale)
    : AnimatedEntity("Explosion_", pos, Explosion::tex, 67, 67, 16, 0.05f,
          scale)
{
    this->s = LoadSoundAlias(Explosion::explosionSound);
    SetSoundVolume(this->s, 0.2);
    PlaySound(this->s);
}

Explosion::~Explosion() { UnloadSoundAlias(this->s); }

void Explosion::draw(const Camera3D& camera) const
{
    if (!this->cycle) {
        AnimatedEntity::draw(camera);
    }
}

void Explosion::update(float dt)
{
    AnimatedEntity::update(dt);
    if (!IsSoundPlaying(this->s)) {
        this->active = false;
    }
}
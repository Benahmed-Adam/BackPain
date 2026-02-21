#include "AnimatedEntity.h"

#include <iostream>

AnimatedEntity::AnimatedEntity(const std::string& name, Vector3 position,
    Texture2D& texture, int frameWidth,
    int frameHeight, int frameCount,
    float frameTime, const Vector2& scale)
    : Entity(name, position, texture, scale)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->frameCount = frameCount;
    this->frameTime = frameTime;
    this->currentFrame = 0.0f;
    this->frameTimer = 0.0f;
    this->cycle = false;
}

AnimatedEntity::~AnimatedEntity() { }

void AnimatedEntity::draw(const Camera3D& camera) const
{
    Rectangle rect = this->getFrameRectangle();
    DrawBillboardRec(camera, this->texture, rect, this->position, this->scale,
        WHITE);
    // DrawBillboard(camera, this->texture, this->position, this->scale.x,
    // WHITE);
}

void AnimatedEntity::update(float dt)
{
    Entity::update(dt);
    this->frameTimer += dt;
    if (this->frameTimer > this->frameTime) {
        if (this->currentFrame + 1 == this->frameCount) {
            this->cycle = true;
        }

        this->currentFrame = (this->currentFrame + 1) % this->frameCount;
        this->frameTimer = 0.0f;
    }
}

int AnimatedEntity::getCurrentFrame() const
{
    return this->currentFrame;
}

void AnimatedEntity::setCurrentFrame(int frame)
{
    this->currentFrame = frame;
}

void AnimatedEntity::setFrameTime(float time)
{
    this->frameTime = time;
}

float AnimatedEntity::getFrameTime() const
{
    return this->frameTime;
}

Rectangle
AnimatedEntity::getFrameRectangle() const
{
    Rectangle rect;
    int framesPerRow = this->texture.width / this->frameWidth;
    rect.x = static_cast<float>((this->currentFrame % framesPerRow)
        * this->frameWidth);
    rect.y = static_cast<float>((this->currentFrame / framesPerRow)
        * this->frameHeight);
    rect.width = static_cast<float>(this->frameWidth);
    rect.height = static_cast<float>(this->frameHeight);
    return rect;
}

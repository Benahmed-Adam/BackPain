#ifndef __ANIMATED_ENTITY_H__
#define __ANIMATED_ENTITY_H__

#include "../Entity.h"

class AnimatedEntity : public Entity {
public:
    AnimatedEntity(const std::string& name, Vector3 position,
        Texture2D& texture, int frameWidth, int frameHeight,
        int frameCount, float frameTime, const Vector2& scale);
    virtual ~AnimatedEntity();

    virtual void draw(const Camera3D& camera) const;
    virtual void update(float dt);

    int getCurrentFrame() const;
    void setCurrentFrame(int frame);
    void setFrameTime(float time);
    float getFrameTime() const;

protected:
    int frameWidth;
    int frameHeight;
    int frameCount; // nombre de frames dans le spritesheet
    int currentFrame;
    float frameTime; /// temps entre deux images
    float frameTimer; // incrémentateur d'une frame
    bool cycle;

    Rectangle getFrameRectangle() const;
};

#endif

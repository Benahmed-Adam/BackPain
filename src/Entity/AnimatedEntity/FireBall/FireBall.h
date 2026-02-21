#ifndef __FIREBALL_H__
#define __FIREBALL_H__

#include "../AnimatedEntity.h"

class FireBall : public AnimatedEntity {
public:
    static Texture2D tex;
    static void load();
    static void unload();
    FireBall(const Vector3& direction, const Vector3& startPos,
        const Vector2& scale);
    ~FireBall();
    void update(float dt);

private:
    float distance;
    static float distanceBeforeDeath;
    static float speed;
};

#endif
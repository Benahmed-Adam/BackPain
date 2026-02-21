#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "../AnimatedEntity.h"

class Explosion : public AnimatedEntity {
public:
    static Texture2D tex;
    static Sound explosionSound;
    static void load();
    static void unload();
    Explosion(const Vector3& pos, const Vector2& scale);
    ~Explosion();
    void update(float dt);
    virtual void draw(const Camera3D& camera) const;

private:
    Sound s;
};

#endif
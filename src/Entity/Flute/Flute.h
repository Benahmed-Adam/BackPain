#ifndef __FLUTE_H__
#define __FLUTE_H__

#include "../Entity.h"

class Flute : public Entity {
private:
    float time;
    float initialY;

public:
    Flute(const Vector3& position, const Vector2& scale);
    virtual ~Flute();

    static Texture2D tex;
    static void load();
    static void unload();

    virtual void draw(const Camera3D& camera) const;
    virtual void update(float dt);
};

#endif
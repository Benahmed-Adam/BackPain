#ifndef __PlayerEnemy_H__
#define __PlayerEnemy_H__

#include "../../PathFinder/PathFinder.h"
#include "../../Scene/Scene.h"
#include "../Entity.h"

class Player;

class PlayerEnemy : public Entity {
public:
    static Texture2D tex;
    static void load();
    static void unload();
    PlayerEnemy(const Vector3& pos, const Vector2& scale, PathFinder* pathfinder,
        Player* player, Scene* scene);
    ~PlayerEnemy();
    void update(float dt);
    virtual void draw(const Camera3D& camera) const;

private:
    float speed;
    PathFinder* pathfinder;
    Player* player;
    Scene* scene;
    float shootTimer;
    const float shootInterval = 2.0f;
};

#endif
#ifndef __BABY_H__
#define __BABY_H__

#include "../../../PathFinder/PathFinder.h"
#include "../../Player/Player.h"
#include "../AnimatedEntity.h"

class Baby : public AnimatedEntity {
public:
    static Texture2D tex;
    static void load();
    static void unload();
    Baby(const Vector3& pos, const Vector2& scale, PathFinder* pathfinder,
        Player* player);
    ~Baby();
    void update(float dt);

private:
    float speed;
    PathFinder* pathfinder;
    Player* player;
};

#endif
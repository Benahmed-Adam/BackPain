#ifndef __PlayerScene_H__
#define __PlayerScene_H__

#include "../../Dialogue/Dialogue.h"
#include "../../PathFinder/PathFinder.h"
#include "../Scene.h"

class PlayerScene : public Scene {
public:
    PlayerScene(int width, int height, int& currScene);
    virtual ~PlayerScene();

    static Texture2D wallTex;
    static Sound successSound;
    static Sound reviveSound;
    static Music theme;

    static void load();
    static void unload();

    virtual void events();
    virtual void update(float dt);
    virtual void draw() const;

    virtual void init();

    virtual void reset();

private:
    Dialogue* currentDialogue;
    PathFinder* pathfinder;

    int& currScene;

    float damageCooldown;
    float fireballCooldown;
};

#endif
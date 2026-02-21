#ifndef __BabyScene_H__
#define __BabyScene_H__

#include "../../Dialogue/Dialogue.h"
#include "../../PathFinder/PathFinder.h"
#include "../Scene.h"

class BabyScene : public Scene {
public:
    BabyScene(int width, int height, int& currScene);
    virtual ~BabyScene();

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
};

#endif
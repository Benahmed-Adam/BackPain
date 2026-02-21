#ifndef __VictoryBabyScene_H__
#define __VictoryBabyScene_H__

#include "../../Dialogue/Dialogue.h"
#include "../Scene.h"

class VictoryBabyScene : public Scene {
public:
    VictoryBabyScene(int width, int height, int& currScene);
    virtual ~VictoryBabyScene();

    static void load();
    static void unload();

    virtual void events();
    virtual void update(float dt);
    virtual void draw() const;

    virtual void init();

    virtual void reset();

private:
    Dialogue* currentDialogue;
    int& currScene;
    bool dialogueFinished;
};

#endif
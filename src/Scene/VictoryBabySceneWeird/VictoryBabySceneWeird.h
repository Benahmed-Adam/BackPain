#ifndef __VictoryBabySceneWeird_H__
#define __VictoryBabySceneWeird_H__

#include "../../Dialogue/Dialogue.h"
#include "../Scene.h"

class VictoryBabySceneWeird : public Scene {
public:
    VictoryBabySceneWeird(int width, int height, int& currScene);
    virtual ~VictoryBabySceneWeird();

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
#ifndef __VictoryPlayerScene_H__
#define __VictoryPlayerScene_H__

#include "../../Dialogue/Dialogue.h"
#include "../Scene.h"

class VictoryPlayerScene : public Scene {
public:
    VictoryPlayerScene(int width, int height, int& currScene);
    virtual ~VictoryPlayerScene();

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
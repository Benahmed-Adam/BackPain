#ifndef __VictoryPlayerSceneWeird_H__
#define __VictoryPlayerSceneWeird_H__

#include "../../Dialogue/Dialogue.h"
#include "../Scene.h"

class VictoryPlayerSceneWeird : public Scene {
public:
    VictoryPlayerSceneWeird(int width, int height, int& currScene);
    virtual ~VictoryPlayerSceneWeird();

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
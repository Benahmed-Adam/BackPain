#ifndef __PIZZAMAN_H__
#define __PIZZAMAN_H__

#include <string>
#include <vector>

#include "../../Dialogue/Dialogue.h"
#include "../Entity.h"

class PizzaMan : public Entity {
public:
    PizzaMan(const Vector3& position, const Vector2& scale,
        const std::vector<std::string>& dialogueLines);
    virtual ~PizzaMan();

    static Texture2D tex;
    static Sound deathSound;
    static Music dialogMusic;

    static void load();
    static void unload();

    virtual void draw(const Camera3D& camera) const;
    virtual void update(float dt);

    bool dialoging;
    std::vector<std::string> dialogueLines;
    Dialogue* dial;
};

#endif
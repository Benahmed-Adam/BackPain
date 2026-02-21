#ifndef __BabyMan_H__
#define __BabyMan_H__

#include <string>
#include <vector>

#include "../../Dialogue/Dialogue.h"
#include "../Entity.h"

class BabyMan : public Entity {
public:
    BabyMan(const Vector3& position, const Vector2& scale,
        const std::vector<std::string>& dialogueLines);
    virtual ~BabyMan();

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
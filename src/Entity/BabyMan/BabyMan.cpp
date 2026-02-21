#include "BabyMan.h"

#include <raylib.h>

#include "../../utils/utils.h"

Texture2D BabyMan::tex;
Sound BabyMan::deathSound;
Music BabyMan::dialogMusic;

void BabyMan::load()
{
    BabyMan::tex = LoadTexture("assets/BabyMan.png");
    BabyMan::deathSound = LoadSound("assets/BabyManDeath.mp3");
    BabyMan::dialogMusic = LoadMusicStream("assets/BabyManDialog.mp3");
}

void BabyMan::unload()
{
    UnloadTexture(BabyMan::tex);
    UnloadSound(BabyMan::deathSound);
    UnloadMusicStream(BabyMan::dialogMusic);
}

BabyMan::BabyMan(const Vector3& position, const Vector2& scale,
    const std::vector<std::string>& dialogueLines)
    : Entity("BabyMan_", position, BabyMan::tex, scale)
    , dialoging(false)
    , dialogueLines(dialogueLines)
{
    this->dial
        = new Dialogue(this->dialogueLines, BabyMan::tex, BabyMan::dialogMusic);
}

BabyMan::~BabyMan()
{
    PlaySound(BabyMan::deathSound);
    delete this->dial;
}

void BabyMan::draw(const Camera3D& camera) const
{
    DrawBillboard(camera, this->texture, this->position, this->scale.x, WHITE);
}

void BabyMan::update(float dt)
{
}
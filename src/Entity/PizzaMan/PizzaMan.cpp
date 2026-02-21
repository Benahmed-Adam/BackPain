#include "PizzaMan.h"

#include <raylib.h>

#include "../../utils/utils.h"

Texture2D PizzaMan::tex;
Sound PizzaMan::deathSound;
Music PizzaMan::dialogMusic;

void PizzaMan::load()
{
    PizzaMan::tex = LoadTexture("assets/PizzaMan.png");
    PizzaMan::deathSound = LoadSound("assets/PizzaManDeath.mp3");
    PizzaMan::dialogMusic = LoadMusicStream("assets/PizzaManDialog.mp3");
}

void PizzaMan::unload()
{
    UnloadTexture(PizzaMan::tex);
    UnloadSound(PizzaMan::deathSound);
    UnloadMusicStream(PizzaMan::dialogMusic);
}

PizzaMan::PizzaMan(const Vector3& position, const Vector2& scale,
    const std::vector<std::string>& dialogueLines)
    : Entity("PizzaMan_", position, PizzaMan::tex, scale)
    , dialoging(false)
    , dialogueLines(dialogueLines)
{
    this->dial = new Dialogue(this->dialogueLines, PizzaMan::tex,
        PizzaMan::dialogMusic);
}

PizzaMan::~PizzaMan()
{
    PlaySound(PizzaMan::deathSound);
    delete this->dial;
}

void PizzaMan::draw(const Camera3D& camera) const
{
    DrawBillboard(camera, this->texture, this->position, this->scale.x, WHITE);
}

void PizzaMan::update(float dt)
{
}
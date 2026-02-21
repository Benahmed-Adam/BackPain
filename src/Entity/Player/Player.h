#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <raylib.h>

#include "../Entity.h"

class Player : public Entity {
public:
    Player(const std::string& name, Vector3 position);
    ~Player();

    static void load();
    static void unload();

    static Texture2D playerPov;
    static Texture2D babyPov;

    virtual void draw(const Camera3D& camera) const;
    virtual void update(float dt);
    void events();

    void drawUI() const;

    const Camera3D& getCamera() const;

    void setFov(float fov);

    void jump(float force);

    Vector3 getViewDirection() const;

    void setRotation(float yaw, float pitch);

    void takeDamage(float amount);

    bool canMove;
    int score;
    float health;
    int role; // 0 c'est un player et 1 c'est un baby
private:
    float maxHealth;
    float stamina;
    float maxStamina;
    float staminaDegenRate;
    float staminaRegenRate;
    float healthRegenRate;
    bool isSprinting;

    Camera3D camera;
    float moveSpeed;
    bool isJumping;
    float jumpForce;
    float gravity;

    float targetFov;
    float fovChangeSpeed;

    float yaw;
    float pitch;

    Vector2 lookVelocity;
    Vector2 lookTarget;
    Vector3 moveVelocity;
};

#endif

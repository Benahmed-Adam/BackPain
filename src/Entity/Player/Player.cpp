#include "Player.h"

#include <raylib.h>
#include <raymath.h>

#include <cmath>

Texture2D Player::playerPov;
Texture2D Player::babyPov;

void Player::load()
{
    Player::playerPov = LoadTexture("assets/PlayerPov.png");
    Player::babyPov = LoadTexture("assets/BabyPov.png");
}

void Player::unload()
{
    UnloadTexture(Player::playerPov);
    UnloadTexture(Player::babyPov);
}

Player::Player(const std::string& name, Vector3 position)
    : Entity(name, position, texture, { 1.0f, 1.0f })
    , score(0)
{
    this->canMove = true;
    this->moveSpeed = 5.0f;
    this->isJumping = false;
    this->jumpForce = 5.0f;
    this->gravity = 9.8f;

    this->health = 100.0f;
    this->maxHealth = 100.0f;
    this->stamina = 100.0f;
    this->maxStamina = 100.0f;
    this->staminaDegenRate = 10.0f;
    this->staminaRegenRate = 15.0f;
    this->healthRegenRate = 5.0f;
    this->isSprinting = false;

    this->targetFov = 60.0f;
    this->fovChangeSpeed = 20.0f;

    this->camera.position
        = { this->position.x, this->position.y, this->position.z };
    this->camera.target
        = { this->position.x, this->position.y, this->position.z };
    this->camera.up = { 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 60.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;

    this->yaw = 0.0f;
    this->pitch = 0.0f;

    this->velocity = { 0.0f, 0.0f, 0.0f };
    this->moveVelocity = { 0.0f, 0.0f, 0.0f };
    this->role = 0;
}

Player::~Player() { }

void Player::draw(const Camera3D& camera) const
{
}

void Player::drawUI() const
{
    float fontSize = GetScreenHeight() / 45.0f;

    DrawText(
        TextFormat("Health: %d/%d", (int)this->health, (int)this->maxHealth),
        10, 10, fontSize, GREEN);
    DrawText(TextFormat("Stamina: %d/%d", (int)this->stamina,
                 (int)this->maxStamina),
        10, 15 + fontSize, fontSize, BLUE);
    DrawText(TextFormat("Flutes found: %d/4", this->score), 10,
        20 + fontSize * 2, fontSize, BLUE);

    Texture2D currentWeaponTexture
        = (this->role == 0) ? Player::playerPov : Player::babyPov;

    float scale = 3.0f;

    float scaledWidth = (float)currentWeaponTexture.width * scale;
    float scaledHeight = (float)currentWeaponTexture.height * scale;

    Vector2 position = { (float)GetScreenWidth() - scaledWidth,
        (float)GetScreenHeight() - scaledHeight };

    DrawTextureEx(currentWeaponTexture, position, 0.0f, scale, WHITE);
}

void Player::update(float dt)
{
    if (this->canMove) {
        if (this->isJumping) {
            this->velocity.y -= this->gravity * dt;
            this->position.y += this->velocity.y * dt;

            if (this->position.y <= 0.0f) {
                this->position.y = 0.0f;
                this->isJumping = false;
                this->velocity.y = 0.0f;
            }
        }
        if (this->position.y > 0.0f)
            this->isJumping = true;

        Vector2 mouseDelta = GetMouseDelta();
        float sensitivity = 0.003f;

        Vector2 lookTarget
            = { -mouseDelta.x * sensitivity, -mouseDelta.y * sensitivity };
        static Vector2 lookVelocity = { 0.0f, 0.0f };

        float lookSmoothness = 15.0f;
        lookVelocity.x
            = Lerp(lookVelocity.x, lookTarget.x, dt * lookSmoothness);
        lookVelocity.y
            = Lerp(lookVelocity.y, lookTarget.y, dt * lookSmoothness);

        this->yaw += lookVelocity.x;
        this->pitch += lookVelocity.y;

        if (this->pitch > 89.0f * DEG2RAD)
            this->pitch = 89.0f * DEG2RAD;
        if (this->pitch < -89.0f * DEG2RAD)
            this->pitch = -89.0f * DEG2RAD;

        Vector3 forwardXZ = { sinf(this->yaw), 0, cosf(this->yaw) };
        Vector3 rightXZ = { cosf(this->yaw), 0, -sinf(this->yaw) };

        Vector3 targetVelocity = { 0.0f, 0.0f, 0.0f };

        if (IsKeyDown(KEY_W))
            targetVelocity = Vector3Add(targetVelocity, forwardXZ);
        if (IsKeyDown(KEY_S))
            targetVelocity = Vector3Subtract(targetVelocity, forwardXZ);
        if (IsKeyDown(KEY_D))
            targetVelocity = Vector3Subtract(targetVelocity, rightXZ);
        if (IsKeyDown(KEY_A))
            targetVelocity = Vector3Add(targetVelocity, rightXZ);

        if (Vector3Length(targetVelocity) > 0.001f)
            targetVelocity = Vector3Normalize(targetVelocity);

        bool wantsToSprint = IsKeyDown(KEY_LEFT_SHIFT);

        if (wantsToSprint && this->stamina > 0) {
            this->isSprinting = true;
        } else {
            this->isSprinting = false;
        }

        if (this->isSprinting) {
            this->stamina -= this->staminaDegenRate * dt;
            if (this->stamina < 0) {
                this->stamina = 0;
                this->isSprinting = false;
            }
        } else if (this->stamina < this->maxStamina) {
            this->stamina += this->staminaRegenRate * dt;
            if (this->stamina > this->maxStamina) {
                this->stamina = this->maxStamina;
            }
        }

        if (this->health < 100) {
            this->health += (this->healthRegenRate * dt) / 2;
        }

        if (this->isSprinting) {
            this->moveSpeed = 25.0f;
            this->targetFov = 80.0f;
        } else {
            this->moveSpeed = 10.0f;
            this->targetFov = 65.0f;
        }

        targetVelocity = Vector3Scale(targetVelocity, this->moveSpeed);

        float moveSmoothness = 20.0f;
        this->moveVelocity.x
            = Lerp(this->moveVelocity.x, targetVelocity.x, dt * moveSmoothness);
        this->moveVelocity.z
            = Lerp(this->moveVelocity.z, targetVelocity.z, dt * moveSmoothness);

        this->position.x += this->moveVelocity.x * dt;
        this->position.z += this->moveVelocity.z * dt;

        float fovSmooth = this->fovChangeSpeed * dt;
        this->camera.fovy = Lerp(this->camera.fovy, this->targetFov, fovSmooth);

        if (IsKeyPressed(KEY_SPACE) && !this->isJumping)
            this->jump(this->jumpForce);
    }

    Vector3 forward
        = { cosf(this->pitch) * sinf(this->yaw), sinf(this->pitch),
              cosf(this->pitch) * cosf(this->yaw) };

    this->camera.position
        = { this->position.x, this->position.y + 1.8f, this->position.z };
    this->camera.target = Vector3Add(this->camera.position, forward);
}

void Player::jump(float force)
{
    this->isJumping = true;
    this->velocity.y = force;
}

const Camera3D&
Player::getCamera() const
{
    return this->camera;
}

Vector3
Player::getViewDirection() const
{
    Vector3 dir = Vector3Subtract(this->camera.target, this->camera.position);
    float len = Vector3Length(dir);
    return (len < 1e-6f) ? Vector3 { 0.0f, 0.0f, 1.0f }
                         : Vector3Scale(dir, 1.0f / len);
}

void Player::setRotation(float yaw, float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
}

void Player::takeDamage(float amount)
{
    this->health -= amount;
    if (this->health < 0) {
        this->health = 0;
    }
}

void Player::setFov(float fov)
{
    this->camera.fovy = fov;
}

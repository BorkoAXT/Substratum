//
// Created by BorkoAXT on 3/8/2026.

#include "Map.h";
#include "Player.h"

#include "raymath.h"

Player::Player() : position(25 * 80 / 2,80), speed(200)
{
}

void Player::Update()
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W)) position.y -= speed * dt;
    if (IsKeyDown(KEY_S)) position.y += speed * dt;
    if (IsKeyDown(KEY_A)) position.x -= speed * dt;
    if (IsKeyDown(KEY_D)) position.x += speed * dt;

    position.x = Clamp(position.x, 5, 200 * 25 - 5);
    position.y = Clamp(position.y, 5, 80 * 25 - 5);
}
void Player::Draw()
{
    DrawRectangle(position.x - 10, position.y - 10, 20, 20, RED);
}

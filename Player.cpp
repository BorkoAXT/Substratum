//
// Created by BorkoAXT on 3/8/2026.

#include "Map.h";
#include "Player.h"

Player::Player()
{
    speed = 200;
    Vector2 position;
}

void Player::Update()
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W)) position.y -= speed * dt;
    if (IsKeyDown(KEY_S)) position.y += speed * dt;
    if (IsKeyDown(KEY_A)) position.x -= speed * dt;
    if (IsKeyDown(KEY_D)) position.x += speed * dt;
}
void Player::Draw()
{
    DrawRectangle(position.x - 10, position.y - 10, 20, 20, RED);
}

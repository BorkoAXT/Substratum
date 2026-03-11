#pragma once
#include "Player.h"
#include "raymath.h"
#include "world/Map.h"
#include "defines/Defines.h"

Player::Player() : position(25 * 80 / 2,80), tilePosition(position.x / 25, position.y / 25), speed(200)
{
}

void Player::Update(Map& map)
{
    float dt = GetFrameTime();
    Vector2 move = { 0, 0 };

    if (IsKeyDown(KEY_W)) move.y -= speed * dt;
    if (IsKeyDown(KEY_S)) move.y += speed * dt;
    if (IsKeyDown(KEY_A)) move.x -= speed * dt;
    if (IsKeyDown(KEY_D)) move.x += speed * dt;

    float nextX = position.x + move.x;

    int tileX_left  = (int)((nextX - 10) / 25);
    int tileX_right = (int)((nextX + 10) / 25);
    int tileY_top    = (int)((position.y - 10) / 25);
    int tileY_bottom = (int)((position.y + 10) / 25);

    if (map.GetBlock(tileX_left, tileY_top).GetType() == AIR &&
        map.GetBlock(tileX_left, tileY_bottom).GetType() == AIR &&
        map.GetBlock(tileX_right, tileY_top).GetType() == AIR &&
        map.GetBlock(tileX_right, tileY_bottom).GetType() == AIR)
    {
        position.x = nextX;
    }

    float nextY = position.y + move.y;
    int tileX_l = (int)((position.x - 10) / 25);
    int tileX_r = (int)((position.x + 10) / 25);
    int tileY_t = (int)((nextY - 10) / 25);
    int tileY_b = (int)((nextY + 10) / 25);

    if (map.GetBlock(tileX_l, tileY_t).GetType() == AIR &&
        map.GetBlock(tileX_r, tileY_t).GetType() == AIR &&
        map.GetBlock(tileX_l, tileY_b).GetType() == AIR &&
        map.GetBlock(tileX_r, tileY_b).GetType() == AIR)
    {
        position.y = nextY;
    }

    tilePosition.x = (int)(position.x / 25);
    tilePosition.y = (int)(position.y / 25);
    position.x = Clamp(position.x, 10, 200 * 25 - 10);
    position.y = Clamp(position.y, 10, 80 * 25 - 10);
}
void Player::Draw()
{
    DrawRectangle(position.x - 10, position.y - 10, 20, 20, RED);
}

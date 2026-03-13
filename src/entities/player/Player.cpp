#include "Player.h"
#include "raymath.h"
#include "defines/Defines.h"
#include "managers/AssetManager.h"

Player::Player(Map& map, float spawnX) : speed(500)
{
    int surfaceY = map.GetSurfaceLevel((int)(spawnX / CELL_SIZE));
    position.x = spawnX;
    position.y = (surfaceY - 3) * CELL_SIZE;
    tilePosition = { position.x / CELL_SIZE, position.y / CELL_SIZE };
    inventory.AddItem(ITEM_STONE);
    inventory.AddItem(ITEM_IRON);
    inventory.AddItem(ITEM_DIRT);
}
void Player::SetCamera(Camera2D* cam)
{
    camera = cam;
}
bool Player::CanMoveTo(Vector2 newPos, Map& map)
{
    int left   = (int)((newPos.x - 10) / CELL_SIZE);
    int right  = (int)((newPos.x + 10) / CELL_SIZE);
    int top    = (int)((newPos.y - 10) / CELL_SIZE);
    int bottom = (int)((newPos.y + 10) / CELL_SIZE);

    if (left < 0 || right >= COLS || top < 0 || bottom >= ROWS) return false;

    return !map.GetBlock(left, top).IsSolid() &&
           !map.GetBlock(left, bottom).IsSolid() &&
           !map.GetBlock(right, top).IsSolid() &&
           !map.GetBlock(right, bottom).IsSolid();
}

void Player::Update(Map& map)
{
    float dt = GetFrameTime();
    Vector2 move = { 0, 0 };

    if (IsKeyDown(KEY_W)) move.y -= speed * dt;
    if (IsKeyDown(KEY_S)) move.y += speed * dt;
    if (IsKeyDown(KEY_A)) move.x -= speed * dt;
    if (IsKeyDown(KEY_D)) move.x += speed * dt;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mousePos, *camera);

        int col = static_cast<int>(worldMouse.x / CELL_SIZE);
        int row = static_cast<int>(worldMouse.y / CELL_SIZE);

        if (col >= 0 && col < COLS && row >= 0 && row < ROWS)
        {
           map.GetBlock(col, row).Hit();
        }
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        ItemID itemId = inventory.GetCurrentItem();
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mousePos, *camera);

        int col = static_cast<int>(worldMouse.x / CELL_SIZE);
        int row = static_cast<int>(worldMouse.y / CELL_SIZE);

        if (col >= 0 && col < COLS && row >= 0 && row < ROWS)
        {
            Block& block = map.GetBlock(col, row);
            if (block.GetType() == AIR && itemId != ITEM_NONE)
            {
                block.SetTexture(AssetManager::GetTexture(itemId));
                block.SetTypeFromItem(itemId);
            }
        }
    }

    Vector2 nextPosX = { position.x + move.x, position.y };
    if (CanMoveTo(nextPosX, map))
        position.x = nextPosX.x;

    Vector2 nextPosY = { position.x, position.y + move.y };
    if (CanMoveTo(nextPosY, map))
        position.y = nextPosY.y;

    tilePosition.x = position.x / CELL_SIZE;
    tilePosition.y = position.y / CELL_SIZE;

    position.x = Clamp(position.x, 10, COLS * CELL_SIZE - 10);
    position.y = Clamp(position.y, 10, ROWS * CELL_SIZE - 10);
    inventory.Update();
}

void Player::Draw()
{
    DrawRectangle(position.x - 10, position.y - 10, 20, 20, RED);
    inventory.Draw(position);
}
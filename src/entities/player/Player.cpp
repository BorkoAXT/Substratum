#include "Player.h"
#include "raymath.h"
#include "defines/Defines.h"
#include "managers/AssetManager.h"

Player::Player(Map& map, float spawnX) : speed(220)
{
    int surfaceY = map.GetSurfaceLevel((int)(spawnX / CELL_SIZE));
    position.x = spawnX;
    position.y = (surfaceY - 3) * CELL_SIZE;
    velocity = {0,0};
    swingAngle = -140.0f;
    swingSpeed = 0.0f;
    tilePosition = { position.x / CELL_SIZE, position.y / CELL_SIZE };
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

    float accel = 1800.0f;
    float friction = 10.0f;
    float gravity = 1800.0f;
    float jumpForce = 520.0f;

    map.UpdateItems();

    if (IsKeyDown(KEY_A)) velocity.x -= accel * dt;
    if (IsKeyDown(KEY_D)) velocity.x += accel * dt;

    velocity.x -= velocity.x * friction * dt;
    velocity.y += gravity * dt;

    Vector2 below = {position.x, position.y + 11};
    bool grounded = !CanMoveTo(below, map);

    if (grounded && IsKeyPressed(KEY_SPACE))
    {
        velocity.y = -jumpForce;
    }

    if (inventory.GetCurrentItem().id == ITEM_PICKAXE)
    {
        const float minAngle = -140.0f;
        const float maxAngle = 140.0f;
        const float accelSwing = 420.0f;
        const float maxSpeed = 900.0f;

        if (IsKeyDown(KEY_T))
        {
            swingSpeed += accelSwing * dt;

            if (swingSpeed > maxSpeed)
                swingSpeed = maxSpeed;

            swingAngle += swingSpeed * dt;

            if (swingAngle > maxAngle)
                swingAngle = minAngle;

            pickaxeRotation = swingAngle;
        }

        if (IsKeyReleased(KEY_T))
        {
            Vector2 mousePos = GetMousePosition();
            Vector2 worldMouse = GetScreenToWorld2D(mousePos, *camera);

            Vector2 dir = Vector2Normalize(Vector2Subtract(worldMouse, position));

            float throwForce = 2000.0f + swingSpeed * 3.0f;
            velocity.x += dir.x * throwForce;
            velocity.y += dir.y * throwForce;

            swingSpeed = 0.0f;
            swingAngle = minAngle;
            pickaxeRotation = 0.0f;
        }
    }

    Vector2 move = { velocity.x * dt, velocity.y * dt };

    Vector2 nextPosX = { position.x + move.x, position.y };

    if (CanMoveTo(nextPosX, map))
        position.x = nextPosX.x;
    else
        velocity.x = 0;

    Vector2 nextPosY = { position.x, position.y + move.y };

    if (CanMoveTo(nextPosY, map))
        position.y = nextPosY.y;
    else
    {
        if (velocity.y > 0) velocity.y = 0;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isMining)
    {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mousePos, *camera);

        int col = (int)(worldMouse.x / CELL_SIZE);
        int row = (int)(worldMouse.y / CELL_SIZE);

        if (col >= 0 && col < COLS && row >= 0 && row < ROWS)
        {
            Vector2 blockCenter = {
                (float)col * CELL_SIZE + CELL_SIZE / 2,
                (float)row * CELL_SIZE + CELL_SIZE / 2
            };

            float dist = Vector2Distance(position, blockCenter);

            if (dist < 80)
            {
                isMining = true;
                miningTimer = 0.0f;
                miningTarget = blockCenter;
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        InventorySlot slot = inventory.GetCurrentItem();
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMouse = GetScreenToWorld2D(mousePos, *camera);

        int col = (int)(worldMouse.x / CELL_SIZE);
        int row = (int)(worldMouse.y / CELL_SIZE);

        if (col >= 0 && col < COLS && row >= 0 && row < ROWS)
        {
            Block& block = map.GetBlock(col, row);

            if (!block.IsSolid() && slot.id != ITEM_NONE && slot.id != ITEM_PICKAXE)
            {
                Texture2D tex = AssetManager::GetTexture(slot.id);
                block.SetTypeFromItem(slot.id, tex);
                inventory.RemoveCurrentItem();
            }
        }
    }

    std::vector<ItemID> pickedUp = map.CollectItems(this->position, 60.0f);

    for (ItemID id : pickedUp)
        inventory.AddItem(id);

    tilePosition.x = position.x / CELL_SIZE;
    tilePosition.y = position.y / CELL_SIZE;

    position.x = Clamp(position.x, 10, COLS * CELL_SIZE - 10);
    position.y = Clamp(position.y, 10, ROWS * CELL_SIZE - 10);

    inventory.Update();

    if (isMining)
    {
        miningTimer += dt;

        pickaxeRotation = sin(miningTimer * 12.0f) * 70.0f;

        if (miningTimer > 0.25f)
        {
            int col = (int)(miningTarget.x / CELL_SIZE);
            int row = (int)(miningTarget.y / CELL_SIZE);

            Block& block = map.GetBlock(col, row);

            if (block.IsTree())
            {
                int r = row;

                while (r >= 0)
                {
                    Block& treePart = map.GetBlock(col, r);

                    if (!treePart.IsTree())
                        break;

                    ItemID droppedItem = ITEM_WOOD;

                    if (droppedItem != ITEM_NONE)
                    {
                        Vector2 spawnPos = {
                            (float)col * CELL_SIZE + CELL_SIZE / 2,
                            (float)r * CELL_SIZE + CELL_SIZE / 2
                        };

                        map.SpawnItem(droppedItem, spawnPos);
                    }

                    treePart.ClearAll();

                    r--;
                }
            }
            else
            {
                ItemID droppedItem = block.Hit();

                if (droppedItem != ITEM_NONE)
                {
                    Vector2 spawnPos =
                    {
                        (float)col * CELL_SIZE + CELL_SIZE / 2,
                        (float)row * CELL_SIZE + CELL_SIZE / 2
                    };

                    map.SpawnItem(droppedItem, spawnPos);
                }
            }

            isMining = false;
            pickaxeRotation = 0.0f;
        }
    }
}

void Player::Draw()
{
    DrawRectangle(position.x - 10, position.y - 10, 20, 20, RED);

    Texture2D tex = AssetManager::GetTexture("pickaxe");

    Rectangle src = {0,0,(float)tex.width,(float)tex.height};

    Rectangle dest =
    {
        position.x + 6,
        position.y - 4,
        25,
        25
    };

    Vector2 origin = {5,20};

    if (inventory.GetCurrentItem().id == ITEM_PICKAXE)
    {
        DrawTexturePro(tex, src, dest, origin, pickaxeRotation, WHITE);
    }

    inventory.Draw(position);
}
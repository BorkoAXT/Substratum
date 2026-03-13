#include "Inventory.h"
#include "raymath.h"
#include "../managers/AssetManager.h"

Inventory::Inventory()
    : currentIndex(0), prevIndex(0), targetIndex(0), direction(0), isVisible(false), transitionProgress(1.0f)
{
    items.resize(3, { ITEM_NONE, 0 });
}

bool Inventory::AddItem(ItemID itemID)
{
    if (itemID == ITEM_NONE) return false;

    for (int i = 0; i < (int)items.size(); i++)
    {
        if (items[i].id == itemID)
        {
            items[i].count++;
            return true;
        }
    }

    for (int i = 0; i < (int)items.size(); i++)
    {
        if (items[i].id == ITEM_NONE)
        {
            items[i].id = itemID;
            items[i].count = 1;
            return true;
        }
    }
    return false;
}

InventorySlot Inventory::GetCurrentItem()
{
    if (items.empty()) return { ITEM_NONE, 0 };
    return items[currentIndex];
}
void Inventory::RemoveCurrentItem()
{
    if (items.empty()) return;

    if (items[currentIndex].id != ITEM_NONE)
    {
        items[currentIndex].count--;

        if (items[currentIndex].count <= 0)
        {
            items[currentIndex].id = ITEM_NONE;
            items[currentIndex].count = 0;
        }
    }
}

void Inventory::Update()
{
    if (IsKeyPressed(KEY_TAB)) isVisible = !isVisible;
    if (!isVisible) return;

    if (transitionProgress >= 1.0f)
    {
        if (IsKeyPressed(KEY_Q))
        {
            direction = -1;
            prevIndex = currentIndex;
            targetIndex = (currentIndex - 1 + (int)items.size()) % (int)items.size();
            transitionProgress = 0.0f;
        }
        else if (IsKeyPressed(KEY_E))
        {
            direction = 1;
            prevIndex = currentIndex;
            targetIndex = (currentIndex + 1) % (int)items.size();
            transitionProgress = 0.0f;
        }
    }

    if (transitionProgress < 1.0f)
    {
        transitionProgress += GetFrameTime() * 6.0f;
        if (transitionProgress >= 1.0f)
        {
            transitionProgress = 1.0f;
            currentIndex = targetIndex;
            direction = 0;
        }
    }
}

void Inventory::Draw(Vector2 playerPos)
{
    if (!isVisible) return;

    const float p = transitionProgress;
    const float xCenter = playerPos.x;
    const float yCenter = playerPos.y - 80.0f;

    const float bigSize = 45.0f;
    const float smallSize = 25.0f;
    const float gap = 20.0f;
    const float step = smallSize + gap;

    for (int i = -1; i <= 1; i++)
    {
        int slotIdx = (currentIndex + i + (int)items.size()) % (int)items.size();

        float posX = i * step - direction * step * p;
        float distFromCenter = fabsf(posX);
        float size = (distFromCenter < step) ? Lerp(smallSize, bigSize, 1.0f - distFromCenter / step) : smallSize;
        float opacity = (distFromCenter > step * 0.5f) ? Lerp(1.0f, 0.0f, (distFromCenter - step * 0.5f) / step) : 1.0f;

        Rectangle slot = { xCenter + posX - size / 2, yCenter - size / 2, size, size };
        DrawRectangleRec(slot, Fade(BLACK, 0.6f * opacity));

        InventorySlot currentSlot = items[slotIdx];
        if (currentSlot.id != ITEM_NONE)
        {
            Texture2D tex = AssetManager::GetTexture(currentSlot.id);
            if (tex.id != 0)
            {
                float scale = (size * 0.8f) / (float)tex.width;
                Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
                Rectangle dest = { slot.x + size * 0.1f, slot.y + size * 0.1f, tex.width * scale, tex.height * scale };
                DrawTexturePro(tex, src, dest, {0,0}, 0.0f, Fade(WHITE, opacity));

                if (currentSlot.count > 1)
                {
                    DrawText(TextFormat("%d", currentSlot.count), (int)slot.x + 2, (int)slot.y + (int)size - 12, 10, Fade(RAYWHITE, opacity));
                }
            }
        }
    }
}
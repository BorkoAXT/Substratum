#include "Inventory.h"
#include "raymath.h"
#include "../managers/AssetManager.h"

Inventory::Inventory()
    : currentIndex(0), prevIndex(0),
      targetIndex(0), direction(0), isVisible(false), transitionProgress(1.0f)
{
    items.resize(SLOT_COUNT, ITEM_NONE);
}

bool Inventory::AddItem(ItemID itemID)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i] == ITEM_NONE)
        {
            items[i] = itemID;
            return true;
        }
    }
    return false;
}
ItemID Inventory::GetCurrentItem()
{
    return items[currentIndex];
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
            targetIndex = (currentIndex - 1 + items.size()) % items.size();
            transitionProgress = 0.0f;
        }
        else if (IsKeyPressed(KEY_E))
        {
            direction = 1;
            prevIndex = currentIndex;
            targetIndex = (currentIndex + 1) % items.size();
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

void Inventory::Draw(Vector2 playerPos) {
    if (!isVisible) return;

    const float p = transitionProgress;
    const float xCenter = playerPos.x;
    const float yCenter = playerPos.y - 80.0f;

    const float bigSize = 45.0f;
    const float smallSize = 25.0f;
    const float gap = 20.0f;
    const float step = smallSize + gap;

    int itemCount = 0;
    for (ItemID id : items) if (id != ITEM_NONE) itemCount++;

    int slotsToDraw = (itemCount == 0) ? 3 : (itemCount == 1) ? 1 : (itemCount == 2) ? 2 : 3;
    int start = -slotsToDraw / 2;
    int end = start + slotsToDraw - 1;

    for (int i = -1; i <= 1; i++)
    {
        int slotIdx = (currentIndex + i + items.size()) % items.size();

        float posX = i * step - direction * step * p;
        float distFromCenter = fabsf(posX);
        float size = (distFromCenter < step) ? Lerp(smallSize, bigSize, 1.0f - distFromCenter / step) : smallSize;
        float opacity = (distFromCenter > step * 0.5f) ? Lerp(1.0f, 0.0f, (distFromCenter - step * 0.5f) / step) : 1.0f;

        Rectangle slot = { xCenter + posX - size / 2, yCenter - size / 2, size, size };
        DrawRectangleRec(slot, Fade(BLACK, 0.6f * opacity));

        ItemID itemId = items[slotIdx];
        if (itemId != ITEM_NONE)
        {
            Texture2D tex = AssetManager::GetTexture(itemId);
            if (tex.id != 0)
            {
                float scale = size / (float)tex.width;
                Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
                Rectangle dest = { slot.x, slot.y, tex.width * scale, tex.height * scale };
                DrawTexturePro(tex, src, dest, {0,0}, 0.0f, Fade(WHITE, opacity));
            }
        }
    }
}


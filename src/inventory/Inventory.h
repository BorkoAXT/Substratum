#pragma once
#include <vector>
#include "raylib.h"
#include "defines/Defines.h"

struct InventorySlot {
    ItemID id = ITEM_NONE;
    int count = 0;
};

class Inventory {
public:
    Inventory();

    bool AddItem(ItemID itemID);
    void Update();
    void Draw(Vector2 playerPos);

    InventorySlot GetCurrentItem();
    void RemoveCurrentItem();
    bool IsVisible() const { return isVisible; }

private:
    std::vector<InventorySlot> items;

    int currentIndex;
    int prevIndex;
    int targetIndex;
    int direction;

    bool isVisible;
    float transitionProgress;

    const int slot_count = 3;
};
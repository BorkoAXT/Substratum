#pragma once
#include <raylib.h>
#include <vector>

#include "defines/Defines.h"


class Inventory
{
public:
    Inventory();

    void Update();
    void Draw(Vector2 pos);
    ItemID GetCurrentItem();
    bool AddItem(ItemID itemID);

private:
    std::vector<ItemID> items;

    int currentIndex;
    bool isVisible;

    int prevIndex;
    int targetIndex;
    int direction;
    float transitionProgress;
};

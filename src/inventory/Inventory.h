#pragma once
#include <raylib.h>
#include <vector>


class Inventory
{
public:
    Inventory();

    void Update();
    void Draw(Vector2 pos);

    void Next();
    void Previous();

private:
    std::vector<int> items;

    int currentIndex;
    bool isVisible;

    int prevIndex;
    int targetIndex;
    int direction;
    float transitionProgress;
};

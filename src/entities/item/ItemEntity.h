#include "raylib.h"
#include "defines/Defines.h"

struct ItemEntity
{
    ItemID id;
    Vector2 position;
    Vector2 velocity;
    bool active;
    float age;
};

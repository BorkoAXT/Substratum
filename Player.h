#include <raylib.h>
#include "Map.h"
class Player
{
public:
    Vector2 position;
    Vector2 tilePosition;
    float speed;
    Player();
    void Update(Map& map);
    void Draw();
};
#include <raylib.h>
class Player
{
public:
    Vector2 position;
    float speed;
    Player();
    void Update();
    void Draw();
};
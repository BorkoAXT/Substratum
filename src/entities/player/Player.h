#include <entities/Entity.h>
#include "world/Map.h"
class Player : public Entity
{
public:
    Player(Map& map, float spawnX);
    void Update(Map& map) override;
    void Draw() override;

    float speed;
    Vector2 tilePosition;

private:
    bool CanMoveTo(Vector2 newPos, Map& map);
};
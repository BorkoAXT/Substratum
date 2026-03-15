#include <entities/Entity.h>
#include "inventory/Inventory.h"
#include "world/Map.h"
class Player : public Entity
{
public:
    Player(Map& map, float spawnX);
    void SetCamera(Camera2D* cam);
    void Update(Map& map) override;
    void Draw() override;

    float speed;
    Vector2 tilePosition;
    Inventory inventory;

private:
    bool CanMoveTo(Vector2 newPos, Map& map);
    Camera2D* camera = nullptr;
    bool isMining = false;
    float miningTimer = 0.0f;
    float pickaxeRotation = 0.0f;
    Vector2 velocity = {0,0};
    bool isSwinging = false;
    float swingAngle = 0;
    float swingVelocity = 0;
    Vector2 pivotPoint = {0,0};
    float swingCharge = 0.0f;
    float swingSpeed = 0.0f;
    Vector2 miningTarget = {0,0};
};
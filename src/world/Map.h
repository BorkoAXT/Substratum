#pragma once
#include <vector>
#include "blocks/Block.h"
#include "../thirdparty/raylib-5.5/src/raylib.h"
#include "entities/item/ItemEntity.h"
class NoiseGen;
class Map
{
public:
    Map(NoiseGen& noiseGen);

    void Draw(Vector2 playerPos);
    int GetSurfaceLevel(int col);
    Block& GetBlock(int row, int col);
    void SpawnItem(ItemID, Vector2 pos);
    void UpdateItems();
    std::vector<ItemID> CollectItems(Vector2 collectorPos, float radius);
    void DrawItems();
    ~Map();

private:
    int rows;
    int cols;
    int cellSize;
    std::vector<Texture2D> textures;
    std::vector<std::vector<Block>> blocks;
    std::vector<ItemEntity> droppedItems;

};
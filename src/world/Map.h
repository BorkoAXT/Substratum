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

    int GetSurfaceLevel(int col);
    Block& GetBlock(int row, int col);
    int GetTreeHeight(int col, int row);
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    int GetCellSize() const { return cellSize; }
    void RemoveTree(int col, int row, int height);
    void ResolveItemInsideBlock(ItemEntity& item);
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
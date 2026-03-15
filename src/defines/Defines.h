
#ifndef DEFINES_H
#define DEFINES_H

#define CYAN {135, 206, 235}
// #define ROWS 1800
// #define COLS 6400

#define ROWS 80
#define COLS 400
#define CELL_SIZE 25

#define SLOT_COUNT 3

enum TileType {
    AIR,
    DIRT,
    GRASS,
    IRON,
    STONE,
    TREE_TRUNK,
    TREE_PART,
    TREE_LEAVES,
    TREE_CAP,
    BACKGROUND,
    RUBY,
    SAPPHIRE,
    COAL,
    YELLOW_FLOWER,
    GRASS_LEAVES_1,
    GRASS_LEAVES_2,
    WOOD,
};

enum ItemID
{
    ITEM_NONE = -1,
    ITEM_DIRT = 0,
    ITEM_STONE = 1,
    ITEM_IRON = 2,
    ITEM_PICKAXE = 3,
    ITEM_WOOD = 4,
};
#endif
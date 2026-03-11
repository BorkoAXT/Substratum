
#ifndef DEFINES_H
#define DEFINES_H

#define CYAN {135, 206, 235}
// #define ROWS 1800
// #define COLS 6400

#define ROWS 80
#define COLS 400
#define CELL_SIZE 25

enum TileType {
    AIR = 0,
    DIRT = 1,
    GRASS = 2,
    IRON = 3,
    TREE_TRUNK = 4,
    TREE_PART = 5,
    TREE_LEAVES = 6,
    TREE_CAP = 7,
};

#endif
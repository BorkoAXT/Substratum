
#ifndef DEFINES_H
#define DEFINES_H

#define CYAN {135, 206, 235}
// #define ROWS 1800
// #define COLS 6400

#define ROWS 80
#define COLS 400
#define CELL_SIZE 25

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
    BACKGROUND
};

#endif
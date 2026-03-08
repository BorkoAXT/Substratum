#include "grid.h"
#include <raylib.h>

Grid::Grid() {
    rows = 20;
    columns = 30;
    pixels = GetScreenWidth() / columns;
}
void Grid::DrawGrid()
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            DrawRectangleLines(
                col * pixels,
                row * pixels,
                pixels,
                pixels,
                DARKGRAY
            );
        }
    }
}



#pragma once
#include <raylib.h>
class Block
{
public:
    Block();
    void SetPos(Vector2 pos);
    Vector2 GetPos();
    Vector2 GetSize();
    void Draw();
    void SetTexture(Texture2D tex);

private:
    int x, y;
    int size;
    Texture2D texture;
};




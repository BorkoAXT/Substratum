#include "Inventory.h"
#include "raymath.h"

Inventory::Inventory()
    : items({10, 20, 30, 40, 50}), currentIndex(0), prevIndex(0),
      targetIndex(0), direction(0), isVisible(false), transitionProgress(1.0f) {}

void Inventory::Update() {
    if (IsKeyPressed(KEY_TAB)) isVisible = !isVisible;
    if (!isVisible) return;

    if (transitionProgress >= 1.0f) {
        if (IsKeyPressed(KEY_LEFT)) {
            direction = -1;
            prevIndex = currentIndex;
            targetIndex = (currentIndex - 1 + items.size()) % items.size();
            transitionProgress = 0.0f;
        } else if (IsKeyPressed(KEY_RIGHT)) {
            direction = 1;
            prevIndex = currentIndex;
            targetIndex = (currentIndex + 1) % items.size();
            transitionProgress = 0.0f;
        }
    }

    if (transitionProgress < 1.0f) {
        transitionProgress += GetFrameTime() * 6.0f;
        if (transitionProgress >= 1.0f) {
            transitionProgress = 1.0f;
            currentIndex = targetIndex;
            direction = 0;
        }
    }
}

void Inventory::Draw(Vector2 playerPos) {
    if (!isVisible) return;

    float p = transitionProgress;
    float xCenter = playerPos.x;
    float yCenter = playerPos.y - 80;

    float big = 45.0f;
    float small = 25.0f;
    float gap = 20.0f;
    float step = small + gap;

    int start = (direction == 1) ? -1 : -2;
    int end = (direction == 1) ? 2 : 1;

    if (direction == 0) { start = -1; end = 1; }

    for (int i = start; i <= end; i++) {
        int itemIdx = (prevIndex + i + items.size()) % items.size();

        float posX = (i * step) - (direction * step * p);

        if (posX < -step * 1.5f || posX > step * 1.5f) continue;

        float distFromCenter = fabsf(posX);
        float size = small;

        if (distFromCenter < step) {
            float sizeLerp = 1.0f - (distFromCenter / step);
            size = Lerp(small, big, sizeLerp);
        }

        float opacity = 1.0f;
        if (distFromCenter > step * 0.5f) {
            opacity = Lerp(1.0f, 0.0f, (distFromCenter - step * 0.5f) / step);
        }

        Rectangle slot = { xCenter + posX - size/2, yCenter - size/2, size, size };

        DrawRectangleRec(slot, Fade(BLACK, 0.6f * opacity));

        int fontSize = (size > small + 10) ? 15 : 10;
        const char* txt = TextFormat("%d", items[itemIdx]);
        int txtWidth = MeasureText(txt, fontSize);

        DrawText(txt, slot.x + (size/2) - (txtWidth/2), slot.y + (size/2) - (fontSize/2), fontSize, Fade(WHITE, opacity));
    }
}
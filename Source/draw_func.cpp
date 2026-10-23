#include "../Headers/draw_func.h"

int main(void)
{
    InitWindow(1660, 1110, "raylib example - basic window");
    while (!WindowShouldClose())
    {

        if(isUpdate)
        {
            drawPicture(picture);
        }
    }
    CloseWindow();
}

void drawPicture(Color **picture)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int pixelsPerX = size_VRAM_x;
    int pixelsPerY = size_VRAM_y;
    int pixelSizeX = screenWidth / pixelsPerX;
    int pixelSizeY = screenHeight / pixelsPerY;
    BeginDrawing();
    for (int y = 0; y < pixelsPerY; y++) {
        for (int x = 0; x < pixelsPerX; x++) {
            int screenX = x * pixelSizeX;
            int screenY = y * pixelSizeY;
            Color pixelColor = picture[y][x];
            DrawRectangle(screenX, screenY, 5, 5, pixelColor);
        }
    }
    EndDrawing();
}
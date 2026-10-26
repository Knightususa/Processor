#include "../Headers/draw_func.h"

void drawPicture(Color *picture)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int minscreen = (screenWidth < screenHeight) ? screenWidth : screenHeight;
    int pixelsPerX = size_VRAM_x;
    int pixelsPerY = size_VRAM_y;
    int pixelSizeX = minscreen / pixelsPerX;
    int pixelSizeY = minscreen / pixelsPerY;
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int y = 0; y < pixelsPerY; y++) 
    {
        for (int x = 0; x < pixelsPerX; x++) 
        {
            int screenX = x * pixelSizeX;
            int screenY = y * pixelSizeY;
            Color pixelColor = picture[y * pixelsPerX + x];
            DrawRectangle(screenX, screenY, pixelSizeX, pixelSizeY, pixelColor);
        }
    }
    EndDrawing();
}
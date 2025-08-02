#include "raylib.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "{#gameName}");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
      BeginDrawing();
      ClearBackground(RED);
      DrawText("Hola Mundo\n{#gameName}", screenWidth/2, screenHeight/2, 40, DARKGRAY);
      EndDrawing();
    }

    return 0;
    CloseWindow();
}

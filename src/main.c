#include "raylib.h"

typedef struct Player
{
  Vector2 position;
} Player;


int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Lizard Meme");
    SetTargetFPS(60);

    Texture sprite = LoadTexture("res/lizardSprite.png");

    Player player = {
      .position = { 50, 50}
    };

    while (!WindowShouldClose())
    {
      BeginDrawing();
      ClearBackground(RED);
      DrawText("Hola Mundo\nLizard Meme", screenWidth/2, screenHeight/2, 40, DARKGRAY);
      DrawTextureEx(sprite, player.position, 0.0f, 0.25f, RAYWHITE);
      
      EndDrawing();
    }

    return 0;
    CloseWindow();
}

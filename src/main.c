#include "raylib.h"
#include <stdlib.h>
#include <time.h>

int randomNum();

typedef struct Player
{
  Vector2 position;
} Player;

typedef struct Iguana
{
  Vector2 position;

} Iguana;

int main(void)
{
    srand(time(NULL));
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Lizard Meme");
    SetTargetFPS(60);

    InitAudioDevice();

    Texture sprite = LoadTexture("res/lizardSprite.png");
    Texture iguanaSprite = LoadTexture("res/iguanaSprite.png");

    Sound lizardSound = LoadSound("res/lizard.wav");
    Sound* lizardPtr = &lizardSound;

    Sound sound2 = LoadSound("res/lizardUpShift.wav");
    Sound sound3 = LoadSound("res/lizardDownShift.wav");
    Sound* ptr2 = &sound2;
    Sound* ptr3 = &sound3;

    Sound* sounds[3];
    sounds[0] = lizardPtr;
    sounds[1] = ptr2;
    sounds[2] = ptr3;

    Player player = {
      .position = { 50, 50}
    };

    Iguana iguana = {
      .position = { 400, 400}
    };

    PlaySound(*lizardPtr);
    while (!WindowShouldClose())
    {
      float deltaTime = GetFrameTime();
      float speed = 300.0f; // pixels per second

      if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      {
        player.position.y -= speed * deltaTime;
      }
      if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      {
        player.position.y += speed * deltaTime;
      }
      if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      {
        player.position.x -= speed * deltaTime;
      }
      if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      {
        player.position.x += speed * deltaTime;
      }

      BeginDrawing();
      ClearBackground(RED);
      DrawText("Hola Mundo\nLizard Meme", screenWidth/2, screenHeight/2, 40, DARKGRAY);
      DrawRectangle(player.position.x+50, player.position.y+10, 160, 200, BLUE);
      DrawRectangle(iguana.position.x, iguana.position.y, 200, 90, GREEN);
      DrawTextureEx(sprite, player.position, 0.0f, 0.28f, RAYWHITE);
      DrawTextureEx(iguanaSprite, (Vector2) {400, 400}, 0.0f, 0.87f, RAYWHITE);
      
      EndDrawing();
    }

    return 0;
    CloseWindow();
}

int randomNumber()
{
  int rng = rand();
  return ((rng % 3)+1);
}
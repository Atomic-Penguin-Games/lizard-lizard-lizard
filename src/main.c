#include "raylib.h"
#include <stdlib.h>
#include <time.h>

int randomNum();

typedef struct Player
{
  Vector2 position;
} Player;


int main(void)
{
    srand(time(NULL));
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Lizard Meme");
    SetTargetFPS(60);

    InitAudioDevice();

    Texture sprite = LoadTexture("res/lizardSprite.png");

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

    PlaySound(*lizardPtr);
    while (!WindowShouldClose())
    {

      if (IsKeyPressed(KEY_W))
      {
        PlaySound(*sounds[0]);
      }
      if (IsKeyPressed(KEY_S))
      {
        PlaySound(*sounds[1]);
      }
      if (IsKeyPressed(KEY_D))
      {
        PlaySound(*sounds[2]);
      }

      BeginDrawing();
      ClearBackground(RED);
      DrawText("Hola Mundo\nLizard Meme", screenWidth/2, screenHeight/2, 40, DARKGRAY);
      DrawTextureEx(sprite, player.position, 0.0f, 0.25f, RAYWHITE);
      
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
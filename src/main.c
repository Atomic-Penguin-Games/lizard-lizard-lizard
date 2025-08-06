#include "raylib.h"
#include "definitions.h"
#include "randomizer.h"
#include "sound.h"
#include "entityManager.h"
#include <stdio.h>

int score = 0;

typedef struct Player
{
  Vector2 position;
  Rectangle hitbox;
  Texture sprite;
  Vector2 hitboxOffset;
} Player;

void drawPlayer(Player player);
void updatePlayer(Player *player);

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Lizard Meme");
    SetTargetFPS(60);

    initSoundSystem();
    initRandomizer();

    EntityManager manager = {
      .entities = {},
      .count = 0,
      .spawnTimer = OBJECT_SPAWN_RATE_MIN
    };
    Texture sprite = LoadTexture("res/lizardSprite.png");
    Texture iguanaSprite = LoadTexture("res/lizardEmoji.png");
    Texture sawSprite = LoadTexture("res/circular_saw_blade.png");

    Player player = {
      .position = { 50, 50},
      .hitbox = {100, 60, 160, 200},
      .sprite = sprite,
      .hitboxOffset = {50, 10}
    };
    //DrawRectangle(player.position.x+50, player.position.y+10, 160, 200, BLUE);
    float spawnTimer = 0.0f;
    float nextSpawnTime = randomSpawnTime(); // Set initial random spawn time

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      
      if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      {
        player.position.y -= SPEED * dT;
        if (player.position.y < 0)
        {
          player.position.y = 0;
        }
      }
      if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      {
        player.position.y += SPEED * dT;
        if (player.position.y + player.hitbox.height > screenHeight)
        {
          player.position.y = screenHeight - player.hitbox.height;
        }
      }
      if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      {
        player.position.x -= SPEED * dT;
        if (player.position.x < 0)
        {
          player.position.x = 0;
        }
      }
      if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      {
        player.position.x += SPEED * dT;
        if (player.position.x + player.hitbox.width > screenWidth)
        {
          player.position.x = screenWidth - player.hitbox.width;
        }
      }
      updatePlayer(&player);
      updateEntities(&manager, dT, screenWidth);
      
      // Update spawn timer and spawn enemies
      spawnTimer += dT;
      
      if (spawnTimer >= nextSpawnTime)
      {
        printf("Spawning\n");
        // Randomly choose between spawning an iguana or sawblade (50/50 chance)
        if (randomNum(2) == 1)
        {
          spawnEntity(&manager, LIZARD, iguanaSprite, screenWidth, screenHeight);
        }
        else
        {
          spawnEntity(&manager, SAW, sawSprite, screenWidth, screenHeight);
        }
        spawnTimer = 0.0f;
        nextSpawnTime = randomSpawnTime(); // Set new random spawn time
      }

      BeginDrawing();
      ClearBackground(RED);
      //DrawText("Hola Mundo\nLizard Meme", screenWidth/2, screenHeight/2, 40, DARKGRAY);
      drawEntities(&manager);
      drawPlayer(player);
      DrawText(TextFormat("%d", score), screenWidth/2, 30, 60, RAYWHITE);

      EndDrawing();
    }

    return 0;
    CloseWindow();
}

void drawPlayer(Player player)
{
  DrawTextureEx(player.sprite, player.position, 0.0f, 0.28f, RAYWHITE);
  if (DEBUG_MODE)
  {
    DrawRectangleLinesEx(player.hitbox, 2, BLUE);
    //DrawRectangleRec(player.hitbox, GREEN); // Assuming you want to visualize the hitbox in green
  }
}

void updatePlayer(Player *player)
{
  player->hitbox.x = player->position.x + player->hitboxOffset.x;
  player->hitbox.y = player->position.y + player->hitboxOffset.y;
}
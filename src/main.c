#include <stdio.h>
#include <raylib.h>
#include "definitions.h"
#include "randomizer.h"
#include "sound.h"
#include "entityManager.h"
#include "player.h"
#include "inputManager.h"
#include "graphicsManager.h"

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    int score = 0;

    InitWindow(screenWidth, screenHeight, "Lizard Meme");
    SetTargetFPS(60);

    initSoundSystem();
    initRandomizer();

    EntityManager manager = initManager();
    GraphicsManager gm = initGraphicsManager();

    Player player = createPlayer(&gm.playerSpritesheet);
    initHitboxPointers(&player);  // Initialize hitbox pointers after creation

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      
      Vector2 velocity = getPlayerInput();
      updatePlayer(&player, velocity, dT, screenWidth, screenHeight);
      updateManager(&manager, &gm, dT, screenWidth, screenHeight);
      updateEntities(&manager, dT, screenWidth);
      CollisionType collisionType = checkForCollisions(&manager, player.hitboxes);
      printf("CollisionType: %d\n", collisionType);
      
      switch(collisionType)
      {
        case SCORE_COLLISION:
          score++;
          playScoreSound();
          playAnimation(&player);
          break;
        case DEATH_COLLISION:
          //TODO Death
          break;
        default:
          break;
      };
      BeginDrawing();
      ClearBackground(RED);
      drawEntities(&manager);
      drawPlayer(&player);
      DrawText(TextFormat("%d", score), screenWidth/2, 30, 60, RAYWHITE);
      if (IsKeyPressed(KEY_SPACE))
      {
        playScoreSound();
        playAnimation(&player);
      }
      EndDrawing();
    }

    return 0;
    CloseWindow();
}
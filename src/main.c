#include <raylib.h>
#include "definitions.h"
#include "randomizer.h"
#include "sound.h"
#include "entityManager.h"
#include "player.h"
#include "inputManager.h"

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
    Texture sprite = LoadTexture("res/animationSpritesheet.png");
    Texture iguanaSprite = LoadTexture("res/lizardEmoji.png");
    Texture sawSprite = LoadTexture("res/circular_saw_blade.png");

    Player player = createPlayer(&sprite);

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      
      Vector2 velocity = getPlayerInput();
      updatePlayer(&player, velocity, dT, screenWidth, screenHeight);
      updateEntities(&manager, dT, screenWidth);
    
      BeginDrawing();
      ClearBackground(RED);
      drawEntities(&manager);
      drawPlayer(&player);
      DrawText(TextFormat("%d", score), screenWidth/2, 30, 60, RAYWHITE);

      EndDrawing();
    }

    return 0;
    CloseWindow();
}
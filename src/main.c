#include <stdio.h>
#include <math.h>
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
    bool pauseMode = false;
    // Virtual resolution (fixed game coordinates)
    const int virtualScreenWidth = SCREEN_WIDTH;
    const int virtualScreenHeight = SCREEN_HEIGHT;
    
    // Actual window size (can be different/resizable)
    const int windowWidth = SCREEN_WIDTH;
    const int windowHeight = SCREEN_HEIGHT;
    
    int score = 0;

    // Enable fullscreen support for web builds
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(windowWidth, windowHeight, "Lizard Meme");
    SetTargetFPS(60);

    // Create render texture for virtual resolution
    RenderTexture2D target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    initSoundSystem();
    initRandomizer();

    EntityManager manager = initManager();
    GraphicsManager gm = initGraphicsManager();

    Player player = createPlayer(&gm.playerSpritesheet);
    initHitboxPointers(&player);  // Initialize hitbox pointers after creation

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      
      // Get current window size for scaling calculations
      int currentWindowWidth = GetScreenWidth();
      int currentWindowHeight = GetScreenHeight();
      
      // Calculate scale to fit virtual resolution to window
      float scale = fminf((float)currentWindowWidth/virtualScreenWidth, (float)currentWindowHeight/virtualScreenHeight);
      
      if (IsKeyPressed(KEY_SPACE)) pauseMode = !pauseMode;

      if (pauseMode == false)
      {
        Vector2 velocity = getPlayerInput();
        updatePlayer(&player, velocity, dT, virtualScreenWidth, virtualScreenHeight);
        updateManager(&manager, &gm, dT, virtualScreenWidth, virtualScreenHeight);
        updateEntities(&manager, dT, virtualScreenWidth);
        CollisionType collisionType = checkForCollisions(&manager, player.hitboxes);
      
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
      }
      
      // Draw everything to the virtual resolution render texture
      BeginTextureMode(target);
        ClearBackground(RED);
        drawEntities(&manager);
        drawPlayer(&player);
        DrawText(TextFormat("%d", score), virtualScreenWidth/2, 30, 60, RAYWHITE);
        if (IsKeyPressed(KEY_SPACE))
        {
          playScoreSound();
          playAnimation(&player);
        }
      EndTextureMode();
      
      // Draw the scaled render texture to the actual window
      BeginDrawing();
        ClearBackground(BLACK);
        
        // Calculate position to center the scaled virtual screen
        int offsetX = (currentWindowWidth - (int)(virtualScreenWidth * scale)) / 2;
        int offsetY = (currentWindowHeight - (int)(virtualScreenHeight * scale)) / 2;
        
        // Draw the virtual screen scaled to fit the window
        Rectangle source = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
        Rectangle dest = { offsetX, offsetY, virtualScreenWidth * scale, virtualScreenHeight * scale };
        
        DrawTexturePro(target.texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
      EndDrawing();
    }

    // Cleanup
    UnloadRenderTexture(target);
    return 0;
    CloseWindow();
}
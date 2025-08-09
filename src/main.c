#include <math.h>
#include <stdio.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "definitions.h"
#include "randomizer.h"
#include "sound.h"
#include "entityManager.h"
#include "player.h"
#include "inputManager.h"
#include "graphicsManager.h"
#include "screens.h"

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

    // Screen management
    ScreenID currentScreen = SCREEN_MAIN_MENU;
    ScreenID nextScreen = SCREEN_MAIN_MENU;

    // Enable fullscreen support for web builds
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(windowWidth, windowHeight, "Lizard Meme");
    SetTargetFPS(60);

    // Create render texture for virtual resolution
    RenderTexture2D target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    //GuiLoadStyle("res/style_jungle.rgs");
    GuiLoadStyle("res/style_cyber.rgs");
    //GuiLoadStyle("res/style_dark.rgs");
    //GuiLoadStyle("res/style_ashes.rgs");

    initSoundSystem();
    initRandomizer();

    EntityManager manager = initManager();
    GraphicsManager gm = initGraphicsManager();

    Player player = createPlayer(&gm.playerSpritesheet);
    initHitboxPointers(&player);  // Initialize hitbox pointers after creation

    // Initialize starting screen
    mainMenuScreenInit();

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      
      // Get current window size for scaling calculations
      int currentWindowWidth = GetScreenWidth();
      int currentWindowHeight = GetScreenHeight();
      
      // Calculate scale to fit virtual resolution to window
      float scale = fminf((float)currentWindowWidth/virtualScreenWidth, (float)currentWindowHeight/virtualScreenHeight);
      
      // Handle screen updates
      switch (currentScreen) {
        case SCREEN_MAIN_MENU:
          nextScreen = mainMenuScreenUpdate(dT);
          break;
        case SCREEN_GAME:
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
          
          // ESC to return to menu
          if (IsKeyPressed(KEY_ESCAPE)) {
            nextScreen = SCREEN_MAIN_MENU;
          }
          break;
        default:
          break;
      }
      
      // Handle quit request
      if (nextScreen == SCREEN_QUIT) {
        printf("Application quit requested\n");
        break;  // Exit the main loop
      }
      
      // Handle screen transitions
      if (nextScreen != currentScreen && nextScreen != SCREEN_QUIT) {
        // Unload current screen
        switch (currentScreen) {
          case SCREEN_MAIN_MENU:
            mainMenuScreenUnload();
            break;
          case SCREEN_GAME:
            // gameScreenUnload(); // Add when you create game screen
            break;
          default:
            break;
        }
        
        // Initialize new screen
        switch (nextScreen) {
          case SCREEN_MAIN_MENU:
            mainMenuScreenInit();
            break;
          case SCREEN_GAME:
            // gameScreenInit(); // Add when you create game screen
            break;
          default:
            break;
        }
        
        currentScreen = nextScreen;
      }
            // Draw everything to the virtual resolution render texture
      BeginTextureMode(target);
        ClearBackground(RED);
        
        // Draw current screen content
        switch (currentScreen) {
          case SCREEN_GAME:
            drawEntities(&manager);
            drawPlayer(&player);
            DrawText(TextFormat("%d", score), virtualScreenWidth/2, 30, 60, RAYWHITE);
            if (pauseMode) {
              DrawText("PAUSED", virtualScreenWidth/2 - 100, virtualScreenHeight/2, 40, YELLOW);
            }
            break;
          case SCREEN_MAIN_MENU:
          default:
            // Main menu content drawn outside render texture for GUI interactivity
            break;
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
        
        // Draw UI elements directly to screen for interactivity
        if (currentScreen == SCREEN_MAIN_MENU) {
          mainMenuScreenDraw(currentWindowWidth, currentWindowHeight);
        }
      EndDrawing();
    }

    // Cleanup current screen before exit
    switch (currentScreen) {
      case SCREEN_MAIN_MENU:
        mainMenuScreenUnload();
        break;
      case SCREEN_GAME:
        // gameScreenUnload(); // Add when you create game screen
        break;
      default:
        break;
    }

    // Cleanup
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
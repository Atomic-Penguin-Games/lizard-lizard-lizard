#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <math.h>
#include <stdio.h>
#include "definitions.h"
#include "randomizer.h"
#include "soundManager.h"
#include "entityManager.h"
#include "player.h"
#include "inputManager.h"
#include "graphicsManager.h"
#include "screens.h"

void unloadGame();

int main(void)
{
    
    bool pauseMode = false;
    bool isRunning = true;
    // Virtual resolution (fixed game coordinates)
    const int virtualScreenWidth = SCREEN_WIDTH;
    const int virtualScreenHeight = SCREEN_HEIGHT;
    
    // Actual window size (can be different/resizable)
    const int windowWidth = SCREEN_WIDTH;
    const int windowHeight = SCREEN_HEIGHT;

    // Screen management
    ScreenID currentScreen = SCREEN_MAIN_MENU;
    ScreenID nextScreen = SCREEN_MAIN_MENU;

    // Enable fullscreen support for web builds
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(windowWidth, windowHeight, "Lizard Meme");
    SetTargetFPS(60);

    //GuiLoadStyle("res/style_jungle.rgs");
    GuiLoadStyle("res/style_cyber.rgs");
    //GuiLoadStyle("res/style_dark.rgs");
    //GuiLoadStyle("res/style_ashes.rgs");

    initRandomizer();

    // Initialize starting screen
    mainMenuScreenInit();

    GraphicsManager graphicsManager = initGraphicsManager();
    SoundManager soundManager = initSoundManager();

    // Game screen will be initialized when needed
    GameScreen gameScreen;
    bool gameScreenInitialized = false;
    


    while (!WindowShouldClose() && isRunning)
    {
      float dT = GetFrameTime(); //deltaTime
      
      // Get current window size for scaling calculations
      int currentWindowWidth = GetScreenWidth();
      int currentWindowHeight = GetScreenHeight();
      
      // Update current screen
      switch (currentScreen)
      {
          case SCREEN_MAIN_MENU:
              nextScreen = mainMenuScreenUpdate(dT);
              break;
          case SCREEN_GAME:
              if (!gameScreenInitialized) {
                  gameScreen = gameScreenInit(&graphicsManager, &soundManager);
                  gameScreenInitialized = true;
              }
              nextScreen = gameScreenUpdate(&gameScreen, dT);
              break;
          default:
              break;
      }
      
      // Draw current screen
      BeginDrawing();
          ClearBackground(DARKBLUE);
          
          switch (currentScreen)
          {
              case SCREEN_MAIN_MENU:
                  mainMenuScreenDraw(currentWindowWidth, currentWindowHeight);
                  break;
              case SCREEN_GAME:
                  gameScreenDraw(&gameScreen, currentWindowWidth, currentWindowHeight);
                  break;
              default:
                  break;
          }
      EndDrawing();
      
      // Handle screen transitions
      if (nextScreen != currentScreen) {
          printf("Screen transition: %d -> %d\n", currentScreen, nextScreen);
          
          switch (nextScreen) {
              case SCREEN_QUIT:
                  printf("Application quit requested\n");
                  isRunning = false;
                  unloadGame();
                  break;
                  
              case SCREEN_MAIN_MENU:
                  // Reset game screen when returning to main menu
                  if (currentScreen == SCREEN_GAME && gameScreenInitialized) {
                      gameScreenInitialized = false;
                      printf("Returning to main menu - game screen reset\n");
                  }
                  currentScreen = nextScreen;
                  break;
                  
              case SCREEN_GAME:
                  printf("Transitioning to game screen\n");
                  currentScreen = nextScreen;
                  break;
                  
              default:
                  currentScreen = nextScreen;
                  break;
          }
      }
    }

    CloseWindow();
    return 0;
}

void unloadGame()
{
    
}
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
    initRandomizer();
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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); //enable fullscreen
    
    InitWindow(windowWidth, windowHeight, "Lizard Meme");
    SetTargetFPS(60);

    GuiLoadStyle("res/style_cyber.rgs");

    GraphicsManager graphicsManager = initGraphicsManager();
    SoundManager soundManager = initSoundManager();
    CursorManager cursorManager = initCursorManager(&graphicsManager);

    mainMenuScreenInit(&graphicsManager, &soundManager, &cursorManager,
        GetScreenWidth(), GetScreenHeight());

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
                  gameScreen = gameScreenInit(&graphicsManager, &soundManager, &cursorManager);
                  gameScreenInitialized = true;
              }
              nextScreen = gameScreenUpdate(&gameScreen, dT);
              break;
          default:
              break;
      }
      
      // Draw current screen
      BeginDrawing();
          ClearBackground(BACKGROUND_BUFFER_COLOR);
          
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
          
          switch (nextScreen) {
              case SCREEN_QUIT:
                  isRunning = false;
                  unloadGame();
                  break;
                  
              case SCREEN_MAIN_MENU:
                  // Reset game screen when returning to main menu
                  if (currentScreen == SCREEN_GAME && gameScreenInitialized) {
                      gameScreenInitialized = false;
                  }
                  // Re-initialize main menu to ensure cursor is properly set up
                  mainMenuScreenInit(&graphicsManager, &soundManager, &cursorManager,
                    currentWindowWidth, currentWindowHeight);
                  currentScreen = nextScreen;
                  break;
                  
              case SCREEN_GAME:
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
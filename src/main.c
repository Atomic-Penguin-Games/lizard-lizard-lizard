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
#include "raygui_style_cyber.h"

void unloadGame();

typedef struct GameContext {
    GraphicsManager* gm;
    SoundManager* sm;
    CursorManager* cm;
} GameContext;

void gameLoop(GameContext gameContext);
void splashScreen();

int main(void)
{
    initRandomizer();
    // Virtual resolution (fixed game coordinates)
    const int virtualScreenWidth = SCREEN_WIDTH;
    const int virtualScreenHeight = SCREEN_HEIGHT;
    
    // Actual window size (can be different/resizable)
    const int windowWidth = SCREEN_WIDTH;
    const int windowHeight = SCREEN_HEIGHT;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); //enable fullscreen
    
    InitWindow(windowWidth, windowHeight, "Lizard Meme");
    SetTargetFPS(60);

    GuiLoadStyleFromMemory(raygui_style_cyber, raygui_style_cyber_length);
    //GuiLoadStyle("res/style_cyber.rgs");

    GraphicsManager graphicsManager = initGraphicsManager();
    SoundManager soundManager = initSoundManager();
    CursorManager cursorManager = initCursorManager(&graphicsManager);

    GameContext gameContext = {
        .gm = &graphicsManager,
        .sm = &soundManager,
        .cm = &cursorManager
    };

    splashScreen();

    gameLoop(gameContext);

    CloseWindow();
    return 0;
}

void splashScreen()
{
    splashScreenInit();

    while(splashScreenUpdate(GetFrameTime()))
    {
        splashScreenDraw(GetScreenWidth(), GetScreenHeight());
    }

    splashScreenUnload();
}

void gameLoop(GameContext gameContext)
{
    mainMenuScreenInit(gameContext.gm, gameContext.sm, gameContext.cm,
        GetScreenWidth(), GetScreenHeight());

    // Game screen will be initialized when needed
    GameScreen gameScreen;
    bool gameScreenInitialized = false;

    // Screen management
    ScreenID currentScreen = SCREEN_MAIN_MENU;
    ScreenID nextScreen = SCREEN_MAIN_MENU;

    while (!WindowShouldClose())
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
                  gameScreen = gameScreenInit(gameContext.gm, gameContext.sm, gameContext.cm);
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
                  return;
                  
              case SCREEN_MAIN_MENU:
                  // Reset game screen when returning to main menu
                  if (currentScreen == SCREEN_GAME && gameScreenInitialized) {
                      gameScreenInitialized = false;
                  }
                  // Re-initialize main menu to ensure cursor is properly set up
                  mainMenuScreenInit(gameContext.gm, gameContext.sm, gameContext.cm,
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
}

void unloadGame()
{
    
}
#include <math.h>
#include <stdio.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "definitions.h"
#include "randomizer.h"
#include "soundManager.h"
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
    // ScreenID currentScreen = SCREEN_MAIN_MENU;
    // ScreenID nextScreen = SCREEN_MAIN_MENU;

    // Enable fullscreen support for web builds
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(windowWidth, windowHeight, "Lizard Meme");
    SetTargetFPS(60);

    //GuiLoadStyle("res/style_jungle.rgs");
    GuiLoadStyle("res/style_cyber.rgs");
    //GuiLoadStyle("res/style_dark.rgs");
    //GuiLoadStyle("res/style_ashes.rgs");

    initRandomizer();

    // Initialize starting screen
    //mainMenuScreenInit();

    GraphicsManager graphicsManager = initGraphicsManager();
    SoundManager soundManager = initSoundManager();

    GameScreen gameScreen = gameScreenInit(&graphicsManager, &soundManager);

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      
      // Get current window size for scaling calculations
      int currentWindowWidth = GetScreenWidth();
      int currentWindowHeight = GetScreenHeight();
      
      gameScreenUpdate(&gameScreen, dT);
      gameScreenDraw(&gameScreen, currentWindowWidth, currentWindowHeight);
      
      // if (nextScreen == SCREEN_QUIT) {
      //   printf("Application quit requested\n");
      //   break;  // Exit the main loop
      // }
    }

    CloseWindow();
    return 0;
}
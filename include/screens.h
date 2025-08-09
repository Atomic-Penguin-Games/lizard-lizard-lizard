#pragma once
#include <raylib.h>

typedef enum {
    SCREEN_SPLASH = 0,
    SCREEN_MAIN_MENU = 1,
    SCREEN_GAME = 2,
    SCREEN_GAME_OVER = 3,
    SCREEN_QUIT = 4  // Special state to signal application quit
} ScreenID;

void splashScreenInit();
ScreenID splashScreenUpdate(float dt);
void splashScreenDraw();
void splashScreenUnload();

void mainMenuScreenInit();
ScreenID mainMenuScreenUpdate(float dt);
void mainMenuScreenDraw(int screenWidth, int screenHeight);
void mainMenuScreenUnload();

void gameScreenInit();
ScreenID gameScreenUpdate(float dt);
void gameScreenDraw();
void gameScreenUnload();
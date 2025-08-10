#pragma once
#include <raylib.h>
#include "entityManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "player.h"

typedef enum {
    SCREEN_SPLASH = 0,
    SCREEN_MAIN_MENU = 1,
    SCREEN_GAME = 2,
    SCREEN_GAME_OVER = 3,
    SCREEN_QUIT = 4  // Special state to signal application quit
} ScreenID;

typedef struct {
    EntityManager entityManager;
    GraphicsManager *graphicsManager;
    SoundManager *soundManager;
    Player player;
    RenderTexture2D target;
    int score;
} GameScreen;

void splashScreenInit();
ScreenID splashScreenUpdate(float dt);
void splashScreenDraw();
void splashScreenUnload();

void mainMenuScreenInit();
ScreenID mainMenuScreenUpdate(float dt);
void mainMenuScreenDraw(int screenWidth, int screenHeight);
void mainMenuScreenUnload();

GameScreen gameScreenInit(GraphicsManager *gm, SoundManager *sm);
ScreenID gameScreenUpdate(GameScreen *gameScreen, float dt);
void gameScreenDraw(GameScreen *gameScreen, int screenWidth, int screenHeight);
void gameScreenUnload();
#include "screens.h"

static Texture2D splashLogo;
static float splashTimer;

void splashScreenInit()
{
    splashTimer = SPLASH_SCREEN_TIMER;
    splashLogo = LoadTexture("res/splash_screen_logo_placeholder.png");
}

//returns false when the timer falls below 0, to have main.c start the game.
bool splashScreenUpdate(float dt)
{
    splashTimer -= dt;
    if (splashTimer <= 0) {
        return false;
    } else {
        return true;
    }
}

void splashScreenDraw(int screenWidth, int screenHeight)
{
    BeginDrawing();
        ClearBackground(BACKGROUND_BUFFER_COLOR);
        DrawTexture(splashLogo, screenWidth/2-(splashLogo.width/2), 
            screenHeight/2 - (splashLogo.height/2), RAYWHITE);
    EndDrawing();
}

void splashScreenUnload()
{
    UnloadTexture(splashLogo);
}
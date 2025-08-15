#include "screens.h"
#include "definitions.h"
#include <raygui.h>
#include <stdio.h>

// Static variables to communicate between Draw and Update
static bool playButtonPressed = false;
static bool exitButtonPressed = false;

void mainMenuScreenInit()
{

}

ScreenID mainMenuScreenUpdate(float dt)
{   
    // Check if buttons were pressed in the draw function
    if (playButtonPressed) {
        playButtonPressed = false;  // Reset the flag
        printf("Transitioning to game screen\n");
        return SCREEN_GAME;
    }
    
    if (exitButtonPressed) {
        exitButtonPressed = false;  // Reset the flag
        printf("Quitting application\n");
        return SCREEN_QUIT;
    }
    
    return SCREEN_MAIN_MENU;
}

void mainMenuScreenDraw(int screenWidth, int screenHeight)
{
    // Draw title
    const char* title = GAME_TITLE;
    int titleWidth = MeasureText(title, 60);
    DrawText(title, (screenWidth / 2) - (titleWidth / 2), screenHeight / 4, 60, RAYWHITE);
    
    // Draw Play button
    Rectangle playButton = {
        (screenWidth / 2.0f) - (MAIN_MENU_BUTTON_WIDTH / 2.0f),
        (screenHeight / 2.25f) - (MAIN_MENU_BUTTON_HEIGHT / 2.0f),
        MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT
    };
    Rectangle exitButton = {
        (screenWidth / 2.0f) - (MAIN_MENU_BUTTON_WIDTH / 2.0f),
        (screenHeight / 2.25f) - (MAIN_MENU_BUTTON_HEIGHT / 2.0f) + 
        MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_PADDING,
        MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT
    };

    GuiSetStyle(DEFAULT, TEXT_SIZE, MAIN_MENU_BUTTON_FONT_SIZE);
    
    // Try raygui buttons first
    if (GuiButton(playButton, "#131#Play")) {
        playButtonPressed = true;
        printf("Play button clicked in Draw\n");
    }
    if (GuiButton(exitButton, "Quit")) {
        exitButtonPressed = true;
        printf("Exit button clicked in Draw\n");
    }
    
    // Fallback: Manual mouse detection for web builds
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePos, playButton)) {
            playButtonPressed = true;
            printf("Play button clicked via mouse fallback\n");
        }
        if (CheckCollisionPointRec(mousePos, exitButton)) {
            exitButtonPressed = true;
            printf("Exit button clicked via mouse fallback\n");
        }
    }
    
    // Visual feedback for hover (since raygui might not work properly)
    if (CheckCollisionPointRec(mousePos, playButton)) {
        DrawRectangleLines(playButton.x - 2, playButton.y - 2, 
                          playButton.width + 4, playButton.height + 4, YELLOW);
    }
    if (CheckCollisionPointRec(mousePos, exitButton)) {
        DrawRectangleLines(exitButton.x - 2, exitButton.y - 2, 
                          exitButton.width + 4, exitButton.height + 4, YELLOW);
    }
}

void mainMenuScreenUnload()
{

}
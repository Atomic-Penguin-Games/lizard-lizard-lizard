#include "screens.h"
#include "definitions.h"
#include "web_cursor.h"
#include <raygui.h>
#include <stdio.h>

// Static variables to communicate between Draw and Update
static bool playButtonPressed = false;
static bool exitButtonPressed = false;

void mainMenuScreenInit()
{
    WebCursorInit();
}

ScreenID mainMenuScreenUpdate(float dt)
{   
    // Check if buttons were pressed in the draw function
    if (playButtonPressed || IsKeyPressed(KEY_SPACE)) {
        playButtonPressed = false;  // Reset the flag
        return SCREEN_GAME;
    }
    
    if (exitButtonPressed) {
        exitButtonPressed = false;  // Reset the flag
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
    if (GuiButton(playButton, "#131#Play")) {
        playButtonPressed = true;
    }
    if (GuiButton(exitButton, "Quit")) {
        exitButtonPressed = true;
    }
    
    // Get mouse position and handle clicks using the web cursor module
    Vector2 mousePos = WebCursorGetPosition();
    
    // Manual button click detection for web compatibility
    if (WebCursorIsPressed()) {
        if (CheckCollisionPointRec(mousePos, playButton)) {
            playButtonPressed = true;
        }
        if (CheckCollisionPointRec(mousePos, exitButton)) {
            exitButtonPressed = true;
        }
    }
    
    // Visual feedback for hover
    if (CheckCollisionPointRec(mousePos, playButton)) {
        DrawRectangleLines(playButton.x - 2, playButton.y - 2, 
                          playButton.width + 4, playButton.height + 4, YELLOW);
    }
    if (CheckCollisionPointRec(mousePos, exitButton)) {
        DrawRectangleLines(exitButton.x - 2, exitButton.y - 2, 
                          exitButton.width + 4, exitButton.height + 4, YELLOW);
    }
    
    // Draw custom cursor
    WebCursorDraw();
}

void mainMenuScreenUnload()
{
    WebCursorCleanup();
}
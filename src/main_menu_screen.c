#include <raygui.h>
#include <stdio.h>
#include "screens.h"
#include "definitions.h"
#include "cursorManager.h"

// Main Menu State Structure
typedef struct {
    Player player;
    bool playButtonPressed;
    bool exitButtonPressed;
} MainMenuState;

// Static state instance
static MainMenuState menuState = {0};

void mainMenuScreenInit(GraphicsManager *gm, SoundManager *sm, int screenWidth, int screenHeight)
{
    menuState.playButtonPressed = false;
    menuState.exitButtonPressed = false;
    Player player = createPlayer(&gm->playerSpritesheet, MAIN_MENU_CHARACTER_SCALE);
    player.position.x = screenWidth / 7;
    player.position.y = screenHeight / 2 + 20;
    menuState.player = player;
    playAnimation(&menuState.player);
    playDefaultScoreSound(sm);
    CursorManagerInit();
}

ScreenID mainMenuScreenUpdate(float dt)
{   
    // Check if buttons were pressed in the draw function
    if (menuState.playButtonPressed || IsKeyPressed(KEY_SPACE)) {
        menuState.playButtonPressed = false;  // Reset the flag
        return SCREEN_GAME;
    }
    
    if (menuState.exitButtonPressed) {
        menuState.exitButtonPressed = false;  // Reset the flag
        return SCREEN_QUIT;
    }
    
    updatePlayer(&menuState.player, (Vector2){0,0}, dt);

    return SCREEN_MAIN_MENU;
}

void mainMenuScreenDraw(int screenWidth, int screenHeight)
{
    // Ensure system cursor is always hidden on main menu
    HideCursor();
    
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
        menuState.playButtonPressed = true;
    }
    if (GuiButton(exitButton, "Quit")) {
        menuState.exitButtonPressed = true;
    }
    
    // Get mouse position and handle clicks using the cursor manager module
    Vector2 mousePos = CursorManagerGetPosition();
    
    // Manual button click detection for web compatibility
    if (CursorManagerIsPressed()) {
        if (CheckCollisionPointRec(mousePos, playButton)) {
            menuState.playButtonPressed = true;
        }
        if (CheckCollisionPointRec(mousePos, exitButton)) {
            menuState.exitButtonPressed = true;
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
    
    CursorManagerDraw();
    menuState.player.position.x = screenWidth / 12;
    drawPlayer(&menuState.player);
}

void mainMenuScreenUnload()
{
    CursorManagerCleanup();
}
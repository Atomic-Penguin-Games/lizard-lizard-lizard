#include "screens.h"
#include "definitions.h"
#include <raygui.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

// Store mouse position and click state for web build
static Vector2 webMousePos = {400, 300}; // Default center position
static bool webMousePressed = false;
static bool webMouseJustPressed = false;

// Emscripten mouse callback
EM_BOOL mouse_move_callback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    // Calculate mouse position relative to canvas
    webMousePos.x = mouseEvent->targetX;
    webMousePos.y = mouseEvent->targetY;
    
    return EM_TRUE;
}

// Emscripten mouse click callbacks
EM_BOOL mouse_down_callback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    webMousePos.x = mouseEvent->targetX;
    webMousePos.y = mouseEvent->targetY;
    webMousePressed = true;
    webMouseJustPressed = true;
    return EM_TRUE;
}

EM_BOOL mouse_up_callback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    webMousePos.x = mouseEvent->targetX;
    webMousePos.y = mouseEvent->targetY;
    webMousePressed = false;
    return EM_TRUE;
}

// Initialize mouse event handling for web
void initWebMouse() {
    emscripten_set_mousemove_callback("#canvas", NULL, EM_TRUE, mouse_move_callback);
    emscripten_set_mousedown_callback("#canvas", NULL, EM_TRUE, mouse_down_callback);
    emscripten_set_mouseup_callback("#canvas", NULL, EM_TRUE, mouse_up_callback);
}
#endif

// Static variables to communicate between Draw and Update
static bool playButtonPressed = false;
static bool exitButtonPressed = false;

void mainMenuScreenInit()
{
#ifdef __EMSCRIPTEN__
    initWebMouse();
#endif
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
    // Hide system cursor for both desktop and web builds
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
        playButtonPressed = true;
    }
    if (GuiButton(exitButton, "Quit")) {
        exitButtonPressed = true;
    }
    
    // Get mouse position from raylib (this will be our visual cursor position)
    Vector2 mousePos = GetMousePosition();
    
    #ifdef __EMSCRIPTEN__
        // For web builds, use our custom mouse tracking since raylib's doesn't work
        mousePos = webMousePos;
        
        // Manual button click detection for web since GuiButton uses broken raylib mouse
        if (webMouseJustPressed) {
            if (CheckCollisionPointRec(mousePos, playButton)) {
                playButtonPressed = true;
            }
            if (CheckCollisionPointRec(mousePos, exitButton)) {
                exitButtonPressed = true;
            }
            webMouseJustPressed = false; // Reset the flag
        }
    #endif
    
    // Visual feedback for hover
    if (CheckCollisionPointRec(mousePos, playButton)) {
        DrawRectangleLines(playButton.x - 2, playButton.y - 2, 
                          playButton.width + 4, playButton.height + 4, YELLOW);
    }
    if (CheckCollisionPointRec(mousePos, exitButton)) {
        DrawRectangleLines(exitButton.x - 2, exitButton.y - 2, 
                          exitButton.width + 4, exitButton.height + 4, YELLOW);
    }
    
    // Draw custom cursor at the correct mouse position
    DrawCircle((int)mousePos.x, (int)mousePos.y, 12, WHITE);
    DrawCircle((int)mousePos.x, (int)mousePos.y, 10, BLACK);
    DrawCircle((int)mousePos.x, (int)mousePos.y, 8, YELLOW);
    DrawCircle((int)mousePos.x, (int)mousePos.y, 5, RED);
}

void mainMenuScreenUnload()
{
    // Restore system cursor when leaving main menu
    ShowCursor();
}
#include "cursorManager.h"
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

#endif

void CursorManagerInit(void)
{
    // Hide the system cursor for both desktop and web builds
    HideCursor();
    
#ifdef __EMSCRIPTEN__
    // Initialize mouse event handling for web
    emscripten_set_mousemove_callback("#canvas", NULL, EM_TRUE, mouse_move_callback);
    emscripten_set_mousedown_callback("#canvas", NULL, EM_TRUE, mouse_down_callback);
    emscripten_set_mouseup_callback("#canvas", NULL, EM_TRUE, mouse_up_callback);
#endif
}

Vector2 CursorManagerGetPosition(void)
{
#ifdef __EMSCRIPTEN__
    // For web builds, use our custom mouse tracking since raylib's doesn't work
    return webMousePos;
#else
    // For desktop builds, use raylib's mouse functions
    return GetMousePosition();
#endif
}

bool CursorManagerIsPressed(void)
{
#ifdef __EMSCRIPTEN__
    // For web builds, use our custom click tracking
    if (webMouseJustPressed) {
        webMouseJustPressed = false; // Reset the flag
        return true;
    }
    return false;
#else
    // For desktop builds, use raylib's mouse functions
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
#endif
}

void CursorManagerDraw(void)
{
    Vector2 mousePos = CursorManagerGetPosition();
    
    // Draw custom cursor at the correct mouse position
    DrawCircle((int)mousePos.x, (int)mousePos.y, 12, WHITE);
    DrawCircle((int)mousePos.x, (int)mousePos.y, 10, BLACK);
    DrawCircle((int)mousePos.x, (int)mousePos.y, 8, YELLOW);
    DrawCircle((int)mousePos.x, (int)mousePos.y, 5, RED);
}

void CursorManagerCleanup(void)
{
    // Restore system cursor
    ShowCursor();
}

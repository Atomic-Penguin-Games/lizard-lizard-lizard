#pragma once
#include <raylib.h>

// Initialize cursor manager system (call once at startup or screen init)
void CursorManagerInit(void);

// Get the current mouse position (works correctly on both desktop and web)
Vector2 CursorManagerGetPosition(void);

// Check if mouse button was just pressed this frame (works correctly on both desktop and web)
bool CursorManagerIsPressed(void);

// Draw the custom cursor at the current mouse position
void CursorManagerDraw(void);

// Cleanup cursor manager system (call when exiting screen)
void CursorManagerCleanup(void);
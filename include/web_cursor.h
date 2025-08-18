#ifndef WEB_CURSOR_H
#define WEB_CURSOR_H

#include <raylib.h>

// Initialize web cursor system (call once at startup or screen init)
void WebCursorInit(void);

// Get the current mouse position (works correctly on both desktop and web)
Vector2 WebCursorGetPosition(void);

// Check if mouse button was just pressed this frame (works correctly on both desktop and web)
bool WebCursorIsPressed(void);

// Draw the custom cursor at the current mouse position
void WebCursorDraw(void);

// Cleanup web cursor system (call when exiting screen)
void WebCursorCleanup(void);

#endif // WEB_CURSOR_H

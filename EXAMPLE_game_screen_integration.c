// Example implementation for game_screen.c
// Add this to your includes at the top:
#include "web_cursor.h"

// Add these variables to track cursor state in GAME_STATE_GAME_OVER mode:
static bool gameCursorInitialized = false;

// In your gameScreenUpdate function, when entering GAME_STATE_GAME_OVER:
if (gameScreen->state == GAME_STATE_PLAYING) {
    // ... existing playing state code ...
    
    if (collisionType == DEATH_COLLISION) {
        gameScreen->state = GAME_STATE_GAME_OVER;
        
        // Initialize custom cursor for game over dialog
        WebCursorInit();
        gameCursorInitialized = true;
        
        playDeathSound(gameScreen->soundManager);
    }
}
else if (gameScreen->state == GAME_STATE_GAME_OVER) {
    // Game over state - handle restart/menu with both keyboard and mouse
    
    if (IsKeyPressed(KEY_SPACE)) {
        // Cleanup cursor and restart game
        if (gameCursorInitialized) {
            WebCursorCleanup();
            gameCursorInitialized = false;
        }
        gameScreen->state = GAME_STATE_PLAYING;
        // ... rest of restart logic ...
    }
    
    // Example: Add clickable buttons in the game over dialog
    Vector2 mousePos = WebCursorGetPosition();
    
    // Define button areas (you'd adjust these to match your dialog layout)
    Rectangle restartButton = {
        VIRTUAL_SCREEN_WIDTH/2 - 100, 
        VIRTUAL_SCREEN_HEIGHT/2 + 40, 
        200, 30
    };
    Rectangle menuButton = {
        VIRTUAL_SCREEN_WIDTH/2 - 100, 
        VIRTUAL_SCREEN_HEIGHT/2 + 80, 
        200, 30
    };
    
    // Handle mouse clicks
    if (WebCursorIsPressed()) {
        if (CheckCollisionPointRec(mousePos, restartButton)) {
            // Restart game
            if (gameCursorInitialized) {
                WebCursorCleanup();
                gameCursorInitialized = false;
            }
            gameScreen->state = GAME_STATE_PLAYING;
            // ... restart logic ...
        }
        else if (CheckCollisionPointRec(mousePos, menuButton)) {
            // Return to main menu
            if (gameCursorInitialized) {
                WebCursorCleanup();
                gameCursorInitialized = false;
            }
            return SCREEN_MAIN_MENU;
        }
    }
    
    // Visual feedback for button hover
    if (CheckCollisionPointRec(mousePos, restartButton)) {
        // Draw hover effect for restart button
    }
    if (CheckCollisionPointRec(mousePos, menuButton)) {
        // Draw hover effect for menu button
    }
}

// In your drawGameOverDialog function, add at the end:
static void drawGameOverDialog(int score)
{
    // ... existing dialog drawing code ...
    
    // Draw custom cursor if initialized
    if (gameCursorInitialized) {
        WebCursorDraw();
    }
}

// Don't forget to cleanup when leaving the game screen entirely:
void gameScreenUnload(GameScreen *gameScreen)
{
    if (gameCursorInitialized) {
        WebCursorCleanup();
        gameCursorInitialized = false;
    }
    // ... existing cleanup code ...
}

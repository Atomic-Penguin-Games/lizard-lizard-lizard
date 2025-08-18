#include "screens.h"
#include "entityManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "player.h"
#include "definitions.h"
#include "raygui.h"
#include "cursorManager.h"
#include "math.h"

// Static function prototypes
static int drawGameOverDialogText(const char* text, int fontSize, 
    int dialogX, int dialogY, int dialogWidth, Color color);
static void drawGameOverDialog(int score, int screenWidth, int screenHeight);
static void drawGameOverOverlay(int score, int screenWidth, int screenHeight);

static bool replayButtonPressed = false;
static bool quitButtonPressed = false;
static bool gameCursorInitialized = false;

GameScreen gameScreenInit(GraphicsManager *gm, SoundManager *sm)
{
    EntityManager entityManager = initEntityManager();
    Player player = createPlayer(&gm->playerSpritesheet);
    RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    GameScreen gameScreen = {
        .entityManager = entityManager,
        .player = player,
        .graphicsManager = gm,
        .soundManager = sm,
        .target = target,
        .score = 0,
        .state = GAME_STATE_PLAYING
    };
    return gameScreen;
}

ScreenID gameScreenUpdate(GameScreen *gameScreen, float dt)
{
    // Check for escape key to return to main menu (works in both states)
    if (IsKeyPressed(KEY_ESCAPE)) {
        return SCREEN_MAIN_MENU;
    }
    
    if (gameScreen->state == GAME_STATE_PLAYING) {
        // Normal gameplay
        Vector2 velocity = getPlayerInput();
        updatePlayer(&gameScreen->player, velocity, dt);
        updateManager(&gameScreen->entityManager, gameScreen->graphicsManager, dt);
        updateEntities(&gameScreen->entityManager, dt);
        CollisionType collisionType = checkForCollisions(
            &gameScreen->entityManager, &gameScreen->player.hitBoxes);
        switch (collisionType)
        {
            case SCORE_COLLISION:
                gameScreen->score++;
                playScoreSound(gameScreen->soundManager);
                playAnimation(&gameScreen->player);
                break;
            case DEATH_COLLISION:
                // Switch to death overlay
                gameScreen->state = GAME_STATE_DEATH_OVERLAY;
                
                // Initialize cursor manager for game over dialog
                CursorManagerInit();
                gameCursorInitialized = true;
                break;
            default:
                break;
        }
    } else if (gameScreen->state == GAME_STATE_DEATH_OVERLAY) {
        // Death overlay input handling
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || replayButtonPressed) {
            // Replay - reset game state
            if (gameCursorInitialized) {
                CursorManagerCleanup();
                gameCursorInitialized = false;
            }
            replayButtonPressed = false;
            gameScreenReset(gameScreen);
        }
        
        if (quitButtonPressed) {
            // Return to main menu
            if (gameCursorInitialized) {
                CursorManagerCleanup();
                gameCursorInitialized = false;
            }
            quitButtonPressed = false;
            return SCREEN_MAIN_MENU;
        }
    }
    
    return SCREEN_GAME;
}

static int drawGameOverDialogText(const char* text, int fontSize, 
    int dialogX, int dialogY, int dialogWidth, Color color)
{
    int textWidth = MeasureText(text, fontSize);
    int textX = dialogX + (dialogWidth - textWidth) / 2;
    
    DrawText(text, textX, dialogY, fontSize, color);
    
    // Return the height of this text line for spacing calculations
    return fontSize;
}

//TODO I HATE this function. Needs to be made smaller and is too hardcoded.
//Declarative UI's are HARD in C.
static void drawGameOverDialog(int score, int screenWidth, int screenHeight)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, 
                 (Color){0, 0, 0, 180});
    
    // Death dialog dimensions (half screen wide, 1/3 screen tall)
    int dialogWidth = screenWidth / 2;
    int dialogHeight = screenHeight / 3;
    int dialogX = (screenWidth - dialogWidth) / 2;
    int dialogY = (screenHeight - dialogHeight) / 2;
    
    // Dialog background
    DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, GAMEOVER_DIALOG_COLOR);
    DrawRectangleLines(dialogX, dialogY, dialogWidth, dialogHeight, GAMEOVER_DIALOG_OUTLINE_COLOR);
    
    // Text content
    const char* gameOverText = "GAME OVER";
    const char* scoreText = TextFormat("Final Score: %d", score);
    const char* replayText = "Or Press SPACE";
    const char* menuText = "Or Press ESC for Main Menu";
    
    // Draw all text with proper spacing
    int textY = dialogY + 20;
    
    textY += drawGameOverDialogText(gameOverText, GAMEOVER_FONT_SIZE, 
                                   dialogX, textY, dialogWidth, RED);
    textY += 15;  // Spacing after title
    
    textY += drawGameOverDialogText(scoreText, GAMEOVER_SCORE_FONT_SIZE, 
                                   dialogX, textY, dialogWidth, WHITE);
    textY += 20;  // Spacing after score
    
    Rectangle replayButton = {
        (screenWidth / 2.0f) - (GAMEOVER_BUTTON_WIDTH / 2.0f),
        textY, GAMEOVER_BUTTON_WIDTH, GAMEOVER_BUTTON_HEIGHT
    };

    GuiSetStyle(DEFAULT, TEXT_SIZE, MAIN_MENU_BUTTON_FONT_SIZE);
    if (GuiButton(replayButton, "#131#Replay")) {
        replayButtonPressed = true;
    }

    textY += replayButton.height + 5;

    textY += drawGameOverDialogText(replayText, GAMEOVER_INSTRUCTION_FONT_SIZE, 
                                   dialogX, textY, dialogWidth, LIGHTGRAY);
    textY += 20;  // Spacing between instructions

    Rectangle quitButton = {
        (screenWidth / 2.0f) - (GAMEOVER_BUTTON_WIDTH / 2.0f),
        textY, GAMEOVER_BUTTON_WIDTH, GAMEOVER_BUTTON_HEIGHT
    };

    if (GuiButton(quitButton, "#113#Quit")) {
        quitButtonPressed = true;
    }

    // Add cursor manager support for web builds
    if (gameCursorInitialized) {
        Vector2 mousePos = CursorManagerGetPosition();
        
        // Manual button click detection for web compatibility
        if (CursorManagerIsPressed()) {
            if (CheckCollisionPointRec(mousePos, replayButton)) {
                replayButtonPressed = true;
            }
            if (CheckCollisionPointRec(mousePos, quitButton)) {
                quitButtonPressed = true;
            }
        }
        
        // Visual feedback for hover
        if (CheckCollisionPointRec(mousePos, replayButton)) {
            DrawRectangleLines(replayButton.x - 2, replayButton.y - 2, 
                              replayButton.width + 4, replayButton.height + 4, YELLOW);
        }
        if (CheckCollisionPointRec(mousePos, quitButton)) {
            DrawRectangleLines(quitButton.x - 2, quitButton.y - 2, 
                              quitButton.width + 4, quitButton.height + 4, YELLOW);
        }
        
        // Draw custom cursor
        CursorManagerDraw();
    }

    textY += quitButton.height + 5;
    
    drawGameOverDialogText(menuText, GAMEOVER_INSTRUCTION_FONT_SIZE, 
                          dialogX, textY, dialogWidth, LIGHTGRAY);
}

static void drawGameOverOverlay(int score, int screenWidth, int screenHeight)
{
    // Draw the semi-transparent overlay over the entire screen
    DrawRectangle(0, 0, screenWidth, screenHeight, 
                 TRANSPARENCY_OVERLAY_COLOR);
    
    // Draw the game over dialog on top of the overlay
    drawGameOverDialog(score, screenWidth, screenHeight);
}

void gameScreenDraw(GameScreen *gameScreen, int currentScreenWidth, int currentScreenHeight)
{
    BeginTextureMode(gameScreen->target);
        ClearBackground(BACKGROUND_BUFFER_COLOR);
        drawEntities(&gameScreen->entityManager);
        drawPlayer(&gameScreen->player);
        DrawText(TextFormat("%d", gameScreen->score), VIRTUAL_SCREEN_WIDTH/2, 30,
        SCORE_FONT_SIZE, RAYWHITE);
        
        // Draw death overlay if needed
        if (gameScreen->state == GAME_STATE_DEATH_OVERLAY) {
            drawGameOverOverlay(gameScreen->score, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
        }
    EndTextureMode();

    // Clear background for this frame
    ClearBackground(BLACK);
    
    // Calculate position to center the scaled virtual screen
    float scale = fminf((float)currentScreenWidth/VIRTUAL_SCREEN_WIDTH,
        (float)currentScreenHeight/VIRTUAL_SCREEN_HEIGHT);

    int offsetX = (currentScreenWidth - (int)(VIRTUAL_SCREEN_WIDTH * scale)) / 2;
    int offsetY = (currentScreenHeight - (int)(VIRTUAL_SCREEN_HEIGHT * scale)) / 2;
    
    // Draw the virtual screen scaled to fit the window
    Rectangle source = { 0, 0, (float)gameScreen->target.texture.width, 
        (float)gameScreen->target.texture.height * -1};
    Rectangle dest = { offsetX, offsetY, VIRTUAL_SCREEN_WIDTH * scale, 
        VIRTUAL_SCREEN_HEIGHT * scale };
    
    DrawTexturePro(gameScreen->target.texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void gameScreenReset(GameScreen *gameScreen)
{
    // Reset player to spawn position
    resetPlayer(&gameScreen->player);
    
    // Clear all entities
    clearAllEntities(&gameScreen->entityManager);
    
    // Reset score (optional - remove this line if you want to keep score across retries)
    gameScreen->score = 0;
    
    // Return to playing state
    gameScreen->state = GAME_STATE_PLAYING;
    
    // Hide system cursor for gameplay (cursor manager cleanup shows it, so we need to hide it again)
    HideCursor();
}

void gameScreenUnload()
{
    // Cleanup cursor manager if it's still initialized
    if (gameCursorInitialized) {
        CursorManagerCleanup();
        gameCursorInitialized = false;
    }
}
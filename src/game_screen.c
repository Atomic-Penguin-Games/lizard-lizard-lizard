#include "screens.h"
#include "entityManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "player.h"
#include "definitions.h"
#include "math.h"

// Static function prototypes
static void drawGameOverDialog(int score);
static void drawGameOverOverlay(int score);

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
                break;
            default:
                break;
        }
    } else if (gameScreen->state == GAME_STATE_DEATH_OVERLAY) {
        // Death overlay input handling
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            // Replay - reset game state
            gameScreenReset(gameScreen);
        }
    }
    
    return SCREEN_GAME;
}

// static int drawCenteredGameOverText()
// {
//     int 
// }

static void drawGameOverDialog(int score)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, 
                 (Color){0, 0, 0, 180});
    
    // Death dialog dimensions (half screen wide, 1/3 screen tall)
    int dialogWidth = VIRTUAL_SCREEN_WIDTH / 2;
    int dialogHeight = VIRTUAL_SCREEN_HEIGHT / 3;
    int dialogX = (VIRTUAL_SCREEN_WIDTH - dialogWidth) / 2;
    int dialogY = (VIRTUAL_SCREEN_HEIGHT - dialogHeight) / 2;
    
    // Dialog background
    DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, GAMEOVER_DIALOG_COLOR);
    DrawRectangleLines(dialogX, dialogY, dialogWidth, dialogHeight, GAMEOVER_DIALOG_OUTLINE_COLOR);
    
    // Text content
    const char* gameOverText = "GAME OVER";
    const char* scoreText = TextFormat("Final Score: %d", score);
    const char* replayText = "Press SPACE to Replay";
    const char* menuText = "Press ESC for Main Menu";
    
    // Center text in dialog
    int gameOverWidth = MeasureText(gameOverText, GAMEOVER_FONT_SIZE);
    int scoreWidth = MeasureText(scoreText, GAMEOVER_SCORE_FONT_SIZE);
    int replayWidth = MeasureText(replayText, GAMEOVER_INSTRUCTION_FONT_SIZE);
    int menuWidth = MeasureText(menuText, GAMEOVER_INSTRUCTION_FONT_SIZE);
    
    int textX = dialogX + (dialogWidth - gameOverWidth) / 2;
    int textY = dialogY + 20;
    
    DrawText(gameOverText, textX, textY, GAMEOVER_FONT_SIZE, RED);
    
    textX = dialogX + (dialogWidth - scoreWidth) / 2;
    textY += GAMEOVER_FONT_SIZE + 15;
    DrawText(scoreText, textX, textY, GAMEOVER_SCORE_FONT_SIZE, WHITE);
    
    textX = dialogX + (dialogWidth - replayWidth) / 2;
    textY += GAMEOVER_SCORE_FONT_SIZE + 20;
    DrawText(replayText, textX, textY, GAMEOVER_INSTRUCTION_FONT_SIZE, LIGHTGRAY);
    
    textX = dialogX + (dialogWidth - menuWidth) / 2;
    textY += GAMEOVER_INSTRUCTION_FONT_SIZE + 10;
    DrawText(menuText, textX, textY, GAMEOVER_INSTRUCTION_FONT_SIZE, LIGHTGRAY);
}

static void drawGameOverOverlay(int score)
{
    // Draw the semi-transparent overlay over the entire screen
    DrawRectangle(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, 
                 TRANSPARENCY_OVERLAY_COLOR);
    
    // Draw the game over dialog on top of the overlay
    drawGameOverDialog(score);
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
            drawGameOverOverlay(gameScreen->score);
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
}

void gameScreenUnload()
{
    
}
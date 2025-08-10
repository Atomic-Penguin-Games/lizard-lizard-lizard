#include "screens.h"
#include "entityManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "player.h"
#include "definitions.h"
#include "math.h"

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
        .score = 0
    };
    initHitboxPointers(&gameScreen.player);
    return gameScreen;
}

ScreenID gameScreenUpdate(GameScreen *gameScreen, float dt)
{
    Vector2 velocity = getPlayerInput();
    updatePlayer(&gameScreen->player, velocity, dt);
    updateManager(&gameScreen->entityManager, gameScreen->graphicsManager, dt);
    updateEntities(&gameScreen->entityManager, dt);
    CollisionType collisionType = checkForCollisions(
        &gameScreen->entityManager, gameScreen->player.hitboxes);
    switch (collisionType)
    {
        case SCORE_COLLISION:
            gameScreen->score++;
            playScoreSound(gameScreen->soundManager);
            playAnimation(&gameScreen->player);
            break;
        case DEATH_COLLISION:
            break;
        default:
            break;
    }
    return SCREEN_GAME;
}

void gameScreenDraw(GameScreen *gameScreen, int currentScreenWidth, int currentScreenHeight)
{
    BeginTextureMode(gameScreen->target);
        ClearBackground(RED);
        drawEntities(&gameScreen->entityManager);
        drawPlayer(&gameScreen->player);
        DrawText(TextFormat("%d", gameScreen->score), VIRTUAL_SCREEN_WIDTH/2, 30,
        SCORE_FONT_SIZE, RAYWHITE);
    EndTextureMode();

    BeginDrawing();
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
    EndDrawing();
}
void gameScreenUnload()
{
    
}
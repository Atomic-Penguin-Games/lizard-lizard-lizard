#include "player.h"
#include "definitions.h"

// Static variables for animation frame calculations
static int frameWidth = 0;
static int frameHeight = 0;
static bool frameDataInitialized = false;

static void initFrameData(Texture *spriteSheet) 
{
    if (!frameDataInitialized) {
        // Calculate based on actual texture dimensions - no gap assumption
        frameWidth = spriteSheet->width / 11;  // Simple division by frame count
        frameHeight = spriteSheet->height;
        frameDataInitialized = true;
    }
}

Player createPlayer(Texture *spriteSheet)
{
    initFrameData(spriteSheet);  // Initialize frame data once with actual texture
    
    Player player = {
        .position = PLAYER_SPAWN_LOCATION,
        .spriteSheet = *spriteSheet,
        .hitbox = {PLAYER_SPAWN_LOCATION.x + PLAYER_HITBOX_X_OFFSET,
                PLAYER_SPAWN_LOCATION.y + PLAYER_HITBOX_Y_OFFSET,
                PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT
            },
        .headbox = {
            PLAYER_SPAWN_LOCATION.x + PLAYER_HEADBOX_X_OFFSET,
            PLAYER_SPAWN_LOCATION.y + PLAYER_HEADBOX_Y_OFFSET,
            PLAYER_HEADBOX_WIDTH, PLAYER_HEADBOX_HEIGHT
        },
        .isAnimating = false,
        .currentFrame = 0,
        .animationTimer = 0.0f,
        .frameTime = 0.1f  // 10 FPS animation
    };
    updateHitboxes(&player);
    return player;
}

void drawPlayer(Player *player)
{
    if (player->isAnimating)
    {
        // Calculate source rectangle for current frame (no gap assumption)
        Rectangle source = {
            player->currentFrame * frameWidth, // No gap offset
            0,
            frameWidth,
            frameHeight
        };
        
        // Draw the animation frame
        Rectangle dest = {
            player->position.x,
            player->position.y,
            frameWidth * PLAYER_SPRITE_SCALE * 2.0f,  // Double the size
            frameHeight * PLAYER_SPRITE_SCALE * 2.0f  // Double the size
        };
        
        DrawTexturePro(player->spriteSheet, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else
    {
        // Draw first frame of animation as idle sprite
        Rectangle source = {
            0, // First frame
            0,
            frameWidth,
            frameHeight
        };
        
        Rectangle dest = {
            player->position.x,
            player->position.y,
            frameWidth * PLAYER_SPRITE_SCALE * 2.0f,  // Match animation scale
            frameHeight * PLAYER_SPRITE_SCALE * 2.0f  // Match animation scale
        };
        
        DrawTexturePro(player->spriteSheet, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    
    if (DEBUG_MODE)
    {
        DrawRectangleLinesEx(player->hitbox, 2, BLUE);
        DrawRectangleLinesEx(player->headbox, 2, GREEN);
        //DrawRectangleRec(player.hitbox, GREEN); // Assuming you want to visualize the hitbox in green
    }
}

void updatePlayer(Player *player, Vector2 velocity, float deltaTime,
int screenWidth, int screenHeight)
{
    player->position.x += velocity.x * deltaTime;
    player->position.y += velocity.y * deltaTime;

    if (player->position.y < 0)
    {
        player->position.y = 0;
    }else if (player->position.y > screenHeight - PLAYER_SPRITE_HEIGHT)
    {
        player->position.y = screenHeight - PLAYER_SPRITE_HEIGHT;
    }

    if (player->position.x < 0)
    {
        player->position.x = 0;
    } else if (player->position.x > screenWidth - PLAYER_SPRITE_WIDTH)
    {
        player->position.x = screenWidth - PLAYER_SPRITE_WIDTH;
    }
    
    // Update animation
    if (player->isAnimating)
    {
        player->animationTimer += deltaTime;
        
        if (player->animationTimer >= player->frameTime)
        {
            player->currentFrame++;
            player->animationTimer = 0.0f;
            
            // Animation has 11 frames (0-10)
            if (player->currentFrame >= 11)
            {
                player->isAnimating = false;
                player->currentFrame = 0;
            }
        }
    }
    
    updateHitboxes(player);
}

void updateHitboxes(Player *player)
{
    
    player->hitbox.x = player->position.x + PLAYER_HITBOX_X_OFFSET;
    player->hitbox.y = player->position.y + PLAYER_HITBOX_Y_OFFSET;
    
    player->headbox.x = player->position.x + PLAYER_HEADBOX_X_OFFSET;
    player->headbox.y = player->position.y + PLAYER_HEADBOX_Y_OFFSET;
}

void playAnimation(Player *player)
{
    player->isAnimating = true;
    player->currentFrame = 0;
    player->animationTimer = 0.0f;
}
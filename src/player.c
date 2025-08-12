#include "player.h"
#include "definitions.h"
#include <stdio.h>

// Static variables for animation frame calculations
static int frameWidth = 0;
static int frameHeight = 0;
static bool frameDataInitialized = false;

static void initFrameData(Texture *spriteSheet) 
{
    if (!frameDataInitialized) {
        // Calculate based on actual texture dimensions - no gap assumption
        frameWidth = spriteSheet->width / 9;  // Simple division by frame count
        frameHeight = spriteSheet->height;
        frameDataInitialized = true;
    }
}

Player createPlayer(Texture *spriteSheet)
{
    initFrameData(spriteSheet);  // Initialize frame data once with actual texture
    
    Player player = {
        .position = PLAYER_SPAWN_LOCATION,
        .spriteSheet = spriteSheet,  // Store pointer instead of copying texture
        .hitBoxes = {
            [HITBOX_HEAD] = {
                .rect = {0, 0, PLAYER_HEADBOX_WIDTH, PLAYER_HEADBOX_HEIGHT},
                .enabled = true,
                .xOffset = PLAYER_HEADBOX_X_OFFSET,
                .yOffset = PLAYER_HEADBOX_Y_OFFSET
            },
            [HITBOX_BODY] = {
                .rect = {0, 0, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT},
                .enabled = true,
                .xOffset = PLAYER_HITBOX_X_OFFSET,
                .yOffset = PLAYER_HITBOX_Y_OFFSET
            },
            [HITBOX_TAIL] = {
                .rect = {0, 0, PLAYER_TAILBOX_WIDTH, PLAYER_TAILBOX_HEIGHT},
                .enabled = true,
                .xOffset = PLAYER_TAILBOX_X_OFFSET,
                .yOffset = PLAYER_TAILBOX_Y_OFFSET
            },
            [HITBOX_LOWER_TAIL] = {
                .rect = {0, 0, PLAYER_LOWER_TAILBOX_WIDTH, PLAYER_LOWER_TAILBOX_HEIGHT},
                .enabled = true,
                .xOffset = PLAYER_LOWER_TAILBOX_X_OFFSET,
                .yOffset = PLAYER_LOWER_TAILBOX_Y_OFFSET
            },
        },
        .isAnimating = false,
        .currentFrame = 0,
        .animationTimer = 0.0f,
        .frameTime = 0.03f,  // 10 FPS animation
        .facingLeft = false  // Start facing right
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
        
        // Flip sprite horizontally if facing left
        if (player->facingLeft)
        {
            source.width = -frameWidth;  // Negative width flips horizontally
        }
        
        // Draw the animation frame
        Rectangle dest = {
            player->position.x,
            player->position.y,
            frameWidth * PLAYER_SPRITE_SCALE * 2.0f,  // Double the size
            frameHeight * PLAYER_SPRITE_SCALE * 2.0f  // Double the size
        };
        
        DrawTexturePro(*player->spriteSheet, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
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
        
        // Flip sprite horizontally if facing left
        if (player->facingLeft)
        {
            source.width = -frameWidth;  // Negative width flips horizontally
        }
        
        Rectangle dest = {
            player->position.x,
            player->position.y,
            frameWidth * PLAYER_SPRITE_SCALE * 2.0f,  // Match animation scale
            frameHeight * PLAYER_SPRITE_SCALE * 2.0f  // Match animation scale
        };
        
        DrawTexturePro(*player->spriteSheet, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    
    if (DEBUG_MODE)
    {
        DrawRectangleLinesEx(player->hitBoxes[0].rect, 2, BLUE);
        DrawRectangleLinesEx(player->hitBoxes[1].rect, 2, GREEN);
        DrawRectangleLinesEx(player->hitBoxes[2].rect, 2, PINK);
        DrawRectangleLinesEx(player->hitBoxes[3].rect, 2, GREEN);
    }
}

void updatePlayer(Player *player, Vector2 velocity, float deltaTime)
{
    // Update facing direction based on movement
    if (velocity.x < 0)
    {
        player->facingLeft = true;
    }
    else if (velocity.x > 0)
    {
        player->facingLeft = false;
    }
    
    player->position.x += velocity.x * deltaTime;
    player->position.y += velocity.y * deltaTime;

    if (player->position.y < 0)
    {
        player->position.y = 0;
    }else if (player->position.y > VIRTUAL_SCREEN_HEIGHT - PLAYER_SPRITE_HEIGHT)
    {
        player->position.y = VIRTUAL_SCREEN_HEIGHT - PLAYER_SPRITE_HEIGHT;
    }

    if (player->position.x < 0)
    {
        player->position.x = 0;
    } else if (player->position.x > VIRTUAL_SCREEN_WIDTH - PLAYER_SPRITE_WIDTH)
    {
        player->position.x = VIRTUAL_SCREEN_WIDTH - PLAYER_SPRITE_WIDTH;
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
    //resizeHitboxes(player);
    updateHitboxes(player);
}

void updateHitboxes(Player *player)
{
    if (player->facingLeft)
    {
        //Mirror sprite to face left
        //Calculate the sprite width at current scale
        float spriteWidth = frameWidth * PLAYER_SPRITE_SCALE * 2.0f;  // Same scale as used in drawing
        
        // Formula: rightEdge - (originalOffset + width) for the new left edge
        for (int i = 0; i < PLAYER_HITBOX_COUNT; i++)
        {
            if (!player->hitBoxes[i].enabled) continue;
            
            player->hitBoxes[i].rect.x = player->position.x + spriteWidth - 
                (player->hitBoxes[i].xOffset + player->hitBoxes[i].rect.width);
            player->hitBoxes[i].rect.y = player->position.y + player->hitBoxes[i].yOffset;
        }
    }
    else
    {
        for (int i = 0; i < PLAYER_HITBOX_COUNT; i++)
        {
            if (!player->hitBoxes[i].enabled) continue;
            
            player->hitBoxes[i].rect.x = player->position.x + player->hitBoxes[i].xOffset;
            player->hitBoxes[i].rect.y = player->position.y + player->hitBoxes[i].yOffset;
        }
    }
}

void playAnimation(Player *player)
{
    player->isAnimating = true;
    player->currentFrame = 0;
    player->animationTimer = 0.0f;
}

void resetPlayer(Player *player)
{
    // Reset position to spawn location
    player->position = PLAYER_SPAWN_LOCATION;
    
    // Reset velocity
    player->velocity = (Vector2){0, 0};
    
    // Reset animation state
    player->isAnimating = false;
    player->currentFrame = 0;
    player->animationTimer = 0.0f;
    
    // Reset facing direction
    player->facingLeft = false;
    
    // Update hitboxes to new position
    updateHitboxes(player);
}
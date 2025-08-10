#include "player.h"
#include "definitions.h"

void resizeHitboxes(Player *player);
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
        .hitbox = {PLAYER_SPAWN_LOCATION.x + PLAYER_HITBOX_X_OFFSET,
                PLAYER_SPAWN_LOCATION.y + PLAYER_HITBOX_Y_OFFSET,
                PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT
            },
        .headbox = {
            PLAYER_SPAWN_LOCATION.x + PLAYER_HEADBOX_X_OFFSET,
            PLAYER_SPAWN_LOCATION.y + PLAYER_HEADBOX_Y_OFFSET,
            PLAYER_HEADBOX_WIDTH, PLAYER_HEADBOX_HEIGHT
        },
        .tailbox = {
            PLAYER_SPAWN_LOCATION.x + PLAYER_TAILBOX_X_OFFSET,
            PLAYER_SPAWN_LOCATION.y + PLAYER_TAILBOX_Y_OFFSET,
            PLAYER_TAILBOX_WIDTH, PLAYER_TAILBOX_HEIGHT
        },
        .lowerTailbox = {
            PLAYER_SPAWN_LOCATION.x + PLAYER_LOWER_TAILBOX_X_OFFSET,
            PLAYER_SPAWN_LOCATION.y + PLAYER_LOWER_TAILBOX_Y_OFFSET,
            PLAYER_LOWER_TAILBOX_WIDTH, PLAYER_LOWER_TAILBOX_HEIGHT
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

void initHitboxPointers(Player *player)
{
    player->hitboxes[0] = &player->hitbox;
    player->hitboxes[1] = &player->headbox;
    player->hitboxes[2] = &player->tailbox;
    player->hitboxes[3] = &player->lowerTailbox;
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
        DrawRectangleLinesEx(player->hitbox, 2, BLUE);
        DrawRectangleLinesEx(player->headbox, 2, GREEN);
        DrawRectangleLinesEx(player->tailbox, 2, PINK);
        DrawRectangleLinesEx(player->lowerTailbox, 2, GREEN);
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
        // When facing left, hitboxes need to be mirrored from the right edge
        // We need to calculate the sprite width at the current scale
        float spriteWidth = frameWidth * PLAYER_SPRITE_SCALE * 2.0f;  // Same scale as used in drawing
        
        // Formula: rightEdge - (originalOffset + width) for the new left edge
        player->hitbox.x = player->position.x + spriteWidth - (PLAYER_HITBOX_X_OFFSET + PLAYER_HITBOX_WIDTH);
        player->hitbox.y = player->position.y + PLAYER_HITBOX_Y_OFFSET;
        
        player->headbox.x = player->position.x + spriteWidth - (PLAYER_HEADBOX_X_OFFSET + PLAYER_HEADBOX_WIDTH);
        player->headbox.y = player->position.y + PLAYER_HEADBOX_Y_OFFSET;
        
        player->tailbox.x = player->position.x + spriteWidth - (PLAYER_TAILBOX_X_OFFSET + PLAYER_TAILBOX_WIDTH);
        player->tailbox.y = player->position.y + PLAYER_TAILBOX_Y_OFFSET;
        
        player->lowerTailbox.x = player->position.x + spriteWidth - (PLAYER_LOWER_TAILBOX_X_OFFSET + PLAYER_LOWER_TAILBOX_WIDTH);
        player->lowerTailbox.y = player->position.y + PLAYER_LOWER_TAILBOX_Y_OFFSET;
    }
    else
    {
        // Normal hitbox positioning (facing right)
        player->hitbox.x = player->position.x + PLAYER_HITBOX_X_OFFSET;
        player->hitbox.y = player->position.y + PLAYER_HITBOX_Y_OFFSET;
        
        player->headbox.x = player->position.x + PLAYER_HEADBOX_X_OFFSET;
        player->headbox.y = player->position.y + PLAYER_HEADBOX_Y_OFFSET;
        
        player->tailbox.x = player->position.x + PLAYER_TAILBOX_X_OFFSET;
        player->tailbox.y = player->position.y + PLAYER_TAILBOX_Y_OFFSET;
        
        player->lowerTailbox.x = player->position.x + PLAYER_LOWER_TAILBOX_X_OFFSET;
        player->lowerTailbox.y = player->position.y + PLAYER_LOWER_TAILBOX_Y_OFFSET;
    }
}

void playAnimation(Player *player)
{
    player->isAnimating = true;
    player->currentFrame = 0;
    player->animationTimer = 0.0f;
}

//used to resize hitboxes to figure out the values to set in definitions.
//updateHitboxes() needs to be disabled or it overwrites whatever is modified here.
// void resizeHitboxes(Player *player)
// {
//     Rectangle rect = player->hitbox;
//     if (IsKeyPressed(KEY_T)) rect.y -= 5;
//     if (IsKeyPressed(KEY_G)) rect.y += 5;
//     if (IsKeyPressed(KEY_F)) rect.x -= 5;
//     if (IsKeyPressed(KEY_H)) rect.x += 5;

//     if (IsKeyPressed(KEY_I)) rect.height += 5;
//     if (IsKeyPressed(KEY_K)) rect.height -= 5;
//     if (IsKeyPressed(KEY_J)) rect.width -= 5;
//     if (IsKeyPressed(KEY_L)) rect.width += 5;
//     // Print rect data when P is pressed
//     if (IsKeyPressed(KEY_P))
//     {
//         printf("Player rect Data:\n");
//         printf("  Position: (%.2f, %.2f)\n", player->position.x, player->position.y);
//         printf("  rect: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", 
//                rect.x, rect.y, rect.width, rect.height);
//         printf("  rect: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", 
//                rect.x, rect.y, rect.width, rect.height);
//         printf("  rect: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", 
//                rect.x, rect.y, rect.width, rect.height);
//         printf("  Animation: frame=%d, animating=%s\n", 
//                player->currentFrame, player->isAnimating ? "true" : "false");
//         printf("---\n");
//     }
//     player->hitbox = rect;
// }
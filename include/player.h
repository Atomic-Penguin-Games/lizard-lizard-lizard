#pragma once
#include <raylib.h>
#include "definitions.h"
#include "soundManager.h"

typedef enum {
  HITBOX_HEAD = 0,
  HITBOX_BODY = 1,
  HITBOX_TAIL = 2,
  HITBOX_LOWER_TAIL = 3
} HitboxType;

typedef struct {
  Rectangle rect;
  float xOffset;
  float yOffset;
  bool enabled;
} PlayerHitbox;

typedef struct Player {
  Vector2 position;
  Vector2 velocity;
  PlayerHitbox hitBoxes[PLAYER_HITBOX_COUNT];
  Texture *spriteSheet;  // Use pointer instead of copy
  
  // Animation fields
  bool isAnimating;
  int currentFrame;
  float animationTimer;
  float frameTime;
  
  // Facing direction
  bool facingLeft;
} Player;

Player createPlayer(Texture *sprite);
void initHitboxPointers(Player *player);
void drawPlayer(Player *player);
void updatePlayer(Player *player, Vector2 velocity, float deltaTime);
void updateHitboxes(Player *player);
void playAnimation(Player *player);
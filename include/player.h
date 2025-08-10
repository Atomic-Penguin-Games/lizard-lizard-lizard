#pragma once
#include <raylib.h>
#include "definitions.h"
#include "soundManager.h"

typedef struct Player
{
  Vector2 position;
  Vector2 velocity;
  Rectangle hitbox;
  Rectangle headbox;
  Rectangle tailbox;
  Rectangle lowerTailbox;
  Texture *spriteSheet;  // Use pointer instead of copy
  Vector2 hitboxOffset;
  
  // Array of pointers to all hitboxes for easy collision checking
  Rectangle *hitboxes[PLAYER_HITBOX_COUNT];
  
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
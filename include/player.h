#pragma once
#include <raylib.h>

typedef struct Player
{
  Vector2 position;
  Vector2 velocity;
  Rectangle hitbox;
  Rectangle headbox;
  Texture spriteSheet;
  Vector2 hitboxOffset;
  
  // Animation fields
  bool isAnimating;
  int currentFrame;
  float animationTimer;
  float frameTime;
} Player;

Player createPlayer(Texture *sprite);
void drawPlayer(Player *player);
void updatePlayer(Player *player, Vector2 velocity, float deltaTime,
    int screenWidth, int screenHeight);
void updateHitboxes(Player *player);
void playAnimation(Player *player);
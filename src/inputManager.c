#include "inputManager.h"
#include "definitions.h"
#include <raymath.h>

Vector2 getPlayerInput()
{
    Vector2 velocity = { 0, 0 };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      {
        velocity.y = -1.0f;
      }
      if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      {
        velocity.y = 1.0f;
      }
      if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      {
        velocity.x = -1.0f;
      }
      if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      {
        velocity.x = 1.0f;
      }
      
      // Normalize the velocity vector to prevent diagonal speed boost
      if (velocity.x != 0.0f || velocity.y != 0.0f) {
          velocity = Vector2Normalize(velocity);
          velocity = Vector2Scale(velocity, PLAYER_SPEED);
      }
      
      return velocity;
}
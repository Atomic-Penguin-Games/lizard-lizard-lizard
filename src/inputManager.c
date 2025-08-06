#include "inputManager.h"
#include "definitions.h"

Vector2 getPlayerInput()
{
    Vector2 velocity = { 0, 0 };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      {
        velocity.y = PLAYER_SPEED * -1;
      }
      if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      {
        velocity.y = PLAYER_SPEED;
      }
      if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      {
        velocity.x = PLAYER_SPEED * -1;
      }
      if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      {
        velocity.x = PLAYER_SPEED;
      }
      return velocity;
}
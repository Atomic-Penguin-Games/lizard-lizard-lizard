#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SPEED 420.0f
#define IGUANA_SPEED 570.0f
#define OBJECT_SPAWN_RATE_MIN 0.65f // Minimum spawn time in seconds
#define OBJECT_SPAWN_RATE_MAX 1.7f // Maximum spawn time in seconds
#define DEBUG_MODE true
#define MAX_IGUANAS 100
#define SAW_ROTATION_SPEED 50.0f

int randomNum(int max);
float randomSpawnTime();
int score = 0;


typedef struct Player
{
  Vector2 position;
  Rectangle hitbox;
  Texture sprite;
  Vector2 hitboxOffset;
} Player;

typedef struct IguanaData
{
  Rectangle hitbox;
  Texture sprite;
  Vector2 hitboxOffset;
} IguanaData;

typedef struct Iguana
{
  Vector2 position;
  IguanaData data; 
} Iguana;

typedef struct IguanaManager
{
  Iguana iguanas[MAX_IGUANAS];
  int count;
  float spawnTimer;
} IguanaManager;

typedef struct Circle
{
  Vector2 center;
  float radius;
} Circle;

typedef struct Sawblade
{
  Vector2 position;
  Circle hitbox;
  Texture sprite;
  Vector2 hitboxOffset;
  float rotation;
} Sawblade;

typedef struct SawbladeManager
{
  Sawblade sawblades[MAX_IGUANAS];
  int count;
} SawbladeManager;

void drawPlayer(Player player);
void updatePlayer(Player *player);
void drawIguana(Iguana iguana);
void updateIguana(Iguana *iguana, float deltaTime, int screenWidth);
void spawnIguana(IguanaManager *manager, IguanaData iguanaData, int screenWidth, int screenHeight);
void updateIguanaManager(IguanaManager *manager, float deltaTime, IguanaData iguanaData, int screenWidth, int screenHeight);
void drawIguanaManager(IguanaManager manager);
void removeIguana(IguanaManager *manager, int index);
void drawSaw(Sawblade sawblade);
void updateSawblade(Sawblade *sawblade, float deltaTime, int screenWidth);
void spawnSawblade(SawbladeManager *manager, Texture sawSprite, Vector2 hitboxOffset, int screenWidth, int screenHeight);
void updateSawbladeManager(SawbladeManager *manager, float deltaTime, int screenWidth);
void drawSawbladeManager(SawbladeManager manager);
void removeSawblade(SawbladeManager *manager, int index);

int main(void)
{
    srand(time(NULL));
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Lizard Meme");
    SetTargetFPS(60);

    InitAudioDevice();

    Texture sprite = LoadTexture("res/lizardSprite.png");
    Texture iguanaSprite = LoadTexture("res/lizardEmoji.png");
    Texture sawSprite = LoadTexture("res/circular_saw_blade.png");

    Sound lizardSound = LoadSound("res/lizard.wav");
    Sound* lizardPtr = &lizardSound;

    Sound sound2 = LoadSound("res/lizardUpShift.wav");
    Sound sound3 = LoadSound("res/lizardDownShift.wav");
    Sound* ptr2 = &sound2;
    Sound* ptr3 = &sound3;

    Sound* sounds[3];
    sounds[0] = lizardPtr;
    sounds[1] = ptr2;
    sounds[2] = ptr3;

    Player player = {
      .position = { 50, 50},
      .hitbox = {100, 60, 160, 200},
      .sprite = sprite,
      .hitboxOffset = {50, 10}
    };
    //DrawRectangle(player.position.x+50, player.position.y+10, 160, 200, BLUE);

    // IguanaData iguanaData = {
    //   .hitbox = {0, 0, 200, 90},
    //   .sprite = iguanaSprite
    // };

    IguanaData iguanaData = {
      .hitbox = {0, 0, 110, 80},
      .sprite = iguanaSprite,
      .hitboxOffset = {0, 0}
    };

    IguanaManager iguanaManager = {
      .count = 0,
      .spawnTimer = 0.0f
    };

    SawbladeManager sawbladeManager = {
      .count = 0
    };

    float spawnTimer = 0.0f;
    float nextSpawnTime = randomSpawnTime(); // Set initial random spawn time

    while (!WindowShouldClose())
    {
      float dT = GetFrameTime(); //deltaTime
      

      if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      {
        player.position.y -= SPEED * dT;
        if (player.position.y < 0)
        {
          player.position.y = 0;
        }
      }
      if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      {
        player.position.y += SPEED * dT;
        if (player.position.y + player.hitbox.height > screenHeight)
        {
          player.position.y = screenHeight - player.hitbox.height;
        }
      }
      if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      {
        player.position.x -= SPEED * dT;
        if (player.position.x < 0)
        {
          player.position.x = 0;
        }
      }
      if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      {
        player.position.x += SPEED * dT;
        if (player.position.x + player.hitbox.width > screenWidth)
        {
          player.position.x = screenWidth - player.hitbox.width;
        }
      }
      updatePlayer(&player);
      
      // Update spawn timer and spawn enemies
      spawnTimer += dT;
      if (spawnTimer >= nextSpawnTime)
      {
        // Randomly choose between spawning an iguana or sawblade (50/50 chance)
        if (randomNum(2) == 1)
        {
          spawnIguana(&iguanaManager, iguanaData, screenWidth, screenHeight);
        }
        else
        {
          Vector2 sawHitboxOffset = {60, 60}; // Center the circular hitbox
          spawnSawblade(&sawbladeManager, sawSprite, sawHitboxOffset, screenWidth, screenHeight);
        }
        spawnTimer = 0.0f;
        nextSpawnTime = randomSpawnTime(); // Set new random spawn time
      }
      
      updateIguanaManager(&iguanaManager, dT, iguanaData, screenWidth, screenHeight);
      updateSawbladeManager(&sawbladeManager, dT, screenWidth);

      for (int i = 0; i < iguanaManager.count; i++)
      {
        if (CheckCollisionRecs(player.hitbox, iguanaManager.iguanas[i].data.hitbox))
        {
          PlaySound(*sounds[randomNum(2)]);
          removeIguana(&iguanaManager, i);
          score++;
          break; // Exit loop to avoid index issues
        }
      }

      for (int i = 0; i < sawbladeManager.count; i++)
      {
        if (CheckCollisionCircleRec(sawbladeManager.sawblades[i].hitbox.center, sawbladeManager.sawblades[i].hitbox.radius, player.hitbox))
        {
          removeSawblade(&sawbladeManager, i);
          //TODO Kill the player
          break; // Exit loop to avoid index issues
        }
      }
      
      BeginDrawing();
      ClearBackground(RED);
      //DrawText("Hola Mundo\nLizard Meme", screenWidth/2, screenHeight/2, 40, DARKGRAY);
      drawIguanaManager(iguanaManager);
      drawSawbladeManager(sawbladeManager);
      drawPlayer(player);
      DrawText(TextFormat("%d", score), screenWidth/2, 30, 60, RAYWHITE);

      EndDrawing();
    }

    return 0;
    CloseWindow();
}

int randomNum(int max)
{
  int rng = rand();
  return ((rng % max)+1);
}

float randomSpawnTime()
{
  // Generate random time between min and max in 0.1 second intervals
  float minTime = OBJECT_SPAWN_RATE_MIN;
  float maxTime = OBJECT_SPAWN_RATE_MAX;
  
  // Convert to 0.1 second intervals
  int minIntervals = (int)(minTime * 10);
  int maxIntervals = (int)(maxTime * 10);
  
  // Generate random interval count
  int intervalRange = maxIntervals - minIntervals + 1;
  int randomInterval = minIntervals + (rand() % intervalRange);
  
  // Convert back to seconds
  return (float)randomInterval / 10.0f;
}

void drawPlayer(Player player)
{
  DrawTextureEx(player.sprite, player.position, 0.0f, 0.28f, RAYWHITE);
  if (DEBUG_MODE)
  {
    DrawRectangleLinesEx(player.hitbox, 2, BLUE);
    //DrawRectangleRec(player.hitbox, GREEN); // Assuming you want to visualize the hitbox in green
  }
}

void updatePlayer(Player *player)
{
  player->hitbox.x = player->position.x + player->hitboxOffset.x;
  player->hitbox.y = player->position.y + player->hitboxOffset.y;
}
void updateIguana(Iguana *iguana, float deltaTime, int screenWidth)
{
  iguana->position.x -= IGUANA_SPEED * deltaTime;
  iguana->data.hitbox.x = iguana->position.x + iguana->data.hitboxOffset.x;
  iguana->data.hitbox.y = iguana->position.y + iguana->data.hitboxOffset.y;
}

void spawnIguana(IguanaManager *manager, IguanaData iguanaData, int screenWidth, int screenHeight)
{
  if (manager->count < MAX_IGUANAS)
  {
    Iguana newIguana = {
      .position = { screenWidth, randomNum(screenHeight - (int)iguanaData.hitbox.height) },
      .data = iguanaData
    };
    newIguana.data.hitbox.x = newIguana.position.x + newIguana.data.hitboxOffset.x;
    newIguana.data.hitbox.y = newIguana.position.y + newIguana.data.hitboxOffset.y;
    
    manager->iguanas[manager->count] = newIguana;
    manager->count++;
  }
}

void updateIguanaManager(IguanaManager *manager, float deltaTime, IguanaData iguanaData, int screenWidth, int screenHeight)
{
  // Update all iguanas and remove those that are off-screen
  for (int i = 0; i < manager->count; i++)
  {
    updateIguana(&manager->iguanas[i], deltaTime, screenWidth);
    
    // Remove iguanas that have moved off-screen to the left
    if (manager->iguanas[i].position.x + manager->iguanas[i].data.hitbox.width < 0)
    {
      removeIguana(manager, i);
      i--; // Decrement i to check the same index again
    }
  }
}

void drawSaw(Sawblade sawblade)
{
  // Calculate the center of the texture for rotation
  Vector2 origin = { sawblade.sprite.width * 0.1f / 2.0f, sawblade.sprite.height * 0.1f / 2.0f };
  
  // Source rectangle (entire texture)
  Rectangle source = { 0, 0, sawblade.sprite.width, sawblade.sprite.height };
  
  // Destination rectangle (position and scaled size)
  Rectangle dest = { 
    sawblade.position.x + origin.x, 
    sawblade.position.y + origin.y, 
    sawblade.sprite.width * 0.1f, 
    sawblade.sprite.height * 0.1f 
  };
  
  DrawTexturePro(sawblade.sprite, source, dest, origin, sawblade.rotation, RAYWHITE);
  
  if (DEBUG_MODE)
  {
    DrawCircleLinesV(sawblade.hitbox.center, sawblade.hitbox.radius, PINK);
  }
}

void drawIguanaManager(IguanaManager manager)
{
  for (int i = 0; i < manager.count; i++)
  {
    drawIguana(manager.iguanas[i]);
  }
}

void drawIguana(Iguana iguana)
{
  DrawTextureEx(iguana.data.sprite, iguana.position, 0.0f, 1.5f, RAYWHITE);
  if (DEBUG_MODE)
  {
    DrawRectangleLinesEx(iguana.data.hitbox, 2, BLUE);  
  }
}

void removeIguana(IguanaManager *manager, int index)
{
  if (index >= 0 && index < manager->count)
  {
    // Shift remaining iguanas down to fill the gap
    for (int j = index; j < manager->count - 1; j++)
    {
      manager->iguanas[j] = manager->iguanas[j + 1];
    }
    manager->count--;
  }
}

void updateSawblade(Sawblade *sawblade, float deltaTime, int screenWidth)
{
  sawblade->position.x -= IGUANA_SPEED * deltaTime;
  sawblade->hitbox.center.x = sawblade->position.x + sawblade->hitboxOffset.x;
  sawblade->hitbox.center.y = sawblade->position.y + sawblade->hitboxOffset.y;
  sawblade->rotation += SAW_ROTATION_SPEED * deltaTime;
}

void spawnSawblade(SawbladeManager *manager, Texture sawSprite, Vector2 hitboxOffset, int screenWidth, int screenHeight)
{
  if (manager->count < MAX_IGUANAS)
  {
    Sawblade newSawblade = {
      .position = { screenWidth, randomNum(screenHeight - 100) },
      .hitbox = { .center = { 0, 0 }, .radius = 50 },
      .sprite = sawSprite,
      .hitboxOffset = hitboxOffset,
      .rotation = 0.0f
    };
    newSawblade.hitbox.center.x = newSawblade.position.x + newSawblade.hitboxOffset.x;
    newSawblade.hitbox.center.y = newSawblade.position.y + newSawblade.hitboxOffset.y;
    
    manager->sawblades[manager->count] = newSawblade;
    manager->count++;
  }
}

void updateSawbladeManager(SawbladeManager *manager, float deltaTime, int screenWidth)
{
  // Update all sawblades and remove those that are off-screen
  for (int i = 0; i < manager->count; i++)
  {
    updateSawblade(&manager->sawblades[i], deltaTime, screenWidth);
    
    // Remove sawblades that have moved off-screen to the left
    if (manager->sawblades[i].position.x + 80 < 0) // 80 is approximate sawblade width
    {
      removeSawblade(manager, i);
      i--; // Decrement i to check the same index again
    }
  }
}

void drawSawbladeManager(SawbladeManager manager)
{
  for (int i = 0; i < manager.count; i++)
  {
    drawSaw(manager.sawblades[i]);
  }
}

void removeSawblade(SawbladeManager *manager, int index)
{
  if (index >= 0 && index < manager->count)
  {
    // Shift remaining sawblades down to fill the gap
    for (int j = index; j < manager->count - 1; j++)
    {
      manager->sawblades[j] = manager->sawblades[j + 1];
    }
    manager->count--;
  }
} 
#include "entityManager.h"
#include "randomizer.h"
#include <stdio.h>

EntityManager initManager()
{
    EntityManager manager = {
      .entities = {},
      .count = 0,
      .spawnTimer = OBJECT_SPAWN_RATE_MIN,
      .nextSpawnTime = 0.25f
    };
    return manager;
}

void updateManager(EntityManager *manager, GraphicsManager *gm, float deltaTime,
    int screenWidth, int screenHeight)
{
    manager->spawnTimer += deltaTime;
    if (manager->spawnTimer >= manager->nextSpawnTime)
      {
        // Randomly choose between spawning an iguana or sawblade (50/50 chance)
        if (randomNum(2) == 1)
        {
          spawnEntity(manager, LIZARD, &gm->lizardSprite, screenWidth, screenHeight);
        }
        else
        {
          spawnEntity(manager, SAW, &gm->sawSprite, screenWidth, screenHeight);
        }
        manager->spawnTimer = 0.0f;
        manager->nextSpawnTime = randomSpawnTime(); // Set new random spawn time
      }
}

void spawnEntity(EntityManager *manager, EntityType type, Texture *sprite,
    int screenWidth, int screenHeight)
{
    if (manager->count < MAX_ENTITIES)
    {
        Entity entity = {
            .position = {screenWidth, 
                randomNum(screenHeight - ENTITY_SPAWN_HEIGHT_OFFSET)},
            .sprite = sprite,
            .rotation = 0.0f,
            .entityType = type,
        };
        if (type == SAW)
        {
            entity.hitbox = (Hitbox){
                .shape = HITBOX_CIRCLE,
                .circle = { .center = {
                    entity.position.x + SAW_HITBOX_OFFSET,
                    entity.position.y + SAW_HITBOX_OFFSET },
                    .radius = SAW_HITBOX_RADIUS
                }
            };
            entity.scale = SAW_SCALE;
        } else if (type == LIZARD)
        {
            entity.hitbox = (Hitbox){
                .shape = HITBOX_RECT,
                .rect = {entity.position.x+LIZARD_HITBOX_OFFSET, 
                    entity.position.y+LIZARD_HITBOX_OFFSET,
                    LIZARD_HITBOX_X, LIZARD_HITBOX_Y
                }
            };
            entity.scale = LIZARD_SCALE;
        }
        manager->entities[manager->count] = entity;
        manager->count++;
    }
}

void updateEntities(EntityManager *manager, float deltaTime, int screenwidth)
{
    for (int i = 0; i < manager->count; i++)
    {
        Entity *entity = &manager->entities[i];
        if (entity->entityType == SAW)
        {
            entity->position.x -= SAW_SPEED * deltaTime;
            entity->hitbox.circle.center.x -= SAW_SPEED * deltaTime;
        } else if (entity->entityType == LIZARD)
        {
            entity->position.x -= LIZARD_SPEED * deltaTime;
            entity->hitbox.rect.x -= LIZARD_SPEED * deltaTime;
        }
        
        // Remove entities that have moved off-screen
        if (entity->position.x + 100 < 0) // Assuming 100 is approximate entity width
        {
            removeEntity(manager, i);
            i--; // Decrement i to check the same index again
        }
    }
}

CollisionType checkForCollisions(EntityManager *manager, Rectangle* playerHitboxes[])
{
    printf("Checking collisions with %d entities\n", manager->count);
    
    for (int i = 0; i < manager->count; i++)
    {
        if (manager->entities[i].entityType == SAW)
        {
            printf("SAW entity %d at (%.2f, %.2f), circle center (%.2f, %.2f), radius %.2f\n", 
                   i, manager->entities[i].position.x, manager->entities[i].position.y,
                   manager->entities[i].hitbox.circle.center.x, manager->entities[i].hitbox.circle.center.y,
                   manager->entities[i].hitbox.circle.radius);
                   
            for (int j = 0; j < PLAYER_HITBOX_COUNT; j++)
            {          
                if (CheckCollisionCircleRec(manager->entities[i].hitbox.circle.center,
                    manager->entities[i].hitbox.circle.radius, *playerHitboxes[j]))
                {
                    removeEntity(manager, i);
                    return DEATH_COLLISION;
                }
            }
        } else if (manager->entities[i].entityType == LIZARD)
        {
            for (int j = 0; j < PLAYER_HITBOX_COUNT; j++)
            {
                if (CheckCollisionRecs(manager->entities[i].hitbox.rect,
                *playerHitboxes[j]))
                {
                    removeEntity(manager, i);
                    return SCORE_COLLISION;
                }
            }
        }
    }
    return NO_COLLISION;
}

void drawEntities(EntityManager *manager)
{
    for (int i = 0; i < manager->count; i++)
    {
        Entity entity = manager->entities[i];
        DrawTextureEx(*entity.sprite, entity.position, entity.rotation,
            entity.scale, RAYWHITE);
        
        if (manager->entities->entityType == LIZARD)
        {
            drawLizard(manager->entities[i]);
        } else if (manager->entities->entityType == SAW)
        {
            drawSawblade(manager->entities[i]);
        }
    }
}

void drawLizard(Entity entity)
{
    DrawRectangleLinesEx(entity.hitbox.rect, 2, BLUE);
}

void drawSawblade(Entity entity)
{

}

// void drawSaw(Sawblade sawblade)
// {
//   // Calculate the center of the texture for rotation
//   Vector2 origin = { sawblade.sprite.width * 0.1f / 2.0f, sawblade.sprite.height * 0.1f / 2.0f };
  
//   // Source rectangle (entire texture)
//   Rectangle source = { 0, 0, sawblade.sprite.width, sawblade.sprite.height };
  
//   // Destination rectangle (position and scaled size)
//   Rectangle dest = { 
//     sawblade.position.x + origin.x, 
//     sawblade.position.y + origin.y, 
//     sawblade.sprite.width * 0.1f, 
//     sawblade.sprite.height * 0.1f 
//   };
  
//   DrawTexturePro(sawblade.sprite, source, dest, origin, sawblade.rotation, RAYWHITE);
  
//   if (DEBUG_MODE)
//   {
//     DrawCircleLinesV(sawblade.hitbox.center, sawblade.hitbox.radius, PINK);
//   }
// }

void removeEntity(EntityManager *manager, int index)
{
    if (index >= 0 && index < manager->count)
    {
        //shift remaining entities down to fill the gap
        for (int j = index; j < manager->count - 1; j++)
        {
            manager->entities[j] = manager->entities[j + 1];
        }
        manager->count--;
    }
}
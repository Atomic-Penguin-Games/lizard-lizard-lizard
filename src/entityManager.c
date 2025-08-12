#include "entityManager.h"
#include "definitions.h"
#include "randomizer.h"
#include <stdio.h>

EntityManager initEntityManager()
{
    EntityManager manager = {
      .entities = {},
      .count = 0,
      .spawnTimer = OBJECT_SPAWN_RATE_MIN,
      .nextSpawnTime = 0.25f
    };
    return manager;
}

void updateManager(EntityManager *manager, GraphicsManager *gm, float deltaTime)
{
    manager->spawnTimer += deltaTime;
    if (manager->spawnTimer >= manager->nextSpawnTime)
      {
        // Randomly choose between spawning an iguana or sawblade (50/50 chance)
        if (randomNum(2) == 1)
        {
          spawnEntity(manager, LIZARD, &gm->lizardSprite, 
            VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
        }
        else
        {
          spawnEntity(manager, SAW, &gm->sawSprite,
            VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
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

void updateEntities(EntityManager *manager, float deltaTime)
{
    for (int i = 0; i < manager->count; i++)
    {
        Entity *entity = &manager->entities[i];
        if (entity->entityType == SAW)
        {
            entity->position.x -= SAW_SPEED * deltaTime;
            entity->hitbox.circle.center.x -= SAW_SPEED * deltaTime;
            entity->rotation -= SAW_ROTATION_SPEED * deltaTime;
        } else if (entity->entityType == LIZARD)
        {
            entity->position.x -= LIZARD_SPEED * deltaTime;
            entity->hitbox.rect.x -= LIZARD_SPEED * deltaTime;
        }
        
        // Remove entities that have moved off-screen
        if (entity->position.x + entity->hitbox.rect.width*2 < 0)
        {
            removeEntity(manager, i);
            i--; // Decrement i to check the same index again
        }
    }
}

CollisionType checkForCollisions(EntityManager *manager, PlayerHitbox (*hitBoxes)[PLAYER_HITBOX_COUNT])
{   
    for (int i = 0; i < manager->count; i++)
    {
        if (manager->entities[i].entityType == SAW)
        {
            for (int j = 0; j < PLAYER_HITBOX_COUNT; j++)
            {          
                if (!(*hitBoxes)[j].enabled) continue;
                
                if (CheckCollisionCircleRec(manager->entities[i].hitbox.circle.center,
                    manager->entities[i].hitbox.circle.radius, (*hitBoxes)[j].rect))
                {
                    removeEntity(manager, i);
                    return DEATH_COLLISION;
                }
            }
        } else if (manager->entities[i].entityType == LIZARD)
        {
            for (int j = 0; j < PLAYER_HITBOX_COUNT; j++)
            {
                if (!(*hitBoxes)[j].enabled) continue;
                
                if (CheckCollisionRecs(manager->entities[i].hitbox.rect,
                    (*hitBoxes)[j].rect))
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
        Entity *entity = &manager->entities[i];  // Use pointer instead of copy
        
        
        if (entity->entityType == LIZARD)
        {
            drawLizard(entity);  // Pass pointer for efficiency
        } else if (entity->entityType == SAW)
        {
            drawSawblade(entity);
        }
    }
}

void drawLizard(const Entity *entity)
{
    DrawTextureEx(*entity->sprite, entity->position, entity->rotation,
            entity->scale, RAYWHITE);
    if (DEBUG_MODE)
    {
        DrawRectangleLinesEx(entity->hitbox.rect, 2, BLUE);
    }
}

void drawSawblade(const Entity *entity)
{
    Vector2 origin = { entity->sprite->width * SAW_SCALE / 2.0f,
                        entity->sprite->height * SAW_SCALE / 2.0f };
    Rectangle source = { 0, 0, entity->sprite->width, entity->sprite->width };

    Rectangle dest = { 
        entity->position.x + origin.x,
        entity->position.y + origin.y,
        entity->sprite->width * SAW_SCALE,
        entity->sprite->height * SAW_SCALE
    };

    DrawTexturePro(*entity->sprite, source, dest, origin, entity->rotation, RAYWHITE);
    if (DEBUG_MODE)
    {
        DrawCircleV(entity->hitbox.circle.center, entity->hitbox.circle.radius, MAGENTA);
    }
}

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

void clearAllEntities(EntityManager *manager)
{
    // Simply reset the count to 0, effectively clearing all entities
    manager->count = 0;
    
    // Reset spawn timer to start fresh
    manager->spawnTimer = 0.0f;
    manager->nextSpawnTime = OBJECT_SPAWN_RATE_MIN;
}
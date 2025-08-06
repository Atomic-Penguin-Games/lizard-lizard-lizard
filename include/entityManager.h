#pragma once
#include "raylib.h"
#include "definitions.h"

typedef enum
{
    SAW,
    LIZARD
} EntityType;

typedef enum 
{
    HITBOX_RECT,
    HITBOX_CIRCLE
} HitboxShape;

typedef struct Hitbox 
{
    HitboxShape shape;
    union
    {
        Rectangle rect;
        struct circle
        {
            Vector2 center;
            float radius;
        } circle;
        
    };
} Hitbox;

typedef struct Entity
{
    EntityType entityType;
    Vector2 position;
    Texture sprite;
    Hitbox hitbox;
    Vector2 hitboxOffset;
    float rotation;
    float scale;
} Entity;

typedef struct EntityManager
{
    Entity entities[MAX_ENTITIES];
    int count;
    float spawnTimer;
} EntityManager;

void spawnEntity(EntityManager *manager, EntityType type, Texture sprite,
int screenWidth, int screenHeight);
void updateEntities(EntityManager *manager, float deltaTime, int screenwidth);
void drawEntities(EntityManager *manager);
void removeEntity(EntityManager *manager, int index);
void drawSawblade(Entity entity);
void drawLizard(Entity entity);
#pragma once
#include <raylib.h>

typedef struct GraphicsManager
{
    Texture playerSpritesheet;
    Texture lizardSpritesheet;
    Texture sawSpritesheet;
} GraphicsManager;

initGraphicsManager();
Texture* getPlayerSprite();
Texture* getLizardSprite();
Texture* getSawSprite();
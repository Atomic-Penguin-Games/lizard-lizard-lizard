#pragma once
#include <raylib.h>

typedef struct GraphicsManager
{
    Texture playerSpritesheet;
    Texture lizardSprite;
    Texture sawSprite;
} GraphicsManager;

GraphicsManager initGraphicsManager();

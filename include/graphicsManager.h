#pragma once
#include <raylib.h>

typedef struct GraphicsManager
{
    Texture playerSpritesheet;
    Texture lizardSprite;
    Texture sawSprite;
    Texture cursorGraphic;
    Texture pauseButtonGraphic;
    Shader flashShader;
    int flashIntensityLoc;
    int flashColorLoc;
} GraphicsManager;

GraphicsManager initGraphicsManager();

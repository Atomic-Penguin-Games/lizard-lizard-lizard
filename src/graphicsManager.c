#include "graphicsManager.h"

GraphicsManager initGraphicsManager()
{
    //Texture sprite = LoadTexture("res/animationSpritesheet.png");
    //Texture iguanaSprite = LoadTexture("res/lizardEmoji.png");
    //Texture sawSprite = LoadTexture("res/circular_saw_blade.png");
    
    // Load flash shader
    Shader flashShader = LoadShader("res/shaders/flash.vs", "res/shaders/flash.fs");
    int flashIntensityLoc = GetShaderLocation(flashShader, "flashIntensity");
    int flashColorLoc = GetShaderLocation(flashShader, "flashColor");
    
    GraphicsManager graphicsManager = {
        .playerSpritesheet = LoadTexture("res/playerAnimationSpritesheet.png"),
        .lizardSprite = LoadTexture("res/lizardEmoji.png"),
        .sawSprite = LoadTexture("res/circular_saw_blade.png"),
        .cursorGraphic = LoadTexture("res/mouse_sprite_4.png"),
        .pauseButtonGraphic = LoadTexture("res/pause.png"),
        .flashShader = flashShader,
        .flashIntensityLoc = flashIntensityLoc,
        .flashColorLoc = flashColorLoc
    };
    
    return graphicsManager;
}
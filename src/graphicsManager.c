#include "graphicsManager.h"

GraphicsManager initGraphicsManager()
{
    //Texture sprite = LoadTexture("res/animationSpritesheet.png");
    //Texture iguanaSprite = LoadTexture("res/lizardEmoji.png");
    //Texture sawSprite = LoadTexture("res/circular_saw_blade.png");
    
    // Load flash shader with error checking
    Shader flashShader = LoadShader("res/shaders/flash.vs", "res/shaders/flash.fs");
    int flashIntensityLoc = -1;
    int flashColorLoc = -1;
    
    // Check if shader loaded successfully
    if (flashShader.id > 0) {
        flashIntensityLoc = GetShaderLocation(flashShader, "flashIntensity");
        flashColorLoc = GetShaderLocation(flashShader, "flashColor");
    } else {
        // Fallback: create an empty shader struct for web builds
        flashShader = (Shader){0};
    }
    
    GraphicsManager graphicsManager = {
        .playerSpritesheet = LoadTexture("res/playerAnimationSpritesheet.png"),
        .lizardSprite = LoadTexture("res/lizardEmoji.png"),
        .sawSprite = LoadTexture("res/circular_saw_blade.png"),
        .cursorGraphic = LoadTexture("res/mouse_sprite.png"),
        .pauseButtonGraphic = LoadTexture("res/pause.png"),
        .flashShader = flashShader,
        .flashIntensityLoc = flashIntensityLoc,
        .flashColorLoc = flashColorLoc
    };
    
    return graphicsManager;
}
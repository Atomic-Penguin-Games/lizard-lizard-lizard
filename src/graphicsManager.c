#include "graphicsManager.h"

GraphicsManager initGraphicsManager()
{
    //Texture sprite = LoadTexture("res/animationSpritesheet.png");
    //Texture iguanaSprite = LoadTexture("res/lizardEmoji.png");
    //Texture sawSprite = LoadTexture("res/circular_saw_blade.png");
    GraphicsManager graphicsManager = {
        .playerSpritesheet = LoadTexture("res/playerAnimationSpritesheet.png"),
        .lizardSprite = LoadTexture("res/lizardEmoji.png"),
        .sawSprite = LoadTexture("res/circular_saw_blade.png"),
        .cursorGraphic = LoadTexture("res/mouse_sprite_4.png"),
        .pauseButtonGraphic = LoadTexture("res/pause.png")
    };
    
    return graphicsManager;
}
#include "graphicsManager.h"
#include "player_spritesheet.h"
#include "lizard_sprite.h"
#include "saw_sprite.h"
#include "mouse_sprite.h"
#include "pause_sprite.h"
#include "shader_flash_vs.h"
#include "shader_flash_fs.h"

GraphicsManager initGraphicsManager()
{
    Image img = LoadImageFromMemory(".png", player_spritesheet, player_spritesheet_length);
    Texture playerSprite = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", lizard_sprite, lizard_sprite_length);
    Texture lizardSprite = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", saw_sprite, saw_sprite_length);
    Texture sawSprite = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", mouse_sprite, mouse_sprite_length);
    Texture mouseSprite = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", pause_sprite, pause_sprite_length);
    Texture pauseSprite = LoadTextureFromImage(img);
    UnloadImage(img);

    Shader flashShader = LoadShaderFromMemory(shader_flash_vs, shader_flash_fs);
    //Shader flashShader = LoadShader("res/shaders/flash.vs", "res/shaders/flash.fs");
    int flashIntensityLoc = -1;
    int flashColorLoc = -1;

    // Shader loader error checking
    if (flashShader.id > 0) {
        flashIntensityLoc = GetShaderLocation(flashShader, "flashIntensity");
        flashColorLoc = GetShaderLocation(flashShader, "flashColor");
    } else {
        // Fallback: create an empty shader struct for web builds
        flashShader = (Shader){0};
    }
    
    GraphicsManager graphicsManager = {
        .playerSpritesheet = playerSprite,
        .lizardSprite = lizardSprite,
        .sawSprite = sawSprite,
        .cursorGraphic = mouseSprite,
        .pauseButtonGraphic = pauseSprite,
        .flashShader = flashShader,
        .flashIntensityLoc = flashIntensityLoc,
        .flashColorLoc = flashColorLoc
    };
    
    return graphicsManager;
}
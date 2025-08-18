#pragma once
#include <raylib.h>

typedef struct SoundManager {
    Sound scoreSounds[3];
} SoundManager;

SoundManager initSoundManager();
void initSoundDevice();
void loadSounds(SoundManager *sm);
void playRandomScoreSound(SoundManager *sm);
void playDefaultScoreSound(SoundManager *sm);

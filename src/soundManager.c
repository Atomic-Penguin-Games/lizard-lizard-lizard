#include "soundManager.h"
#include "randomizer.h"

SoundManager initSoundManager()
{
    initSoundDevice();
    SoundManager sm = {};
    loadSounds(&sm);
    return sm;
}

void initSoundDevice()
{
    InitAudioDevice();
}

void loadSounds(SoundManager *soundManager)
{
    Sound scoreSound = LoadSound("res/lizard.wav");
    Sound scoreSound2 = LoadSound("res/lizardUpShift.wav");
    Sound scoreSound3 = LoadSound("res/lizardDownShift.wav");

    soundManager->scoreSounds[0] = scoreSound;
    soundManager->scoreSounds[1] = scoreSound2;
    soundManager->scoreSounds[2] = scoreSound3;
}

void playScoreSound(SoundManager *sm)
{
    PlaySound(sm->scoreSounds[randomNum(3)-1]);
}

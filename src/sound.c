#include "sound.h"


Sound scoreSounds[3];


void initSoundSystem()
{
    initSoundDevice();
    loadSounds();
}

void initSoundDevice()
{
    InitAudioDevice();
}

void loadSounds()
{
    Sound scoreSound = LoadSound("res/lizard.wav");
    Sound scoreSound2 = LoadSound("res/lizardUpShift.wav");
    Sound scoreSound3 = LoadSound("res/lizardDownShift.wav");

    scoreSounds[0] = scoreSound;
    scoreSounds[1] = scoreSound2;
    scoreSounds[2] = scoreSound3;
}

void playScoreSound()
{
    PlaySound(scoreSounds[randomNum(3)-1]);
}
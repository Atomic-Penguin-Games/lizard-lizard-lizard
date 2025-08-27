#include "soundManager.h"
#include "randomizer.h"
#include "lizard_sound.h"
#include "lizard_sound_up_shift.h"
#include "lizard_sound_down_shift.h"

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
    Wave scoreWave = LoadWaveFromMemory(".wav", lizard_sound, lizard_sound_length);
    Sound scoreSound = LoadSoundFromWave(scoreWave);
    UnloadWave(scoreWave);

    scoreWave = LoadWaveFromMemory(".wav", lizard_sound_up_shift, 
        lizard_sound_up_shift_length);
    Sound scoreSound2 = LoadSoundFromWave(scoreWave);
    UnloadWave(scoreWave);

    scoreWave = LoadWaveFromMemory(".wav", lizard_sound_down_shift, 
        lizard_sound_down_shift_length);
    Sound scoreSound3 = LoadSoundFromWave(scoreWave);
    UnloadWave(scoreWave);

    soundManager->scoreSounds[0] = scoreSound;
    soundManager->scoreSounds[1] = scoreSound2;
    soundManager->scoreSounds[2] = scoreSound3;
}

void playRandomScoreSound(SoundManager *sm)
{
    PlaySound(sm->scoreSounds[randomNum(3)-1]);
}

void playDefaultScoreSound(SoundManager *sm)
{
    PlaySound(sm->scoreSounds[0]);
}

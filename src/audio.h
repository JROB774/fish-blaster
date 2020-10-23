#ifndef AUDIO_H
#define AUDIO_H

typedef enum SoundID__
{
    SND_NSHOT_0,
    SND_NSHOT_1,
    SND_NSHOT_2,
    SND_SQUEAK_0,
    SND_SQUEAK_1,
    SND_SQUEAK_2,
    SND_TING_0,
    SND_TING_1,
    SND_TING_2,
    SND_CODE,
    SND_SMACK,
    SND_BREAK,
    SND_ITEM,
    SND_TOTAL

} SoundID;

typedef enum MusicID__
{
    MUS_TOTAL

} MusicID;

INTERNAL void init_audio ();
INTERNAL void quit_audio ();

INTERNAL void play_sound (SoundID id, int loops);
INTERNAL void play_music (MusicID id, int loops);

// Audio volume should always be set with these functions rather than setting
// the gAudio.sound_volume and gAudio.music_volume values directly. As these
// functions performs some extra operations in order for the volume to change.
INTERNAL void set_sound_volume (float volume); // [0-1]
INTERNAL void set_music_volume (float volume); // [0-1]

// SOUND GROUPS

GLOBAL const SoundID SND_NSHOT [] = { SND_NSHOT_0,  SND_NSHOT_1,  SND_NSHOT_2  };
GLOBAL const SoundID SND_SQUEAK[] = { SND_SQUEAK_0, SND_SQUEAK_1, SND_SQUEAK_2 };
GLOBAL const SoundID SND_TING  [] = { SND_TING_0,   SND_TING_1,   SND_TING_2   };

#endif /* AUDIO_H */

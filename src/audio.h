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
    SND_BOOM,
    SND_SWISH,
    SND_RSHOT_0,
    SND_RSHOT_1,
    SND_RSHOT_2,
    SND_SSHOT_0,
    SND_SSHOT_1,
    SND_SSHOT_2,
    SND_ZAP,
    SND_TOTAL

} SoundID;

typedef enum MusicID__
{
    MUS_TRACK_0,
    MUS_TRACK_1,
    MUS_TRACK_2,
    MUS_TOTAL

} MusicID;

INTERNAL void init_audio ();
INTERNAL void quit_audio ();

INTERNAL int  play_sound         (SoundID id, int loops);
INTERNAL int  play_sound_channel (SoundID id, int loops, int channel);
INTERNAL void play_music         (MusicID id, int loops);
INTERNAL void stop_channel       (int channel);

// Audio volume should always be set with these functions rather than setting
// the gAudio.sound_volume and gAudio.music_volume values directly. As these
// functions performs some extra operations in order for the volume to change.
INTERNAL void set_sound_volume (float volume); // [0-1]
INTERNAL void set_music_volume (float volume); // [0-1]

// SOUND GROUPS

GLOBAL const SoundID SND_NSHOT [] = { SND_NSHOT_0,  SND_NSHOT_1,  SND_NSHOT_2  };
GLOBAL const SoundID SND_SQUEAK[] = { SND_SQUEAK_0, SND_SQUEAK_1, SND_SQUEAK_2 };
GLOBAL const SoundID SND_TING  [] = { SND_TING_0,   SND_TING_1,   SND_TING_2   };
GLOBAL const SoundID SND_RSHOT [] = { SND_RSHOT_0,  SND_RSHOT_1,  SND_RSHOT_2  };
GLOBAL const SoundID SND_SSHOT [] = { SND_SSHOT_0,  SND_SSHOT_1,  SND_SSHOT_2  };

// MUSIC GROUPS

GLOBAL const MusicID MUS_TRACK [] = { MUS_TRACK_0,  MUS_TRACK_1,  MUS_TRACK_2  };

#endif /* AUDIO_H */

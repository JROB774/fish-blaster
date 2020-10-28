////////////////////////////////////////////////////////////////////////////////

// The stuff in this section is all used internally by the audio/sound system!

#define AUDIO_FREQUENCY     MIX_DEFAULT_FREQUENCY
#define AUDIO_SAMPLE_FORMAT MIX_DEFAULT_FORMAT
#define AUDIO_CHANNELS      2 // Stereo Sound
#define AUDIO_SAMPLE_SIZE   2048

typedef struct Sound__ { Mix_Chunk* data; } Sound;
typedef struct Music__ { Mix_Music* data; } Music;

GLOBAL struct
{
    Sound sound[SND_TOTAL];
    Music music[MUS_TOTAL];

    bool initialized;

    float sound_volume;
    float music_volume;

} gAudio;

INTERNAL void load_sound (Sound* sound, const char* file_name)
{
    assert(sound);
    if (gAudio.initialized)
    {
        sound->data = Mix_LoadWAV(file_name);
        if (!sound->data)
        {
            LOGDEBUG("Failed to load sound \"%s\"! (%s)", file_name, Mix_GetError());
        }
    }
}

INTERNAL void free_sound (Sound* sound)
{
    assert(sound);
    Mix_FreeChunk(sound->data);
    sound->data = NULL;
}

INTERNAL void load_music (Music* music, const char* file_name)
{
    assert(music);
    if (gAudio.initialized)
    {
        music->data = Mix_LoadMUS(file_name);
        if (!music->data)
        {
            LOGDEBUG("Failed to load music \"%s\"! (%s)", file_name, Mix_GetError());
        }
    }
}

INTERNAL void free_music (Music* music)
{
    assert(music);
    Mix_FreeMusic(music->data);
    music->data = NULL;
}

////////////////////////////////////////////////////////////////////////////////

INTERNAL void init_audio ()
{
    if (!(Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG))
    {
        LOGWARNING("Failed to initialize mixer OGG functionality! (%s)", Mix_GetError());
    }

    if (Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_SAMPLE_FORMAT, AUDIO_CHANNELS, AUDIO_SAMPLE_SIZE) != 0)
    {
        LOGWARNING("Failed to open audio device! (%s)", Mix_GetError());
    }
    else
    {
        Mix_AllocateChannels(32);

        set_sound_volume(CAST(float,gSettings.sound_volume)/CAST(float,MIX_MAX_VOLUME));
        set_music_volume(CAST(float,gSettings.music_volume)/CAST(float,MIX_MAX_VOLUME));

        gAudio.initialized = true;

        // Load all of the sounds.
        load_sound(&gAudio.sound[SND_NSHOT_0 ], "assets/nshot0.wav" );
        load_sound(&gAudio.sound[SND_NSHOT_1 ], "assets/nshot1.wav" );
        load_sound(&gAudio.sound[SND_NSHOT_2 ], "assets/nshot2.wav" );
        load_sound(&gAudio.sound[SND_SQUEAK_0], "assets/squeak0.wav");
        load_sound(&gAudio.sound[SND_SQUEAK_1], "assets/squeak1.wav");
        load_sound(&gAudio.sound[SND_SQUEAK_2], "assets/squeak2.wav");
        load_sound(&gAudio.sound[SND_TING_0  ], "assets/ting0.wav"  );
        load_sound(&gAudio.sound[SND_TING_1  ], "assets/ting1.wav"  );
        load_sound(&gAudio.sound[SND_TING_2  ], "assets/ting2.wav"  );
        load_sound(&gAudio.sound[SND_CODE    ], "assets/code.wav"   );
        load_sound(&gAudio.sound[SND_SMACK   ], "assets/smack.wav"  );
        load_sound(&gAudio.sound[SND_BREAK   ], "assets/break.wav"  );
        load_sound(&gAudio.sound[SND_ITEM    ], "assets/item.wav"   );
        load_sound(&gAudio.sound[SND_BOOM    ], "assets/boom.wav"   );
        load_sound(&gAudio.sound[SND_SWISH   ], "assets/swish.wav"  );
        load_sound(&gAudio.sound[SND_RSHOT_0 ], "assets/rshot0.wav" );
        load_sound(&gAudio.sound[SND_RSHOT_1 ], "assets/rshot1.wav" );
        load_sound(&gAudio.sound[SND_RSHOT_2 ], "assets/rshot2.wav" );
        load_sound(&gAudio.sound[SND_SSHOT_0 ], "assets/sshot0.wav" );
        load_sound(&gAudio.sound[SND_SSHOT_1 ], "assets/sshot1.wav" );
        load_sound(&gAudio.sound[SND_SSHOT_2 ], "assets/sshot2.wav" );
        load_sound(&gAudio.sound[SND_ZAP     ], "assets/zap.wav"    );
        // Load all of the music.
        load_music(&gAudio.music[MUS_TRACK_0 ], "assets/track0.ogg" );
        load_music(&gAudio.music[MUS_TRACK_1 ], "assets/track1.ogg" );
        load_music(&gAudio.music[MUS_TRACK_2 ], "assets/track2.ogg" );
    }
}

INTERNAL void quit_audio ()
{
    if (gAudio.initialized)
    {
        // Free all of the sounds.
        free_sound(&gAudio.sound[SND_NSHOT_0 ]);
        free_sound(&gAudio.sound[SND_NSHOT_1 ]);
        free_sound(&gAudio.sound[SND_NSHOT_2 ]);
        free_sound(&gAudio.sound[SND_SQUEAK_0]);
        free_sound(&gAudio.sound[SND_SQUEAK_1]);
        free_sound(&gAudio.sound[SND_SQUEAK_2]);
        free_sound(&gAudio.sound[SND_TING_0  ]);
        free_sound(&gAudio.sound[SND_TING_1  ]);
        free_sound(&gAudio.sound[SND_TING_2  ]);
        free_sound(&gAudio.sound[SND_CODE    ]);
        free_sound(&gAudio.sound[SND_SMACK   ]);
        free_sound(&gAudio.sound[SND_BREAK   ]);
        free_sound(&gAudio.sound[SND_ITEM    ]);
        free_sound(&gAudio.sound[SND_BOOM    ]);
        free_sound(&gAudio.sound[SND_SWISH   ]);
        free_sound(&gAudio.sound[SND_RSHOT_0 ]);
        free_sound(&gAudio.sound[SND_RSHOT_1 ]);
        free_sound(&gAudio.sound[SND_RSHOT_2 ]);
        free_sound(&gAudio.sound[SND_SSHOT_0 ]);
        free_sound(&gAudio.sound[SND_SSHOT_1 ]);
        free_sound(&gAudio.sound[SND_SSHOT_2 ]);
        free_sound(&gAudio.sound[SND_ZAP     ]);
        // Free all of the music.
        free_music(&gAudio.music[MUS_TRACK_0 ]);
        free_music(&gAudio.music[MUS_TRACK_1 ]);
        free_music(&gAudio.music[MUS_TRACK_2 ]);

        Mix_CloseAudio();
    }

    Mix_Quit();
}

INTERNAL int play_sound (SoundID id, int loops)
{
    return play_sound_channel(id,loops,-1);
}
INTERNAL int play_sound_channel (SoundID id, int loops, int channel)
{
    assert((id >= 0) && (id < SND_TOTAL));
    int returned_channel = 0;
    if (gAudio.initialized)
    {
        returned_channel = Mix_PlayChannel(channel, gAudio.sound[id].data, loops);
        if (returned_channel == -1)
        {
            LOGDEBUG("Failed to play sound! (%s)", Mix_GetError());
        }
    }
    return returned_channel;
}

INTERNAL void play_music (MusicID id, int loops)
{
    assert((id >= 0) && (id < MUS_TOTAL));
    if (gAudio.initialized)
    {
        if (Mix_PlayMusic(gAudio.music[id].data, loops) == -1)
        {
            LOGDEBUG("Failed to play music! (%s)", Mix_GetError());
        }
    }
}

INTERNAL void stop_channel (int channel)
{
    Mix_HaltChannel(channel);
}

INTERNAL void set_sound_volume (float volume)
{
    gAudio.sound_volume = CLAMP(volume, 0.0f, 1.0f);
    int ivolume = CAST(int, CAST(float,MIX_MAX_VOLUME)*gAudio.sound_volume);
    Mix_Volume(-1, ivolume);
}
INTERNAL void set_music_volume (float volume)
{
    gAudio.music_volume = CLAMP(volume, 0.0f, 1.0f);
    int ivolume = CAST(int, CAST(float,MIX_MAX_VOLUME)*gAudio.music_volume);
    Mix_VolumeMusic(ivolume);
}

INTERNAL float get_sound_volume ()
{
    return gAudio.sound_volume;
}
INTERNAL float get_music_volume ()
{
    return gAudio.music_volume;
}

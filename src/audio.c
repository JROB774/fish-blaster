////////////////////////////////////////////////////////////////////////////////

// The stuff in this section is all used internally by the audio/sound system!

#define AUDIO_FREQUENCY      MIX_DEFAULT_FREQUENCY
#define AUDIO_SAMPLE_FORMAT  MIX_DEFAULT_FORMAT
#define AUDIO_CHANNELS       2 // Stereo Sound
#define AUDIO_SAMPLE_SIZE    2048
#define AUDIO_SOUND_CHANNELS 64

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
    // Doesn't seem like it's necessary in web builds as the OGG files play anyway even
    // though this function fails when ran on the web? So I guess we'll just disable it.
    #if defined(PLATFORM_WIN32)
    if (!(Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG))
    {
        LOGWARNING("Failed to initialize mixer OGG functionality! (%s)", Mix_GetError());
    }
    #endif

    if (Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_SAMPLE_FORMAT, AUDIO_CHANNELS, AUDIO_SAMPLE_SIZE) != 0)
    {
        LOGWARNING("Failed to open audio device! (%s)", Mix_GetError());
    }
    else
    {
        Mix_AllocateChannels(AUDIO_SOUND_CHANNELS);

        set_sound_volume(CAST(float,gSettings.sound_volume)/CAST(float,MIX_MAX_VOLUME));
        set_music_volume(CAST(float,gSettings.music_volume)/CAST(float,MIX_MAX_VOLUME));

        gAudio.initialized = true;

        // Load all of the sounds.
        load_sound(&gAudio.sound[SND_NSHOT_0 ], ASSET_NSHOT0 );
        load_sound(&gAudio.sound[SND_NSHOT_1 ], ASSET_NSHOT1 );
        load_sound(&gAudio.sound[SND_NSHOT_2 ], ASSET_NSHOT2 );
        load_sound(&gAudio.sound[SND_SQUEAK_0], ASSET_SQUEAK0);
        load_sound(&gAudio.sound[SND_SQUEAK_1], ASSET_SQUEAK1);
        load_sound(&gAudio.sound[SND_SQUEAK_2], ASSET_SQUEAK2);
        load_sound(&gAudio.sound[SND_TING_0  ], ASSET_TING0  );
        load_sound(&gAudio.sound[SND_TING_1  ], ASSET_TING1  );
        load_sound(&gAudio.sound[SND_TING_2  ], ASSET_TING2  );
        load_sound(&gAudio.sound[SND_CODE    ], ASSET_CODE   );
        load_sound(&gAudio.sound[SND_SMACK   ], ASSET_SMACK  );
        load_sound(&gAudio.sound[SND_BREAK   ], ASSET_BREAK  );
        load_sound(&gAudio.sound[SND_ITEM    ], ASSET_ITEM   );
        load_sound(&gAudio.sound[SND_BOOM    ], ASSET_BOOM   );
        load_sound(&gAudio.sound[SND_SWISH   ], ASSET_SWISH  );
        load_sound(&gAudio.sound[SND_RSHOT_0 ], ASSET_RSHOT0 );
        load_sound(&gAudio.sound[SND_RSHOT_1 ], ASSET_RSHOT1 );
        load_sound(&gAudio.sound[SND_RSHOT_2 ], ASSET_RSHOT2 );
        load_sound(&gAudio.sound[SND_SSHOT_0 ], ASSET_SSHOT0 );
        load_sound(&gAudio.sound[SND_SSHOT_1 ], ASSET_SSHOT1 );
        load_sound(&gAudio.sound[SND_SSHOT_2 ], ASSET_SSHOT2 );
        load_sound(&gAudio.sound[SND_ZAP     ], ASSET_ZAP    );
        // Load all of the music.
        load_music(&gAudio.music[MUS_TRACK_0 ], ASSET_TRACK0 );
        load_music(&gAudio.music[MUS_TRACK_1 ], ASSET_TRACK1 );
        load_music(&gAudio.music[MUS_TRACK_2 ], ASSET_TRACK2 );
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
    volume = CLAMP(volume, 0.0f, 1.0f);
    gAudio.sound_volume = CLAMP(volume, 0.0f, 1.0f);
    int ivolume = CAST(int, CAST(float,MIX_MAX_VOLUME)*gAudio.sound_volume);
    Mix_Volume(-1, ivolume);
}
INTERNAL void set_music_volume (float volume)
{
    volume = CLAMP(volume, 0.0f, 1.0f);
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

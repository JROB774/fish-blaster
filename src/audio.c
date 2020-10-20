#define AUDIO_FREQUENCY     MIX_DEFAULT_FREQUENCY
#define AUDIO_SAMPLE_FORMAT MIX_DEFAULT_FORMAT
#define AUDIO_CHANNELS      2 // Stereo Sound
#define AUDIO_SAMPLE_SIZE   2048

INTERNAL void init_audio ()
{
    if (Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_SAMPLE_FORMAT, AUDIO_CHANNELS, AUDIO_SAMPLE_SIZE) != 0)
    {
        LOGWARNING("Failed to open audio device! (%s)", Mix_GetError());
    }
    else
    {
        Mix_AllocateChannels(32);

        // @Incomplete: Load these values from a settings file...
        set_sound_volume(0.5f);
        set_music_volume(0.5f);
    }
}

INTERNAL void quit_audio ()
{
    Mix_CloseAudio();
}

INTERNAL void load_sound (Sound* sound, const char* file_name)
{
    assert(sound);
    sound->data = Mix_LoadWAV(file_name);
    if (!sound->data)
    {
        LOGDEBUG("Failed to load sound \"%s\"! (%s)", file_name, Mix_GetError());
    }
}

INTERNAL void free_sound (Sound* sound)
{
    assert(sound);
    Mix_FreeChunk(sound->data);
    sound->data = NULL;
}

INTERNAL void play_sound (Sound* sound, int loops)
{
    assert(sound);
    if (Mix_PlayChannel(-1, sound->data, loops) == -1)
    {
        LOGDEBUG("Failed to play sound! (%s)", Mix_GetError());
    }
}

INTERNAL void load_music (Music* music, const char* file_name)
{
    assert(music);
    music->data = Mix_LoadMUS(file_name);
    if (!music->data)
    {
        LOGDEBUG("Failed to load music \"%s\"! (%s)", file_name, Mix_GetError());
    }
}

INTERNAL void free_music (Music* music)
{
    assert(music);
    Mix_FreeMusic(music->data);
    music->data = NULL;
}

INTERNAL void play_music (Music* music, int loops)
{
    assert(music);
    if (Mix_PlayMusic(music->data, loops) == -1)
    {
        LOGDEBUG("Failed to play music! (%s)", Mix_GetError());
    }
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

#define SETTINGS_DEFAULT_SOUND_VOLUME  (MIX_MAX_VOLUME / 2)
#define SETTINGS_DEFAULT_MUSIC_VOLUME  (MIX_MAX_VOLUME / 2)
#define SETTINGS_DEFAULT_CURSOR_TYPE   CUR_0
#define SETTINGS_DEFAULT_MUSIC_TRACK   MUS_TRACK_0
#define SETTINGS_DEFAULT_FULLSCREEN    false

#if defined(PLATFORM_WIN32)
#define SETTINGS_DEFAULT_WINDOW_WIDTH  (SCREEN_W * 3)
#define SETTINGS_DEFAULT_WINDOW_HEIGHT (SCREEN_H * 3)
#elif defined(PLATFORM_WEB)
#define SETTINGS_DEFAULT_WINDOW_WIDTH  (SCREEN_W * 3)+32
#define SETTINGS_DEFAULT_WINDOW_HEIGHT (SCREEN_H * 3)+32
#endif

#define SETTINGS_FILE "settings.dat"

INTERNAL void load_settings ()
{
    // Set the settings to their default values in case they can't be loaded.
    gSettings.sound_volume  = SETTINGS_DEFAULT_SOUND_VOLUME;
    gSettings.music_volume  = SETTINGS_DEFAULT_MUSIC_VOLUME;
    gSettings.cursor_type   = SETTINGS_DEFAULT_CURSOR_TYPE;
    gSettings.music_track   = SETTINGS_DEFAULT_MUSIC_TRACK;
    gSettings.window_width  = SETTINGS_DEFAULT_WINDOW_WIDTH;
    gSettings.window_height = SETTINGS_DEFAULT_WINDOW_HEIGHT;
    gSettings.fullscreen    = SETTINGS_DEFAULT_FULLSCREEN;

    // If the file is NULL there was either a problem or no settings have been saved!
    // Either way it means we won't be loading settings and we fallback to defaults.
    FILE* file = fopen(SETTINGS_FILE, "rb");
    if (file)
    {
        fread(&gSettings,sizeof(gSettings),1,file);
        fclose(file);
    }
}

INTERNAL void save_settings ()
{
    gSettings.sound_volume  = CAST(U32,CAST(float,MIX_MAX_VOLUME)*get_sound_volume());
    gSettings.music_volume  = CAST(U32,CAST(float,MIX_MAX_VOLUME)*get_music_volume());
    gSettings.cursor_type   = CAST(U32,gPlayer.current_cursor);
    gSettings.music_track   = CAST(U32,gApp.current_track);
    gSettings.window_width  = CAST(U32,get_window_cached_w ());
    gSettings.window_height = CAST(U32,get_window_cached_h ());
    gSettings.fullscreen    = CAST(U32,is_fullscreen());

    FILE* file = fopen(SETTINGS_FILE, "wb");
    if (!file)
    {
        LOGWARNING("Failed to save settings!");
    }
    else
    {
        fwrite(&gSettings,sizeof(gSettings),1,file);
        fclose(file);
    }
}

INTERNAL void reset_settings ()
{
    set_sound_volume(CAST(float,SETTINGS_DEFAULT_SOUND_VOLUME)/CAST(float,MIX_MAX_VOLUME));
    set_music_volume(CAST(float,SETTINGS_DEFAULT_MUSIC_VOLUME)/CAST(float,MIX_MAX_VOLUME));
    set_player_cursor_type(SETTINGS_DEFAULT_CURSOR_TYPE);
    set_music_track(SETTINGS_DEFAULT_MUSIC_TRACK);
    set_window_size(SETTINGS_DEFAULT_WINDOW_WIDTH, SETTINGS_DEFAULT_WINDOW_HEIGHT);
    set_fullscreen(SETTINGS_DEFAULT_FULLSCREEN);
}

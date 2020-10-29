#ifndef SETTINGS_H
#define SETTINGS_H

GLOBAL struct
{
    U32 sound_volume;
    U32 music_volume;
    U32 cursor_type;
    U32 music_track;
    U32 window_width;
    U32 window_height;
    U32 fullscreen;

} gSettings;

INTERNAL void  load_settings ();
INTERNAL void  save_settings ();
INTERNAL void reset_settings ();

#endif /* SETTINGS_H */

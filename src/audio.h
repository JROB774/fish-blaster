#ifndef AUDIO_H
#define AUDIO_H

typedef struct Sound__ { Mix_Chunk* data; } Sound;
typedef struct Music__ { Mix_Music* data; } Music;

GLOBAL struct
{
    float sound_volume;
    float music_volume;
    bool  initialized;

} gAudio;

INTERNAL void init_audio ();
INTERNAL void quit_audio ();

INTERNAL void load_sound (Sound* sound, const char* file_name);
INTERNAL void free_sound (Sound* sound);
INTERNAL void play_sound (Sound* sound, int loops);

INTERNAL void load_music (Music* music, const char* file_name);
INTERNAL void free_music (Music* music);
INTERNAL void play_music (Music* music, int loops);

// Audio volume should always be set with these functions rather than setting
// the gAudio.sound_volume and gAudio.music_volume values directly. As these
// functions performs some extra operations in order for the volume to change.
INTERNAL void set_sound_volume (float volume); // [0-1]
INTERNAL void set_music_volume (float volume); // [0-1]

#endif /* AUDIO_H */

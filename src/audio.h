#ifndef AUDIO_H
#define AUDIO_H

typedef enum SoundID__ { SND_SHOOT, SND_HIT, SND_TOTAL } SoundID;
typedef enum MusicID__ { MUS_TOTAL                     } MusicID;

INTERNAL void init_audio ();
INTERNAL void quit_audio ();

INTERNAL void play_sound (SoundID sound, int loops);
INTERNAL void play_music (MusicID music, int loops);

// Audio volume should always be set with these functions rather than setting
// the gAudio.sound_volume and gAudio.music_volume values directly. As these
// functions performs some extra operations in order for the volume to change.
INTERNAL void set_sound_volume (float volume); // [0-1]
INTERNAL void set_music_volume (float volume); // [0-1]

#endif /* AUDIO_H */

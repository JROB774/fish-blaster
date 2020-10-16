#ifndef FTIMER_H
#define FTIMER_H

#define FRAMERATE 60

GLOBAL struct FrameTimer
{
    U64 performance_frequency;
    U64 last_counter;
    U64 end_counter;

    float current_fps;
    float fixed_time_step;
    float delta_time;

} gFrameTimer;

INTERNAL void init_frame_timer ();
INTERNAL void cap_framerate    ();

#endif /* FTIMER_H */

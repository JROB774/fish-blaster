INTERNAL float counter_to_seconds (U64 counter)
{
    return (CAST(float, counter) / CAST(float, gFrameTimer.performance_frequency));
}

INTERNAL void init_frame_timer ()
{
    gFrameTimer.performance_frequency = SDL_GetPerformanceFrequency();
    gFrameTimer.last_counter          = SDL_GetPerformanceCounter();
    gFrameTimer.end_counter           = 0;
    gFrameTimer.current_fps           = 0.0f;
    gFrameTimer.fixed_time_step       = 1.0f / FRAMERATE;
    gFrameTimer.delta_time            = gFrameTimer.fixed_time_step;
}

INTERNAL void cap_framerate ()
{
    gFrameTimer.end_counter = SDL_GetPerformanceCounter();
    U64 elapsed_counter = gFrameTimer.end_counter - gFrameTimer.last_counter;

    float elapsed = counter_to_seconds(elapsed_counter);
    while (elapsed < gFrameTimer.fixed_time_step)
    {
        float time_left = gFrameTimer.fixed_time_step - elapsed;
        U32 sleep = CAST(U32, 1000.0f * time_left);
        if (sleep > 0) SDL_Delay(sleep);

        elapsed_counter = SDL_GetPerformanceCounter() - gFrameTimer.last_counter;
        elapsed = counter_to_seconds(elapsed_counter);
    }

    gFrameTimer.current_fps = CAST(float, gFrameTimer.performance_frequency) / CAST(float, elapsed_counter);
    gFrameTimer.last_counter = SDL_GetPerformanceCounter();
}

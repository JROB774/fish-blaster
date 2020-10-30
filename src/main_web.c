#include "main_web.h"

void main_loop ()
{
    update_input_state();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        handle_application(&event);
        switch (event.type)
        {
            case (SDL_KEYDOWN):
            {
                switch (event.key.keysym.sym)
                {
                    case (SDLK_f):
                    {
                        set_fullscreen(!is_fullscreen());
                    } break;
                }
            } break;
            case (SDL_QUIT):
            {
                gWindow.running = false;
            } break;
        }
    }

    update_application(gFrameTimer.delta_time);
    render_application(gFrameTimer.delta_time);
    render_display();
    cap_framerate();
}

int main (int argc, char** argv)
{
    init_frame_timer();

    load_settings();

    if (init_window())
    {
        if (init_renderer())
        {
            // We do not fail if audio doesn't initialize as we can still run the game without it.
            init_audio();

            if (init_application())
            {
                show_window();
                emscripten_set_main_loop(main_loop, -1, 1);
            }
        }
    }

    return 0;
}

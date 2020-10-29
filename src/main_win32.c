#include "main_win32.h"

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

                while (gWindow.running)
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
                                    case (SDLK_RETURN): if (!(SDL_GetModState()&KMOD_ALT)) break;
                                    // FALL-THROUGH!
                                    case (SDLK_f):
                                    // FALL-THROUGH!
                                    case (SDLK_F11):
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
                    cap_framerate();
                    render_display();
                }
                quit_application();

                save_settings();
            }
            quit_audio();
            quit_renderer();
        }
        quit_window();
    }

    quit_logger();
    return 0;
}

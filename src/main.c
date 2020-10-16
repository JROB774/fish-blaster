#include "main.h"

int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    init_window();
    init_renderer();

    show_window();

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case (SDL_KEYDOWN):
                {
                    switch (event.key.keysym.sym)
                    {
                        case (SDLK_F11):
                        case (SDLK_f):
                        {
                            set_fullscreen(!is_fullscreen());
                        } break;
                    }
                } break;
                case (SDL_QUIT):
                {
                    running = false;
                } break;
            }

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        render_clear(COLOR_BLACK);
        ARGBColor* pixels = get_screen();
        for (int i=0; i<(SCREEN_W*SCREEN_H); ++i) pixels[i] = i;
        render_display();
    }

    quit_renderer();
    quit_window();

    SDL_Quit();

    return 0;
}

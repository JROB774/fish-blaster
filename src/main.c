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
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        clear_renderer(0xFFFF0000);
        display_renderer();
    }

    quit_renderer();
    quit_window();

    SDL_Quit();

    return 0;
}

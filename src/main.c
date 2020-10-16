#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Defining these as blank is necessary in order to get SDL2 working with TCC.
// We also had to modify some typedefs in SDL_config.h, we will look into
// making that part more portable across compiler's in the future. However,
// for now it is fine as it is as we are the only ones compiling this source.
#ifdef __TINYC__
#define SDLCALL
#define DECLSPEC
#endif

#define SDL_MAIN_HANDLED 1
#include <SDL2/SDL.h>

typedef int bool;
#define false 0
#define true  1

int main (int argc, char** argv)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("REVIVALJAM", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 256,240, SDL_WINDOW_SHOWN);
    assert(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    assert(renderer);

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

        SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

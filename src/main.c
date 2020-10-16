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

#define SCREEN_W 256
#define SCREEN_H 240

typedef  uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

int main (int argc, char** argv)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("REVIVALJAM", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_W,SCREEN_H, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    assert(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    assert(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetWindowMinimumSize(window, SCREEN_W,SCREEN_H);

    SDL_Surface* screen;
    SDL_Texture* texture;

    U32 window_pixel_format = SDL_GetWindowPixelFormat(window);
    assert(window_pixel_format != SDL_PIXELFORMAT_UNKNOWN);

    // Convert the window's pixel format into a mask usable with SDL_CreateRGBSurface.
    U32 r,g,b,a;
    int bpp; // We don't use this but SDL needs us to pass it.
    SDL_PixelFormatEnumToMasks(window_pixel_format, &bpp, &r,&g,&b,&a);

    screen = SDL_CreateRGBSurface(0, SCREEN_W,SCREEN_H, 32, r,g,b,a); // ARGB data...
    assert(screen);
    texture = SDL_CreateTexture(renderer, window_pixel_format, SDL_TEXTUREACCESS_STREAMING, SCREEN_W,SCREEN_H);
    assert(texture);

    // Fill in the buffer with a smooth gradient to test the software rendering.
    U32* pixels = screen->pixels;
    for (int i=0; i<SCREEN_W*SCREEN_H; ++i) pixels[i] = i;

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

        SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

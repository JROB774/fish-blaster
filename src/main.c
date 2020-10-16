#define SCREEN_W 256
#define SCREEN_H 240

#include "main.h"

int main (int argc, char** argv)
{
    SDL_Renderer* renderer;

    SDL_Init(SDL_INIT_EVERYTHING);

    init_window();

    renderer = SDL_CreateRenderer(gWindow.window, -1, SDL_RENDERER_ACCELERATED);
    assert(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface* screen;
    SDL_Texture* texture;

    U32 window_pixel_format = SDL_GetWindowPixelFormat(gWindow.window);
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

        SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
        SDL_RenderClear(renderer);

        SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);

    quit_window();

    SDL_Quit();

    return 0;
}

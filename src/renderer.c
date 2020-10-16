INTERNAL void init_renderer ()
{
    gRenderer.renderer = SDL_CreateRenderer(gWindow.window, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer.renderer)
    {
        // @Incomplete: Handle error...
    }
    else
    {
        U32 pixel_format = SDL_GetWindowPixelFormat(gWindow.window);
        if (pixel_format == SDL_PIXELFORMAT_UNKNOWN)
        {
            // @Incomplete: Handle error...
        }
        else
        {
            // Convert the window's pixel format into a mask usable with SDL_CreateRGBSurface.
            U32 r,g,b,a;
            int bpp; // We don't use this but SDL needs us to pass it.
            if (!SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a))
            {
                // @Incomplete: Handle error...
            }
            else
            {
                gRenderer.screen = SDL_CreateRGBSurface(0, SCREEN_W,SCREEN_H, 32, r,g,b,a); // Our screen pixels.
                if (!gRenderer.screen)
                {
                    // @Incomplete: Handle error...
                }
                else
                {
                    gRenderer.target = SDL_CreateTexture(gRenderer.renderer,
                        pixel_format, SDL_TEXTUREACCESS_STREAMING, SCREEN_W,SCREEN_H);
                    if (!gRenderer.target)
                    {
                        // @Incomplete: Handle error...
                    }
                }
            }
        }
    }
}

INTERNAL void quit_renderer ()
{
    SDL_DestroyTexture(gRenderer.target);
    SDL_FreeSurface(gRenderer.screen);
    SDL_DestroyRenderer(gRenderer.renderer);
}

INTERNAL void clear_renderer (U32 color)
{
    SDL_SetRenderDrawColor(gRenderer.renderer, 0x00,0x00,0x00,0xFF);
    SDL_RenderClear(gRenderer.renderer);

    U32* pixels = gRenderer.screen->pixels;
    for (int i=0; i<(SCREEN_W*SCREEN_H); ++i) pixels[i] = color;
}

INTERNAL void display_renderer ()
{
    SDL_UpdateTexture(gRenderer.target, NULL, gRenderer.screen->pixels, gRenderer.screen->pitch);
    SDL_RenderCopy(gRenderer.renderer, gRenderer.target, NULL, NULL);
    SDL_RenderPresent(gRenderer.renderer);
}

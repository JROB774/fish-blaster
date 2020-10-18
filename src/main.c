#include "main.h"

INTERNAL void render_test (Bitmap* bitmap)
{
    render_clear(0xFF2F4B99);
    const ARGBColor FISH_PALETTE[4] = { 0xFF000000, 0xFFFF3F3F, 0xFFFFFFFF, 0x00000000 };
    Clip clip = { 32, 0, 16, 8 };
    render_bitmap(bitmap, 20,40, FISH_PALETTE, &clip);
}

int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    init_window();
    init_renderer();
    init_frame_timer();

    Bitmap bitmap;
    Font font;

    load_bitmap_from_file(&bitmap, "assets/tile.bmp");

    show_window();

    bool running = true;
    while (running)
    {
        update_input_state();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
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
                    running = false;
                } break;
            }
        }

        render_clear(COLOR_BLACK);
        render_test(&bitmap);
        cap_framerate();
        render_display();
    }

    free_bitmap(&bitmap);

    quit_renderer();
    quit_window();

    SDL_Quit();

    return 0;
}

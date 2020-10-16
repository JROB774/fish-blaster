#include "main.h"

INTERNAL void render_test (Bitmap* bitmap)
{
    static int i=0;

    ARGBColor* pixels = get_screen();
    for (int i=0; i<(SCREEN_W*SCREEN_H); ++i) pixels[i] = i;

    render_fill ( 20,20, 100,50, COLOR_RED);
    render_line ( 20,20, 119,69, COLOR_YELLOW);
    render_line ( 20,69, 119,20, COLOR_YELLOW);
    render_rect ( 20,20, 100,50, COLOR_WHITE);
    render_point( 20,20, COLOR_BLACK);
    render_point(119,20, COLOR_BLACK);
    render_point( 20,69, COLOR_BLACK);
    render_point(119,69, COLOR_BLACK);

    const ARGBColor palettes[4][4] =
    {
    { COLOR_RED,   COLOR_GREEN, COLOR_BLUE,  COLOR_WHITE },
    { COLOR_WHITE, COLOR_RED,   COLOR_GREEN, COLOR_BLUE  },
    { COLOR_BLUE,  COLOR_WHITE, COLOR_RED,   COLOR_GREEN },
    { COLOR_GREEN, COLOR_BLUE,  COLOR_WHITE, COLOR_RED   }
    };

    render_bitmap(bitmap, 0,0, palettes[i], NULL);
    i++;
    if (i > 3) i=0;
}

int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    init_window();
    init_renderer();

    Bitmap bitmap;
    load_bitmap_from_file(&bitmap, "assets/test.bmp");

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
                        case (SDLK_RETURN): if (!(SDL_GetModState()&KMOD_ALT)) break;
                        // FALL-THROUGH!
                        case (SDLK_F11):
                        // FALL-THROUGH!
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
        render_test(&bitmap);
        render_display();
    }

    free_bitmap(&bitmap);

    quit_renderer();
    quit_window();

    SDL_Quit();

    return 0;
}

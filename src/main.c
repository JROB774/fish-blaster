#include "main.h"

int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    init_window();
    init_renderer();

    Bitmap bitmap;
    load_bitmap_from_file(&bitmap, "assets/test.bmp");
    free_bitmap(&bitmap);

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

        render_fill ( 20,20, 100,50, COLOR_RED);
        render_line ( 20,20, 119,69, COLOR_YELLOW);
        render_line ( 20,69, 119,20, COLOR_YELLOW);
        render_rect ( 20,20, 100,50, COLOR_WHITE);
        render_point( 20,20, COLOR_BLACK);
        render_point(119,20, COLOR_BLACK);
        render_point( 20,69, COLOR_BLACK);
        render_point(119,69, COLOR_BLACK);

        render_display();
    }

    quit_renderer();
    quit_window();

    SDL_Quit();

    return 0;
}

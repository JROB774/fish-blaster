INTERNAL void init_window ()
{
    // We make the window hidden by default so that we can perform all of the game's
    // initialization before displaying the game window -- this looks a lot nicer.
    gWindow.window = SDL_CreateWindow("REVIVALJAM", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_W,SCREEN_H, SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE);
    if (!gWindow.window)
    {
        // @Incomplete: Handle error...
    }
    else
    {
        SDL_SetWindowMinimumSize(gWindow.window, SCREEN_W,SCREEN_H);
    }
}

INTERNAL void quit_window ()
{
    SDL_DestroyWindow(gWindow.window);
}

INTERNAL void show_window ()
{
    SDL_ShowWindow(gWindow.window);
}

INTERNAL void hide_window ()
{
    SDL_HideWindow(gWindow.window);
}

INTERNAL void set_fullscreen (bool enable)
{
    gWindow.fullscreen = enable;
    if (SDL_SetWindowFullscreen(gWindow.window, (gWindow.fullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) < 0)
    {
        // @Incomplete: Handle error...
    }
}

INTERNAL bool is_fullscreen ()
{
    return gWindow.fullscreen;
}

INTERNAL int get_window_w ()
{
    int width;
    SDL_GetWindowSize(gWindow.window, &width, NULL);
    return width;
}

INTERNAL int get_window_h ()
{
    int height;
    SDL_GetWindowSize(gWindow.window, NULL, &height);
    return height;
}

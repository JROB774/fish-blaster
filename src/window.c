INTERNAL bool init_window ()
{
    gWindow.running = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        LOGERROR("Failed to initialize SDL! (%s)", SDL_GetError());
        return false;
    }

    gWindow.cached_width = gSettings.window_width;
    gWindow.cached_height = gSettings.window_height;

    // We make the window hidden by default so that we can perform all of the game's
    // initialization before displaying the game window -- this looks a lot nicer.
    gWindow.window = SDL_CreateWindow("Fish Blaster", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, gWindow.cached_width,gWindow.cached_height, SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE);
    if (!gWindow.window)
    {
        LOGERROR("Failed to create window! (%s)", SDL_GetError());
        return false;
    }
    SDL_SetWindowMinimumSize(gWindow.window, SCREEN_W,SCREEN_H);

    set_fullscreen(gSettings.fullscreen);

    return true;
}

INTERNAL void quit_window ()
{
    SDL_DestroyWindow(gWindow.window);
    SDL_Quit();
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
        LOGWARNING("Failed to set window fullscreen state! (%s)", SDL_GetError());
    }
}

INTERNAL bool is_fullscreen ()
{
    return gWindow.fullscreen;
}

INTERNAL void set_window_size (int width, int height)
{
    SDL_SetWindowSize(gWindow.window, width, height);
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

INTERNAL int get_window_cached_w ()
{
    if (gWindow.fullscreen) return gWindow.cached_width;
    return get_window_w();
}
INTERNAL int get_window_cached_h ()
{
    if (gWindow.fullscreen) return gWindow.cached_height;
    return get_window_h();
}

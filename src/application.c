INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);
    return true;
}

INTERNAL void quit_application ()
{
    SDL_ShowCursor(SDL_ENABLE);
}

INTERNAL void update_application (float dt)
{
    // Nothing...
}

INTERNAL void render_application (float dt)
{
    render_clear(0xFF2F4B99);

    render_bitmap(20,40,PAL_FISH_RED,&SPR_FISH); // @Temporary!

    // Render the in-game mouse cursor/crosshair.
    float cx = get_mouse_x()-(SPR_CURSOR.w/2);
    float cy = get_mouse_y()-(SPR_CURSOR.h/2);
    render_bitmap(cx,cy,PAL_CURSOR,&SPR_CURSOR);
}

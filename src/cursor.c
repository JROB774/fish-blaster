INTERNAL void init_cursor ()
{
    SDL_ShowCursor(SDL_DISABLE);
}

INTERNAL void quit_cursor ()
{
    SDL_ShowCursor(SDL_ENABLE);
}

INTERNAL void render_cursor ()
{
    float x = get_mouse_x()-(SPR_CURSOR.w/2);
    float y = get_mouse_y()-(SPR_CURSOR.h/2);

    render_bitmap(x,y,PAL_CURSOR,&SPR_CURSOR);
}

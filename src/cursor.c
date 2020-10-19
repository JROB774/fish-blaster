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
    Clip clip = { 8,0,8,8 };
    float x = get_mouse_x()-(clip.w/2);
    float y = get_mouse_y()-(clip.h/2);
    render_bitmap(x,y,0,&clip);
}

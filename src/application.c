#define GAME_BACKGROUND_COLOR 0xFF2F4B99

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
    render_clear(GAME_BACKGROUND_COLOR);

    // @Temporary: Just testing out some rendering stuff right now!
    static float t = 0.0f;
    t += dt;
    if (t > 0.4f) t -= 0.4f;
    const Clip* rframe = (t < 0.2f) ? &SPR_FISH_R_0 : &SPR_FISH_R_1;
    const Clip* lframe = (t < 0.2f) ? &SPR_FISH_L_0 : &SPR_FISH_L_1;
    render_bitmap(20,40,PAL_FISH_0,rframe);
    render_bitmap(20,50,PAL_FISH_1,rframe);
    render_bitmap(20,60,PAL_FISH_2,rframe);
    render_bitmap(80,40,PAL_FISH_0,lframe);
    render_bitmap(80,50,PAL_FISH_1,lframe);
    render_bitmap(80,60,PAL_FISH_2,lframe);

    // Render the in-game mouse cursor/crosshair.
    float cx = get_mouse_x()-(SPR_CURSOR.w/2);
    float cy = get_mouse_y()-(SPR_CURSOR.h/2);
    render_bitmap(cx,cy,PAL_CURSOR,&SPR_CURSOR);
}

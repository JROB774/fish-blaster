INTERNAL bool init_application ()
{
    init_cursor();
    return true;
}

INTERNAL void quit_application ()
{
    quit_cursor();
}

INTERNAL void update_application (float dt)
{
    // Nothing...
}

INTERNAL void render_application (float dt)
{
    render_clear(0xFF2F4B99);
    // const ARGBColor FISH_PALETTE[4] = { COLOR_BLACK,0xFFFF3F3F,COLOR_WHITE,0 };
    // Clip clip = { 16, 0, 16, 8 };
    // render_bitmap(&gAppState.tilebmp, 20,40, FISH_PALETTE, &clip);
    render_cursor();
}

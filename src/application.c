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
    render_bitmap(20,40,PAL_FISH_RED,&SPR_FISH);
    render_cursor();
}

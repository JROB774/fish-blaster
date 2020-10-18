INTERNAL void init_application ()
{
    load_bitmap_from_file(&gAppState.tilebmp, "assets/tile.bmp");
}

INTERNAL void quit_application ()
{
    free_bitmap(&gAppState.tilebmp);
}

INTERNAL void update_application (float dt)
{
    // Nothing...
}

INTERNAL void render_application (float dt)
{
    render_clear(0xFF2F4B99);
    const ARGBColor FISH_PALETTE[4] = { 0xFF000000, 0xFFFF3F3F, 0xFFFFFFFF, 0x00000000 };
    Clip clip = { 32, 0, 16, 8 };
    render_bitmap(&gAppState.tilebmp, 20,40, FISH_PALETTE, &clip);
}

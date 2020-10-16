#pragma pack(push,1)
typedef struct BMPHeader__
{
    U16 file_type;
    U32 file_size;
    U16 reserved1;
    U16 reserved2;
    U32 bitmap_offset;
    U32 size;
    S32 width;
    S32 height;
    U16 planes;
    U16 bits_per_pixel;

} BMPHeader;
#pragma pack(pop)

INTERNAL void init_renderer ()
{
    gRenderer.renderer = SDL_CreateRenderer(gWindow.window, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer.renderer)
    {
        // @Incomplete: Handle error...
    }
    else
    {
        U32 pixel_format = SDL_GetWindowPixelFormat(gWindow.window);
        if (pixel_format == SDL_PIXELFORMAT_UNKNOWN)
        {
            // @Incomplete: Handle error...
        }
        else
        {
            // Convert the window's pixel format into a mask usable with SDL_CreateRGBSurface.
            U32 r,g,b,a;
            int bpp; // We don't use this but SDL needs us to pass it.
            if (!SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a))
            {
                // @Incomplete: Handle error...
            }
            else
            {
                gRenderer.screen = SDL_CreateRGBSurface(0, SCREEN_W,SCREEN_H, 32, r,g,b,a); // Our screen pixels.
                if (!gRenderer.screen)
                {
                    // @Incomplete: Handle error...
                }
                else
                {
                    gRenderer.target = SDL_CreateTexture(gRenderer.renderer,
                        pixel_format, SDL_TEXTUREACCESS_STREAMING, SCREEN_W,SCREEN_H);
                    if (!gRenderer.target)
                    {
                        // @Incomplete: Handle error...
                    }
                }
            }
        }
    }
}

INTERNAL void quit_renderer ()
{
    SDL_DestroyTexture(gRenderer.target);
    SDL_FreeSurface(gRenderer.screen);
    SDL_DestroyRenderer(gRenderer.renderer);
}

INTERNAL void load_bitmap_from_file (Bitmap* bitmap, const char* file_name)
{
    assert(bitmap);

    // @Cleanup: Pull this out into a general-purpose read entire file function!!!
    // Read the entire bitmap file into memory.
    size_t buffer_size;
    U8* buffer_data;
    HANDLE file = CreateFileA(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        // @Incomplete: Handle error...
    }
    else
    {
        buffer_size = GetFileSize(file, NULL);
        if (buffer_size == INVALID_FILE_SIZE)
        {
            // @Incomplete: Handle error...
        }
        else
        {
            buffer_data = VirtualAlloc(NULL, buffer_size, MEM_COMMIT, PAGE_READWRITE);
            if (!buffer_data)
            {
                // @Incomplete: Handle error...
            }
            else
            {
                DWORD unused_but_needed;
                ReadFile(file, buffer_data, buffer_size, &unused_but_needed, NULL);
            }
        }
        CloseHandle(file);
    }

    if (buffer_data)
    {
        BMPHeader* header = CAST(BMPHeader*, buffer_data);
        assert(header->bits_per_pixel == 4); // NOTE: Currently we are only handling BMP files with 4-bit depth!

        int line_size = (header->width/2+(header->width/2) % 4); // BMP lines are aligned to a 4-byte boundary.

        bitmap->w = header->width;
        bitmap->h = header->height;

        bitmap->pixels = VirtualAlloc(NULL, (bitmap->w*bitmap->h), MEM_COMMIT, PAGE_READWRITE);
        if (bitmap->pixels)
        {
            U8*  src = buffer_data+header->bitmap_offset;
            U32* dst = bitmap->pixels;

            // Decode bitmap bits into palette index values from 0-3.
            for (int iy=0; iy<bitmap->h; ++iy)
            {
                for (int ix=0; ix<bitmap->w/2; ++ix)
                {
                    int src_byte = iy * line_size + ix;
                    // int dst_index = (bitmap->h-1-iy) * bitmap->w + ix * 2;
                    printf("%d %d ", (src[src_byte]>>4)&0xF, src[src_byte]&0xF);
                }
                printf("\n");
            }
        }

        VirtualFree(buffer_data, 0, MEM_RELEASE);
    }
}

INTERNAL void free_bitmap (Bitmap* bitmap)
{
    assert(bitmap);
    if (bitmap->pixels) VirtualFree(bitmap->pixels, 0, MEM_RELEASE);
}

INTERNAL ARGBColor* get_screen ()
{
    return CAST(ARGBColor*, gRenderer.screen->pixels);
}

INTERNAL void render_clear (ARGBColor color)
{
    // We clear outside the viewport to black.
    SDL_SetRenderDrawColor(gRenderer.renderer, 0x00,0x00,0x00,0xFF);
    SDL_RenderClear(gRenderer.renderer);

    for (int i=0; i<(SCREEN_W*SCREEN_H); ++i) CAST(ARGBColor*,gRenderer.screen->pixels)[i] = color;
}

INTERNAL void render_display ()
{
    // Determine how much we can scale the viewport up by when rendering to the window.
    SDL_Rect viewport;
    int scale_x = get_window_w() / SCREEN_W;
    int scale_y = get_window_h() / SCREEN_H;
    int scale   = MIN(scale_x,scale_y);
    viewport.w  = SCREEN_W * scale;
    viewport.h  = SCREEN_H * scale;
    viewport.x  = (get_window_w()-viewport.w) / 2;
    viewport.y  = (get_window_h()-viewport.h) / 2;

    SDL_UpdateTexture(gRenderer.target, NULL, gRenderer.screen->pixels, gRenderer.screen->pitch);
    SDL_RenderCopy(gRenderer.renderer, gRenderer.target, NULL, &viewport);
    SDL_RenderPresent(gRenderer.renderer);
}

INTERNAL void render_point (int x, int y, ARGBColor color)
{
    if (x < get_render_target_min_x() || x >= get_render_target_max_x() ||
        y < get_render_target_min_y() || y >= get_render_target_max_y()) return;
    get_screen()[y*SCREEN_W+x] = color;
}

INTERNAL void render_line (int x1, int y1, int x2, int y2, ARGBColor color)
{
    if (x1 > get_render_target_max_x() && x2 > get_render_target_max_x()) return;
    if (y1 > get_render_target_max_y() && y2 > get_render_target_max_y()) return;

    // Clamp the bounds to avoid overflows.
    x1 = CLAMP(x1, get_render_target_min_x(), get_render_target_max_x());
    y1 = CLAMP(y1, get_render_target_min_y(), get_render_target_max_y());
    x2 = CLAMP(x2, get_render_target_min_x(), get_render_target_max_x());
    y2 = CLAMP(y2, get_render_target_min_y(), get_render_target_max_y());

    bool steep = false;
    if (ABS(x1-x2)<ABS(y1-y2))
    {
        SWAP(x1, y1, int);
        SWAP(x2, y2, int);
        steep = true;
    }
    if (x1>x2)
    {
        SWAP(x1, x2, int);
        SWAP(y1, y2, int);
    }
    int dx = x2-x1;
    int dy = y2-y1;
    int derror2 = ABS(dy)*2;
    int error2 = 0;
    int iy = y1;

    ARGBColor* pixels = get_screen();
    for (int ix=x1; ix<=x2; ++ix)
    {
        if (steep) pixels[ix*SCREEN_W+iy] = color;
        else pixels[iy*SCREEN_W+ix] = color;
        error2 += derror2;
        if (error2 > dx)
        {
            iy += (y2>y1?1:-1);
            error2 -= dx*2;
        }
    }
}

INTERNAL void render_rect (int x, int y, int w, int h, ARGBColor color)
{
    // NOTE: We assume that the width and height are not negative...

    if (x > get_render_target_max_x()) return;
    if (y > get_render_target_max_y()) return;

    int x1 = x;
    int y1 = y;
    int x2 = x+w-1;
    int y2 = y+h-1;

    render_line(x2,y1,x2,y2, color); // Right
    render_line(x1,y1,x1,y2, color); // Left
    render_line(x1,y1,x2,y1, color); // Top
    render_line(x1,y2,x2,y2, color); // Bottom
}

INTERNAL void render_fill (int x, int y, int w, int h, ARGBColor color)
{
    // NOTE: We assume that the width and height are not negative...

    if (x > SCREEN_W-1) return;
    if (y > SCREEN_H-1) return;

    int x1 = x;
    int y1 = y;
    int x2 = x+w-1;
    int y2 = y+h-1;

    // Clamp the bounds to avoid overflows.
    x1 = CLAMP(x1, get_render_target_min_x(), get_render_target_max_x());
    y1 = CLAMP(y1, get_render_target_min_y(), get_render_target_max_y());
    x2 = CLAMP(x2, get_render_target_min_x(), get_render_target_max_x());
    y2 = CLAMP(y2, get_render_target_min_y(), get_render_target_max_y());

    ARGBColor* pixels = get_screen();
    for (int iy=y1; iy<=y2; ++iy)
    {
        for (int ix=x1; ix<=x2; ++ix)
        {
            pixels[iy*SCREEN_W+ix] = color;
        }
    }
}

// INTERNAL INTERFACE

INTERNAL int get_render_target_min_x ()
{
    return 0;
}
INTERNAL int get_render_target_max_x ()
{
    return gRenderer.screen->w;
}
INTERNAL int get_render_target_min_y ()
{
    return 0;
}
INTERNAL int get_render_target_max_y ()
{
    return gRenderer.screen->h;
}

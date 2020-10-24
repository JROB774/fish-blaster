////////////////////////////////////////////////////////////////////////////////

// The stuff in this section is all used internally by the renderer system!

#define PALETTE_BLACK 0xFF000000
#define PALETTE_COLOR 0xFF828282
#define PALETTE_WHITE 0xFFFFFFFF
#define PALETTE_CLEAR 0xFFC1C1C1

#define RENDER_TEXT_BUFFER_SIZE 1024
#define FIRST_GLYPH_TILE_INDEX   265

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
    U32 compression;
    U32 image_size;
    S32 x_pixels_per_meter;
    S32 y_pixels_per_meter;
    U32 num_colors;
    U32 num_important_colors;

} BMPHeader;
#pragma pack(pop)

typedef struct Bitmap__
{
    int w,h;
    int* pixels; // Indicies [0-3] into a 4-color palette specified on render.

} Bitmap;

typedef struct Palette__
{
    int w,h;
    ARGBColor* pixels;

} Palette;

GLOBAL struct
{
    SDL_Renderer* renderer;
    SDL_Surface*  screen;
    SDL_Texture*  target;
    SDL_Rect      viewport;
    Bitmap        bitmap;
    Palette       palette;
    int           palette_mode;
    int           txoffset;
    int           tyoffset;
    int           xoffset;
    int           yoffset;
    float         shake_time;
    int           shake_x;
    int           shake_y;

} gRenderer;

INTERNAL ARGBColor* get_screen ()
{
    return CAST(ARGBColor*, gRenderer.screen->pixels);
}

INTERNAL int get_render_target_min_x () { return 0;                     }
INTERNAL int get_render_target_max_x () { return gRenderer.screen->w-1; }
INTERNAL int get_render_target_min_y () { return 0;                     }
INTERNAL int get_render_target_max_y () { return gRenderer.screen->h-1; }

// The caller is required to free the returned file data!
INTERNAL U8* read_entire_file (const char* file_name)
{
    U8* buffer = NULL;
    FILE* file = fopen(file_name, "rb");
    if (file)
    {
        fseek(file, 0L, SEEK_END);
        size_t size = ftell(file);
        rewind(file);
        buffer = malloc(size*sizeof(U8));
        if (buffer)
        {
            fread(buffer, size*sizeof(U8), 1, file);
        }
        fclose(file);
    }
    return buffer;
}

// The caller is required to free the returned file data!
INTERNAL U8* read_bitmap_file (const char* file_name)
{
    // Read the entire bitmap file into memory.
    U8* bitmap_data = read_entire_file(file_name);
    if (!bitmap_data)
    {
        return NULL;
    }
    else
    {
        // Some additional checks are performed to make sure the bitmap file is
        // in the format that we desire so that we can parse its data correctly.
        BMPHeader* header = CAST(BMPHeader*, bitmap_data);
        if (header->file_type != 0x4D42) // This magic number is 'BM' in lil endian byteorder.
        {
            LOGERROR("BMP \"%s\" is unsupported type!", file_name);
            free(bitmap_data);
            bitmap_data = NULL;
        }
    }
    return bitmap_data;
}

////////////////////////////////////////////////////////////////////////////////

INTERNAL bool init_renderer ()
{
    //
    // This section of code sets up the actual software renderer itself.
    //

    gRenderer.renderer = SDL_CreateRenderer(gWindow.window, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer.renderer)
    {
        LOGERROR("Failed to create renderer! (%s)", SDL_GetError());
        return false;
    }
    U32 pixel_format = SDL_GetWindowPixelFormat(gWindow.window);
    if (pixel_format == SDL_PIXELFORMAT_UNKNOWN)
    {
        LOGERROR("Failed to retrieve window pixel format! (%s)", SDL_GetError());
        return false;
    }
    // Convert the window's pixel format into a mask usable with SDL_CreateRGBSurface.
    U32 r,g,b,a;
    int bpp; // We don't use this but SDL needs us to pass it.
    if (!SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a))
    {
        LOGERROR("Failed to convert format to mask! (%s)", SDL_GetError());
        return false;
    }
    gRenderer.screen = SDL_CreateRGBSurface(0, SCREEN_W,SCREEN_H, 32, r,g,b,a); // Our screen pixels.
    if (!gRenderer.screen)
    {
        LOGERROR("Failed to create screen buffer! (%s)", SDL_GetError());
        return false;
    }
    gRenderer.target = SDL_CreateTexture(gRenderer.renderer,
        pixel_format, SDL_TEXTUREACCESS_STREAMING, SCREEN_W,SCREEN_H);
    if (!gRenderer.target)
    {
        LOGERROR("Failed to create screen texture! (%s)", SDL_GetError());
        return false;
    }

    //
    // This section of code loads the bitmap containing all the game's graphics.
    //

    U8* bitmap_data = read_bitmap_file("assets/imgbit.bmp");
    if (!bitmap_data)
    {
        LOGERROR("Failed to load the renderer graphics!");
        return false;
    }
    else
    {
        bool success = true;

        // Perform an extra check to make sure our bitmap graphics are in the exepcted 4-bit data format.
        BMPHeader* header = CAST(BMPHeader*, bitmap_data);
        if (header->bits_per_pixel != 4)
        {
            LOGERROR("Renderer graphics are not a 4-bit bitmap!");
            success = false;
        }
        else
        {
            // Parse the bitmap color table for palette information.
            ARGBColor* color_table = CAST(ARGBColor*, bitmap_data+sizeof(BMPHeader));
            int palette_index[4] = {0};
            for (int i=0; i<header->num_colors; ++i)
            {
                switch (color_table[i])
                {
                    case (PALETTE_BLACK): palette_index[i] = 0; break;
                    case (PALETTE_COLOR): palette_index[i] = 1; break;
                    case (PALETTE_WHITE): palette_index[i] = 2; break;
                    case (PALETTE_CLEAR): palette_index[i] = 3; break;
                    default:
                        LOGDEBUG("Graphics contains unrecognized color 0x%08X!", color_table[i]);
                }
            }

            // Decode the color data from 4-bit BMP indexed color into indices into our internal palette system!
            int line_size = (header->width/2+(header->width/2) % 4); // BMP lines are aligned to a 4-byte boundary.

            gRenderer.bitmap.w = header->width;
            gRenderer.bitmap.h = header->height;

            gRenderer.bitmap.pixels = malloc((gRenderer.bitmap.w*gRenderer.bitmap.h)*sizeof(int));
            if (!gRenderer.bitmap.pixels)
            {
                LOGERROR("Failed to allocate renderer graphics!");
                success = false;
            }
            else
            {
                U8 * src = bitmap_data+header->bitmap_offset;
                int* dst = gRenderer.bitmap.pixels;

                // Decode bitmap bits into palette index values from 0-3.
                for (int iy=0; iy<gRenderer.bitmap.h; ++iy)
                {
                    for (int ix=0; ix<gRenderer.bitmap.w/2; ++ix)
                    {
                        int src_byte = iy * line_size + ix;
                        int dst_index = (gRenderer.bitmap.h-1-iy) * gRenderer.bitmap.w + (ix*2);

                        int hi = (src[src_byte]>>4) & 0xF;
                        int lo = (src[src_byte]   ) & 0xF;

                        dst[dst_index  ] = palette_index[hi];
                        dst[dst_index+1] = palette_index[lo];
                    }
                }
            }
        }
        free(bitmap_data);
        if (!success)
        {
            return false;
        }
    }

    //
    // This section of code loads the bitmap containing all the game's palettes.
    //

    U8* palette_data = read_bitmap_file("assets/imgpal.bmp");
    if (!palette_data)
    {
        LOGERROR("Failed to load the renderer palettess!");
        return false;
    }
    else
    {
        bool success = true;

        // Perform an extra check to make sure our palette graphics are in the exepcted 32-bit data format.
        BMPHeader* header = CAST(BMPHeader*, palette_data);
        if (header->bits_per_pixel != 32)
        {
            LOGERROR("Renderer palettes are not a 32-bit bitmap!");
            success = false;
        }
        else
        {
            gRenderer.palette.w = header->width;
            gRenderer.palette.h = header->height;

            gRenderer.palette.pixels = malloc((gRenderer.palette.w*gRenderer.palette.h)*sizeof(ARGBColor));
            if (!gRenderer.palette.pixels)
            {
                LOGERROR("Failed to allocate renderer palettes!");
                success = false;
            }
            else
            {
                // We just need to copy the palette data over from the bitmap as its already in our desired format.
                // The only processing we want to do is converting our clear color to all zeroes for transparency.
                ARGBColor* pixels = CAST(ARGBColor*, palette_data+header->bitmap_offset);
                for (int iy=0; iy<gRenderer.palette.h; ++iy)
                {
                    for (int ix=0; ix<gRenderer.palette.w; ++ix)
                    {
                        int src = (                      iy) * gRenderer.palette.w + ix;
                        int dst = (gRenderer.palette.h-1-iy) * gRenderer.palette.w + ix;

                        gRenderer.palette.pixels[dst] = ((pixels[src] == PALETTE_CLEAR) ? 0 : pixels[src]);
                    }
                }
            }
        }
        free(palette_data);
        if (!success)
        {
            return false;
        }
    }

    return true;
}

INTERNAL void quit_renderer ()
{
    free(gRenderer.palette.pixels);
    free(gRenderer.bitmap.pixels);
    SDL_DestroyTexture(gRenderer.target);
    SDL_FreeSurface(gRenderer.screen);
    SDL_DestroyRenderer(gRenderer.renderer);
}

INTERNAL void render_clear (ARGBColor color)
{
    // We clear outside the viewport to black.
    ARGBColor border_color = get_palette_color(PAL_BLACK,0);

    int r = (border_color >> 16) & 0xFF;
    int g = (border_color >>  8) & 0xFF;
    int b = (border_color >>  0) & 0xFF;

    SDL_SetRenderDrawColor(gRenderer.renderer, r,g,b,0xFF);
    SDL_RenderClear(gRenderer.renderer);

    // We clear the inside to what has been passed in.
    for (int i=0; i<(SCREEN_W*SCREEN_H); ++i) CAST(ARGBColor*,gRenderer.screen->pixels)[i] = color;
}

INTERNAL void render_display ()
{
    // Determine how much we can scale the viewport up by when rendering to the window.
    int scale_x = get_window_w() / SCREEN_W;
    int scale_y = get_window_h() / SCREEN_H;
    int scale   = MIN(scale_x,scale_y);

    gRenderer.viewport.w  = SCREEN_W * scale;
    gRenderer.viewport.h  = SCREEN_H * scale;
    gRenderer.viewport.x  = (get_window_w()-gRenderer.viewport.w) / 2;
    gRenderer.viewport.y  = (get_window_h()-gRenderer.viewport.h) / 2;

    SDL_UpdateTexture(gRenderer.target, NULL, gRenderer.screen->pixels, gRenderer.screen->pitch);
    SDL_RenderCopy(gRenderer.renderer, gRenderer.target, NULL, &gRenderer.viewport);
    SDL_RenderPresent(gRenderer.renderer);
}

INTERNAL void render_bitmap (int x, int y, int palette_index, const Clip* clip)
{
    assert(clip);

    x += gRenderer.xoffset;
    y += gRenderer.yoffset;

    if (x > get_render_target_max_x()) return;
    if (y > get_render_target_max_y()) return;

    int*       src = gRenderer.bitmap.pixels;
    ARGBColor* dst = get_screen();

    int bx = clip->x;
    int by = clip->y;
    int bw = clip->w;
    int bh = clip->h;

    // The rectangular region we will be drawing to.
    int x1 = x;
    int y1 = y;
    int x2 = x + bw-1;
    int y2 = y + bh-1;

    // The graphic is entirely off-screen!
    if (x2 < get_render_target_min_x() || y2 < get_render_target_min_y()) return;

    // Clamp the bounds to avoid overflows.
    x2 = CLAMP(x2, get_render_target_min_x(), get_render_target_max_x());
    y2 = CLAMP(y2, get_render_target_min_y(), get_render_target_max_y());

    ARGBColor* palette = gRenderer.palette.pixels + (palette_index*gRenderer.palette.w+(gRenderer.palette_mode*4));
    for (int iy=y1,sy=by; iy<=y2; ++iy,++sy)
    {
        if (iy >= get_render_target_min_y())
        {
            for (int ix=x1,sx=bx; ix<=x2; ++ix,++sx)
            {
                if (ix >= get_render_target_min_x())
                {
                    ARGBColor color = palette[src[sy*gRenderer.bitmap.w+sx]];
                    if (color) dst[iy*SCREEN_W+ix] = color;
                }
            }
        }
    }
}

INTERNAL void render_text (int x, int y, int palette_index, const char* text, ...)
{
    x += gRenderer.xoffset;
    y += gRenderer.yoffset;

    if (x > get_render_target_max_x()) return;
    if (y > get_render_target_max_y()) return;

    char text_buffer[RENDER_TEXT_BUFFER_SIZE] = {0};

    // Format the arguments into a formatted string in a buffer.
    va_list args;
    va_start(args, text);
    vsnprintf(text_buffer, RENDER_TEXT_BUFFER_SIZE, text, args);
    va_end(args);

    int start_x = x;
    int start_y = y;

    for (const char* c=text_buffer; *c; ++c)
    {
        switch (*c)
        {
            case ('\n'): x = start_x, y += TILE_H; break;
            default:
            {
                int tile = FIRST_GLYPH_TILE_INDEX+((*c)-(' '));
                Clip glyph;
                glyph.x = tile % (gRenderer.bitmap.w / TILE_W) * TILE_W;
                glyph.y = tile / (gRenderer.bitmap.w / TILE_W) * TILE_H;
                glyph.w = TILE_W;
                glyph.h = TILE_H;
                render_bitmap(x,y, palette_index, &glyph);
                x += TILE_W;
            } break;
        }
    }
}

INTERNAL void render_point (int x, int y, ARGBColor color)
{
    x += gRenderer.xoffset;
    y += gRenderer.yoffset;

    if (x < get_render_target_min_x() || x >= get_render_target_max_x() ||
        y < get_render_target_min_y() || y >= get_render_target_max_y()) return;

    get_screen()[y*SCREEN_W+x] = color;
}

INTERNAL void render_line (int x1, int y1, int x2, int y2, ARGBColor color)
{
    x1 += gRenderer.xoffset;
    y1 += gRenderer.yoffset;
    x2 += gRenderer.xoffset;
    y2 += gRenderer.yoffset;

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

    x += gRenderer.xoffset;
    y += gRenderer.yoffset;

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

    x += gRenderer.xoffset;
    y += gRenderer.yoffset;

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

void xLine(int x1, int x2, int y, ARGBColor colour)
{
    while (x1 <= x2) render_point(x1++, y, colour);
}

void yLine(int x, int y1, int y2, ARGBColor colour)
{
    while (y1 <= y2) render_point(x, y1++, colour);
}

// Circle drawing algorithm, with thickness, taken from here: https://stackoverflow.com/a/27756701
INTERNAL void render_circle (int x, int y, int r, int thickness, ARGBColor color)
{
    int outer = r;
    int inner = outer-thickness+1;

    int xo   = outer;
    int xi   = inner;
    int yy   = 0;
    int erro = 1-xo;
    int erri = 1-xi;

    while (xo >= yy)
    {
        render_line(x+xi, y+yy, x+xo, y+yy, color);
        render_line(x+yy, y+xi, x+yy, y+xo, color);
        render_line(x-xo, y+yy, x-xi, y+yy, color);
        render_line(x-yy, y+xi, x-yy, y+xo, color);
        render_line(x-xo, y-yy, x-xi, y-yy, color);
        render_line(x-yy, y-xo, x-yy, y-xi, color);
        render_line(x+xi, y-yy, x+xo, y-yy, color);
        render_line(x+yy, y-xo, x+yy, y-xi, color);

        yy++;

        if (erro < 0)
        {
            erro += 2*yy+1;
        }
        else
        {
            xo--;
            erro += 2*(yy-xo+1);
        }

        if (yy > inner)
        {
            xi = yy;
        }
        else
        {
            if (erri < 0)
            {
                erri += 2*yy+1;
            }
            else
            {
                xi--;
                erri += 2*(yy-xi+1);
            }
        }
    }
}

INTERNAL void set_palette_mode (int palette_mode)
{
    gRenderer.palette_mode = palette_mode;
}

INTERNAL SDL_Rect get_viewport ()
{
    return gRenderer.viewport;
}

INTERNAL int get_text_w (const char* text, ...)
{
    char text_buffer[RENDER_TEXT_BUFFER_SIZE] = {0};

    // Format the arguments into a formatted string in a buffer.
    va_list args;
    va_start(args, text);
    vsnprintf(text_buffer, RENDER_TEXT_BUFFER_SIZE, text, args);
    va_end(args);

    int linewidth = 0;
    int width = 0;

    for (int i=0; i<strlen(text_buffer); ++i)
    {
        if (text_buffer[i] == '\n')
        {
            width = MAX(width, linewidth);
            linewidth = 0;
        }
        else
        {
            linewidth += TILE_W;
        }
    }

    return MAX(width, linewidth);
}

INTERNAL int get_text_h (const char* text, ...)
{
    char text_buffer[RENDER_TEXT_BUFFER_SIZE] = {0};

    // Format the arguments into a formatted string in a buffer.
    va_list args;
    va_start(args, text);
    vsnprintf(text_buffer, RENDER_TEXT_BUFFER_SIZE, text, args);
    va_end(args);

    int height = TILE_H;

    for (int i=0; i<strlen(text_buffer); ++i)
    {
        if (text_buffer[i] == '\n')
        {
            height += TILE_H;
        }
    }

    return height;
}

INTERNAL ARGBColor get_palette_color (int palette_index, int color_index)
{
    assert((color_index >= 0) && (color_index < gRenderer.palette.w));
    ARGBColor* palette = gRenderer.palette.pixels + (palette_index*gRenderer.palette.w+(gRenderer.palette_mode*4));
    return palette[color_index];
}

INTERNAL void shake_camera (int x, int y, float duration)
{
    gRenderer.shake_x = x, gRenderer.shake_y = y;
    gRenderer.shake_time = duration;
}

INTERNAL void update_camera (float dt)
{
    // Perform screen/camera shake and update the timer.
    if (gRenderer.shake_time > 0.0f)
    {
        gRenderer.txoffset = random_int_range(-abs(gRenderer.shake_x), abs(gRenderer.shake_x));
        gRenderer.tyoffset = random_int_range(-abs(gRenderer.shake_y), abs(gRenderer.shake_y));
        gRenderer.shake_time -= dt;
    }
    else
    {
        gRenderer.txoffset = 0;
        gRenderer.tyoffset = 0;
    }
}

INTERNAL void begin_camera ()
{
    gRenderer.xoffset = gRenderer.txoffset;
    gRenderer.yoffset = gRenderer.tyoffset;
}

INTERNAL void end_camera ()
{
    gRenderer.xoffset = 0;
    gRenderer.yoffset = 0;
}

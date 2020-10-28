#ifndef PLAYER_H
#define PLAYER_H

#define GOD_DURATION  1.00f
#define ITEM_DURATION 9.99f

#define MAX_LIFE 2

#define NUM_SPRD_SHOT 4
#define SPRD_COOLDOWN 0.25f

typedef enum ItemID__
{
    ITEM_NONE,
    ITEM_TIME,
    ITEM_MULT,
    ITEM_SPRD,
    ITEM_RAPD

} ItemID;

typedef enum CursorID__
{
    CUR_0,
    CUR_1,
    CUR_2,
    CUR_3,
    CUR_4,
    CUR_TOTAL

} CursorID;

GLOBAL struct
{
    int cursor_x, cursor_prev_x;
    int cursor_y, cursor_prev_y;

    int life;

    CursorID current_cursor;
    ItemID current_item;

    float cooldown_time;
    float god_time;
    float item_time;

} gPlayer;

INTERNAL void   init_player ();
INTERNAL void create_player ();
INTERNAL void update_player (float dt);
INTERNAL void render_player (float dt);
INTERNAL void    hit_player ();

INTERNAL void set_player_cursor_type (CursorID cursor);

INTERNAL bool is_player_in_screen_bounds ();

#endif /* PLAYER_H */

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

GLOBAL struct
{
    int cursor_x, cursor_prev_x;
    int cursor_y, cursor_prev_y;

    int life;

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

INTERNAL bool is_player_in_screen_bounds ();

#endif /* PLAYER_H */

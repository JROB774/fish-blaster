GLOBAL struct
{
    bool prev_mouse_state   [MOUSE_BUTTON_TOTAL];
    bool curr_mouse_state   [MOUSE_BUTTON_TOTAL];
    U8   prev_keyboard_state[SDL_NUM_SCANCODES ];
    U8   curr_keyboard_state[SDL_NUM_SCANCODES ];

} gInputState;

INTERNAL void update_input_state ()
{
    // MOUSE
    memmove(gInputState.prev_mouse_state, gInputState.curr_mouse_state, sizeof(gInputState.curr_mouse_state));
    gInputState.curr_mouse_state[LMB] = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT );
    gInputState.curr_mouse_state[RMB] = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
    // KEYBOARD
    memcpy(gInputState.prev_keyboard_state, gInputState.curr_keyboard_state, SDL_NUM_SCANCODES*sizeof(U8));
    memcpy(gInputState.curr_keyboard_state, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES*sizeof(U8));
}

// MOUSE

INTERNAL bool button_pressed (MouseButton button)
{
    assert(button == LMB || button == RMB);
    return (!gInputState.prev_mouse_state[button] && gInputState.curr_mouse_state[button]);
}
INTERNAL bool button_released (MouseButton button)
{
    assert(button == LMB || button == RMB);
    return (gInputState.prev_mouse_state[button] && !gInputState.curr_mouse_state[button]);
}
INTERNAL bool button_down (MouseButton button)
{
    assert(button == LMB || button == RMB);
    return gInputState.curr_mouse_state[button];
}
INTERNAL bool button_up (MouseButton button)
{
    assert(button == LMB || button == RMB);
    return !gInputState.curr_mouse_state[button];
}

INTERNAL bool is_mouse_in_screen_bounds ()
{
    int mx = get_mouse_x(), my = get_mouse_y();
    return (mx >= 0 && mx < SCREEN_W && my >= 0 && my < SCREEN_H);
}

INTERNAL float get_mouse_x ()
{
    int scale_x = get_window_w() / SCREEN_W;
    int scale_y = get_window_h() / SCREEN_H;
    int scale   = MIN(scale_x,scale_y);

    int x; SDL_GetMouseState(&x, NULL);
    return CAST(float, (x-get_viewport().x) / scale);
}
INTERNAL float get_mouse_y ()
{
    int scale_x = get_window_w() / SCREEN_W;
    int scale_y = get_window_h() / SCREEN_H;
    int scale   = MIN(scale_x,scale_y);

    int y; SDL_GetMouseState(NULL, &y);
    return CAST(float, (y-get_viewport().y) / scale);
}

// KEYBOARD

INTERNAL bool key_pressed (KeyboardCode code)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
    assert(scancode < 0 || scancode > SDL_NUM_SCANCODES);
    return (gInputState.curr_keyboard_state[scancode] && !gInputState.prev_keyboard_state[scancode]);
}

INTERNAL bool key_released (KeyboardCode code)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
    assert(scancode < 0 || scancode > SDL_NUM_SCANCODES);
    return (!gInputState.curr_keyboard_state[scancode] && gInputState.prev_keyboard_state[scancode]);
}

INTERNAL bool key_down (KeyboardCode code)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
    assert(scancode < 0 || scancode > SDL_NUM_SCANCODES);
    return (gInputState.curr_keyboard_state[scancode] != 0);
}

INTERNAL bool key_up (KeyboardCode code)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
    assert(scancode < 0 || scancode > SDL_NUM_SCANCODES);
    return (gInputState.curr_keyboard_state[scancode] == 0);
}

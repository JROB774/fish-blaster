GLOBAL struct
{
    bool prev_mouse_state[MOUSE_BUTTON_TOTAL];
    bool curr_mouse_state[MOUSE_BUTTON_TOTAL];

} gInputState;

INTERNAL void update_input_state ()
{
    MoveMemory(gInputState.prev_mouse_state, gInputState.curr_mouse_state, sizeof(gInputState.curr_mouse_state));

    gInputState.curr_mouse_state[LMB] = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT );
    gInputState.curr_mouse_state[RMB] = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
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

INTERNAL float get_mouse_x ()
{
    int x; SDL_GetMouseState(&x, NULL);
    return CAST(float, x);
}

INTERNAL float get_mouse_y ()
{
    int y; SDL_GetMouseState(NULL, &y);
    return CAST(float, y);
}

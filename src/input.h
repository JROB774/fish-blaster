#ifndef INPUT_H
#define INPUT_H

INTERNAL void update_input_state ();

// MOUSE

typedef enum MouseButton__ { LMB = SDL_BUTTON_LEFT, RMB = SDL_BUTTON_RIGHT, MOUSE_BUTTON_TOTAL } MouseButton;

INTERNAL bool button_pressed  (MouseButton button);
INTERNAL bool button_released (MouseButton button);
INTERNAL bool button_down     (MouseButton button);
INTERNAL bool button_up       (MouseButton button);

INTERNAL bool is_mouse_in_screen_bounds ();

INTERNAL float get_mouse_x ();
INTERNAL float get_mouse_y ();

// KEYBOARD

typedef enum KeyboardCode__ { ESCAPE = SDLK_ESCAPE, RESTART = SDLK_r, KEYBOARD_CODE_TOTAL } KeyboardCode;

INTERNAL bool key_pressed  (KeyboardCode code);
INTERNAL bool key_released (KeyboardCode code);
INTERNAL bool key_down     (KeyboardCode code);
INTERNAL bool key_up       (KeyboardCode code);

#endif /* INPUT_H*/

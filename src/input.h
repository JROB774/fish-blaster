#ifndef INPUT_H
#define INPUT_H

INTERNAL void update_input_state ();

// MOUSE

typedef enum MouseButton__ { LMB, RMB, MOUSE_BUTTON_TOTAL } MouseButton;

INTERNAL bool button_pressed  (MouseButton button);
INTERNAL bool button_released (MouseButton button);
INTERNAL bool button_down     (MouseButton button);
INTERNAL bool button_up       (MouseButton button);

INTERNAL float get_mouse_x ();
INTERNAL float get_mouse_y ();

#endif /* INPUT_H*/

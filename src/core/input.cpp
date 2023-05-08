// #include <core/input.h>

// bool Input::is_key_down(int key) { return keys_down[key]; }

// bool Input::is_key_pressed(int key) { return keys_pressed[key]; }

// bool Input::is_key_released(int key) { return keys_released[key]; }

// bool Input::is_key_up(int key) { return !keys_down[key]; }

// bool Input::is_mouse_button_down(int button) {
//   return mouse_buttons_down[button];
// }

// bool Input::is_mouse_button_pressed(int button) {
//   return mouse_buttons_pressed[button];
// }

// bool Input::is_mouse_button_released(int button) {
//   return mouse_buttons_released[button];
// }

// bool Input::is_mouse_button_up(int button) {
//   return !mouse_buttons_down[button];
// }

// void Input::set_key_down(int key) {
//   keys_down[key] = true;
//   keys_pressed[key] = true;
// }

// void Input::set_key_up(int key) {
//   keys_down[key] = false;
//   keys_released[key] = true;
// }

// void Input::set_key_pressed(int key) { keys_pressed[key] = true; }

// void Input::set_key_released(int key) { keys_released[key] = true; }

// void Input::set_mouse_button_down(int button) {
//   mouse_buttons_down[button] = true;
//   mouse_buttons_pressed[button] = true;
// }

// void Input::set_mouse_button_up(int button) {
//   mouse_buttons_down[button] = false;
//   mouse_buttons_released[button] = true;
// }

// void Input::set_mouse_button_pressed(int button) {
//   mouse_buttons_pressed[button] = true;
// }

// void Input::set_mouse_button_released(int button) {
//   mouse_buttons_released[button] = true;
// }

// void Input::set_mouse_position(float x, float y) {
//   mouse_x = x;
//   mouse_y = y;
// }

// void Input::set_mouse_scroll(float x, float y) {
//   mouse_scroll_x = x;
//   mouse_scroll_y = y;
// }

// void Input::reset() {
//   for (int i = 0; i < 1024; i++) {
//     keys_down[i] = false;
//     keys_pressed[i] = false;
//     keys_released[i] = false;
//   }
//   for (int i = 0; i < 8; i++) {
//     mouse_buttons_down[i] = false;
//     mouse_buttons_pressed[i] = false;
//     mouse_buttons_released[i] = false;
//   }
//   mouse_scroll_x = 0;
//   mouse_scroll_y = 0;
//   mouse_x = 0;
//   mouse_y = 0;
// }
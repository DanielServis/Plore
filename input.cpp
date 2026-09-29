#include <iostream>
#include <GLFW/glfw3.h>

#include "input.hpp"

bool Input::keys[1024] = {false};
bool Input::mouse_buttons[8] = {false};

void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    if (action == GLFW_RELEASE)
        keys[key] = false;
}

void Input::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        mouse_buttons[button] = true;
    if (action == GLFW_RELEASE)
        mouse_buttons[button] = false;
}

bool Input::get_key_down(int key)
{
    return keys[key];
}

bool Input::get_mouse_button_down(int button)
{
    return mouse_buttons[button];
}
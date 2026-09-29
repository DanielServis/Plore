#pragma once

#include <GLFW/glfw3.h>

class Input
{
    private:
        static bool keys[1024];
        static bool mouse_buttons[8];

        Input() = delete;

    public:
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
        static bool get_key_down(int key);
        static bool get_mouse_button_down(int button);
};
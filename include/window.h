#ifndef WINDOW_H_
#define WINDOW_H_

#include "GLFW/glfw3.h"

struct Window 
{
    GLFWwindow *window;

    Window(int width, int height, bool visible, GLFWwindow *shared = NULL);

    operator GLFWwindow*();
};

#endif // WINDOW_H_

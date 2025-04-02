#pragma once

#include <GLFW/glfw3.h>

namespace Callbacks
{
   void Register(GLFWwindow* window);

   void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
   void MouseCursor(GLFWwindow* window, double x, double y);
   void MouseButton(GLFWwindow* window, int button, int action, int mods);
   void Resize(GLFWwindow* window, int width, int height);
};

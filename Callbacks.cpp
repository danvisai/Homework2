#include "Callbacks.h"
#include "Scene.h"
#include <glm/glm.hpp>

void Callbacks::Register(GLFWwindow* window)
{
   glfwSetKeyCallback(window, Keyboard);
   glfwSetCursorPosCallback(window, MouseCursor);
   glfwSetMouseButtonCallback(window, MouseButton);
   glfwSetFramebufferSizeCallback(window, Resize);
}

//This function gets called when a key is pressed
void Callbacks::Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //std::cout << "key : " << key << ", " << char(key) << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;

   if (action == GLFW_PRESS)
   {
      switch (key)
      {
      case 'r':
      case 'R':
         Scene::ReloadShader();
         break;

      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose(window, GLFW_TRUE);
         break;
      }
   }
}

//This function gets called when the mouse moves over the window.
void Callbacks::MouseCursor(GLFWwindow* window, double x, double y)
{
   //std::cout << "cursor pos: " << x << ", " << y << std::endl;
}

//This function gets called when a mouse button is pressed.
void Callbacks::MouseButton(GLFWwindow* window, int button, int action, int mods)
{
   //std::cout << "button : "<< button << ", action: " << action << ", mods: " << mods << std::endl;
}

void Callbacks::Resize(GLFWwindow* window, int width, int height)
{
   width = glm::max(1, width);
   height = glm::max(1, height);
   //Set viewport to cover entire framebuffer
   glViewport(0, 0, width, height);
   //Set aspect ratio used in view matrix calculation
   Scene::Camera::Aspect = float(width) / float(height);
   Scene::Camera::UpdateP();
}


#include <windows.h>
#include "Callbacks.h"
#include "Scene.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


//C++ programs start executing in the main() function.
int main(int argc, char **argv)
{
   GLFWwindow* window;

   /* Initialize the library */
   if (!glfwInit())
   {
      return -1;
   }

#ifdef _DEBUG
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

   /* Create a windowed mode window and its OpenGL context */
   const char* const window_title = PROJECT_NAME; //defined in project settings
   window = glfwCreateWindow(Scene::InitWindowWidth, Scene::InitWindowHeight, window_title, NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   //Register callback functions with glfw. 
   Callbacks::Register(window);

   /* Make the window's context current */
   glfwMakeContextCurrent(window);

   Scene::Init(window);
   
   //Init ImGui
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 150");

   /* Loop until the user closes the window */
   while (!glfwWindowShouldClose(window))
   {
      Scene::Idle();
      Scene::Display(window);

      /* Poll for and process events */
      glfwPollEvents();
   }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

   glfwTerminate();
   return 0;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main()
{
    //intializes glfw and gives context to the window...
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creates the pointer to the window with a size and name, other two parameters are for specifying which monitor display, and for "sharing" resources, whatever that means.
    GLFWwindow* window = glfwCreateWindow(800,600, "BenHurley2026", NULL, NULL);
    // if window fails to create, terminate.
    if (window == NULL)
    {
        std::cout << "FAILED TO CREATE GLFW WINDOW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    //sets the created window pointer to the current context to use.
    glfwMakeContextCurrent(window);
    //if glad isn't loaded correctly, terminate.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    // first two parameters are x and y for the left corner of the window, next two are viewport size of window.
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // while window doesn't need to close, poll events like mouse movement and keyboard presses and swap color buffers (make picture move)
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

    return 0;
}
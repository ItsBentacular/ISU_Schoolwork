#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const int SRC_WIDTH = 800;
const int SRC_HEIGHT = 600;

int main()
{
    //intializes glfw and gives context to the window...
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creates the pointer to the window with a size and name, other two parameters are for specifying which monitor display, and for "sharing" resources, whatever that means.
    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH,SRC_HEIGHT, "BenHurley2026", NULL, NULL);
    // if window fails to create, terminate.
    if (window == NULL)
    {
        std::cout << "FAILED TO CREATE GLFW WINDOW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    //sets the created window pointer to the current context to use.
    glfwMakeContextCurrent(window);
    // first two parameters are x and y for the left corner of the window, next two are viewport size of window.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //if glad isn't loaded correctly, terminate.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // while window doesn't need to close, poll events like mouse movement and keyboard presses and swap color buffers (make picture move)
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        //rendering commands go here, because we want it in between processing inputs and creating the image
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
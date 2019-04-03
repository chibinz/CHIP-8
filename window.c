#include "window.h"

GLFWwindow *initWindow(int width, int height, int scale)
{
    if (!glfwInit())
    {
        printf("Initialization failed\n");
        // exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width * scale, height * scale, "Test", NULL, NULL);
    if(window == NULL)
    {
        printf("Window creation failed\n");
        // exit(-1);
    }

    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    glfwSetKeyCallback(window, NULL);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        // exit(-1);
    }

    printf("%s\n", glGetString(GL_VERSION));
    return window;
}
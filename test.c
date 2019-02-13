#include <GLFW/glfw3.h>
#include <stdio.h>

int main(void)
{
    if (!glfwInit())
    {
        printf("Initialization failed");
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Test", NULL, NULL);

    glfwMakeContextCurrent(window);

    printf("%s", glGetString(GL_VERSION));

    while (!glfwWindowShouldClose(window))
    {
        glBegin(GL_TRIANGLES);

        glColor3f(1.0, 0.0, 0.0); // Red
        glVertex3f(0.0, 1.0, 0.0);

        glColor3f(0.0, 1.0, 0.0); // Green
        glVertex3f(-1.0, -1.0, 0.0);

        glColor3f(0.0, 0.0, 1.0); // Blue
        glVertex3f(1.0, -1.0, 0.0);

        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "cpu.h"
#include "ram.h"
#include "rom.h"
#include "interpret.h"
#include "disassemble.h"
#include "graphics.h"
#include "window.h"
#include "shader.h"

int main(int argc, char **argv)
{
    //check number of arguments
    if(argc != 2)
    {
        printf("Use: interpreter <rom>\n");
        exit(-1);
    }

    initCPU();
    initRAM();
    readROM(argv[1]);

    GLFWwindow *window = initWindow(WIDTH, HEIGHT, 20);
    int nVertices = numOfVertices();

    char *vertexShader = LoadShader("sprite.vert");
    char *fragmentShader = LoadShader("sprite.frag");
    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    printf("\n%s\n%s\n", vertexShader, fragmentShader);

    // vertex buffers must be generated and binded before vertex arrays
    initVertexBuffer();
    initVertexArray();
    projectScreen(shader);

    //call interpreter to interpret the rom;
    while(pc < rsize + 0x200 && !glfwWindowShouldClose(window))
    {
        interpret();

        drawVertices();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader); 

        glDrawArrays(GL_TRIANGLES, 0, nVertices);

        // printf("0x%x ", glGetError());

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    free(vertexShader);
    free(fragmentShader);

    glfwTerminate();

    return 0;
}

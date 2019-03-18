#include "graphics.h"

void clearScreen(void)
{
    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < HEIGHT; j++)
            screenBuffer[j * WIDTH + i] = 0;
}

int drawSprite(unsigned char x, unsigned char y, char n)
{
    int flag = 0;

    // printf("\n\n\n");
    for(int j = 0; j < n; j++)
    {
        unsigned char mask = 0b10000000;
        for(int k = 0; k < 8; k++)
        {
            // adjusted x, y 
            // pixels beyond screen size will be rotated

            unsigned char ax = (x + k) % WIDTH;
            unsigned char ay = (y + j) % HEIGHT;

            if(screenBuffer[ay * WIDTH + ax])
                flag = 1;

            // if(x + k > WIDTH)
            //     printf("Form width exceeded: x: %d k: %d x + k: %d\n", x, k, x + k);
            // if(y + j > HEIGHT)
            //     printf("Form length exceeded: y: %d j: %d y + j: %d\n", y, j, y + j);

            screenBuffer[ay * WIDTH + ax] = ram[i + j] & mask;
 
            mask >>= 1;
        }
    }

    // for(int j = 0; j < HEIGHT; j++, putchar('\n'))
    //     for(int k = 0; k < WIDTH; k++)
    //         if(screenBuffer[j * WIDTH + k])
    //             putchar('#');
    //         else
    //             putchar(' ');

    // printf("\n\n\n");

    return flag;
}

int numOfVertices(void)
{
    return WIDTH * HEIGHT * 2 * 3;
}

void drawVertices(void)
{
    // 2 triangle, 3 vertices, 3 attributes

    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
            if(screenBuffer[i * WIDTH + j])
                drawPixel(i, j, 1);
}

void drawPixel(int y, int x, int color)
{
    int row = y * WIDTH * STRIDE;
    int column = x * STRIDE;
    int offset = row + column;

    // x                            // y                            // color
    vertices[offset]     = x;       vertices[offset + 1] = y;       vertices[offset + 2] = color;
    vertices[offset + 3] = x + 1;   vertices[offset + 4] = y;       vertices[offset + 5] = color;
    vertices[offset + 6] = x + 1;   vertices[offset + 7] = y + 1;   vertices[offset + 8] = color;
    
    vertices[offset + 9]  = x;      vertices[offset + 10] = y;      vertices[offset + 11] = color;
    vertices[offset + 12] = x + 1;  vertices[offset + 13] = y + 1;  vertices[offset + 14] = color;
    vertices[offset + 15] = x;      vertices[offset + 16] = y + 1;  vertices[offset + 17] = color;
}

void initVertexBuffer(void)
{
    unsigned int vbo;
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void initVertexArray(void)
{
    unsigned int vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);    

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));

}

void projectScreen(unsigned int program)
{
    mat4 projection;
    glm_ortho(0, WIDTH, HEIGHT, 0, -1, 1, projection);

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

}
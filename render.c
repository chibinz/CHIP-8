#include "render.h" 

int numOfVertices(int width, int height)
{
    return width * height * 2 * 3;
}

int sizeOfArray(int width, int height)
{
    return sizeof(float) * width * height * 2 * 3 * 3;
}

float *createVertices(int width, int height, char **graph)
{
    // 2 triangle, 3 vertices, 3 attributes
    int stride = 2 * 3 * 3;
    float *vertices = malloc(sizeOfArray(width, height));

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            if(graph[i][j])
                drawPixel(i, j, 1, width, stride, vertices);

    return vertices;
}

void drawPixel(int y, int x, int color, int width, int stride, float *vertices)
{
    int row = y * width * stride;
    int column = x * stride;
    int offset = row + column;

    // x                            // y                            // color
    vertices[offset]     = x;       vertices[offset + 1] = y;       vertices[offset + 2] = color;
    vertices[offset + 3] = x + 1;   vertices[offset + 4] = y;       vertices[offset + 5] = color;
    vertices[offset + 6] = x + 1;   vertices[offset + 7] = y + 1;   vertices[offset + 8] = color;
    
    vertices[offset + 9]  = x;      vertices[offset + 10] = y;      vertices[offset + 11] = color;
    vertices[offset + 12] = x + 1;  vertices[offset + 13] = y + 1;  vertices[offset + 14] = color;
    vertices[offset + 15] = x;      vertices[offset + 16] = y + 1;  vertices[offset + 17] = color;
}

void initVertexBuffer(float *vertices)
{
    unsigned int vbo;
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeOfArray(WIDTH, HEIGHT), vertices, GL_STATIC_DRAW);

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
#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include "graphics.h"

float *vertices;

int numOfVertices(int width, int height);
int sizeOfArray(int width, int height);
float *createVertices(int width, int height, char **graph);
void drawPixel(int y, int x, int color, int width, int stride, float *vertices);

void initVertexBuffer(float *vertices);
void initVertexArray(void);
void projectScreen(unsigned int program);

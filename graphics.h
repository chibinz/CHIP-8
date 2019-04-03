#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include "cpu.h"
#include "ram.h"

#define WIDTH 64
#define HEIGHT 32
#define STRIDE 2 * 3 * 3

char screenBuffer[WIDTH * HEIGHT];
float vertices[WIDTH * HEIGHT * STRIDE];

void clearScreen(void);
int drawSprite(unsigned char x, unsigned char y, char n);

int numOfVertices(void);
void drawVertices(void);
void drawPixel(int y, int x, int color);

void initVertexBuffer();
void initVertexArray(void);
void projectScreen(unsigned int program);

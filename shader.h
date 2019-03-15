#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

unsigned int CompileShader(unsigned int type, const char *source);
int CreateShader(const char *vertexShader, const char *fragmentShader);
char * LoadShader(const char *shaderPath);
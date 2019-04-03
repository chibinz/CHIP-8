#include "shader.h"

unsigned int CompileShader(unsigned int type, const char *source)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    //error checking
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        printf("\n%s\n", message);
    }

    return id;
}

int CreateShader(const char *vertexShader, const char *fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

char * LoadShader(const char *shaderPath)
{
    FILE *sfile = fopen(shaderPath, "rb");
    long length = 0;

    fseek(sfile, 0, SEEK_END);
    length = ftell(sfile);
    fseek(sfile, 0, SEEK_SET);

    char *shader = malloc(sizeof(char) * (length + 1));
    fread(shader, 1, length, sfile);
    fclose(sfile);

    shader[length] ='\0';

    return shader;
}

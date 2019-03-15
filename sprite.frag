#version 400 core
out vec4 Color;

in float fcolor;

void main()
{
   Color = vec4(0, fcolor, 0, 1);
};
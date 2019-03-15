#version 400 core
layout(location = 0) in vec2 position;
layout(location = 1) in float vcolor;

out float fcolor;

uniform mat4 projection;

void main()
{
   gl_Position = projection * vec4(position, 0, 1);
   fcolor = vcolor;
};
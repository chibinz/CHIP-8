#include "keyboard.h"

int keymap(char key)
{
    switch(key)
    {
        case 0x01: return GLFW_KEY_1;
        case 0x02: return GLFW_KEY_2;
        case 0x03: return GLFW_KEY_3;
        case 0x04: return GLFW_KEY_Q;
        case 0x05: return GLFW_KEY_W;
        case 0x06: return GLFW_KEY_F;
        case 0x07: return GLFW_KEY_A;
        case 0x08: return GLFW_KEY_R;
        case 0x09: return GLFW_KEY_S;
        case 0x0A: return GLFW_KEY_Z;
        case 0x00: return GLFW_KEY_X;
        case 0x0B: return GLFW_KEY_C;
        case 0x0C: return GLFW_KEY_4;
        case 0x0D: return GLFW_KEY_P;
        case 0x0E: return GLFW_KEY_T;
        case 0x0F: return GLFW_KEY_V;
    }
}

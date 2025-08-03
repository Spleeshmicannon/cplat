#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef enum : uint32_t
{
    LEFT,
    RIGHT,
    MIDDLE,
    MAX_BUTTONS
}
CP_MOUSE_BUTTON;

typedef enum : uint32_t
{
    CP_KEY_BACKSPACE = 0x08,
    CP_KEY_ENTER = 0x0D,
    CP_KEY_TAB = 0x09,
    CP_KEY_SHIFT = 0x10,
    CP_KEY_CONTROL = 0x11,

    CP_KEY_PAUSE = 0x13,
    CP_KEY_CAPITAL = 0x14,

    CP_KEY_ESCAPE = 0x1B,

    CP_KEY_CONVERT = 0x1C,
    CP_KEY_NONCONVERT = 0x1D,
    CP_KEY_ACCEPT = 0x1E,
    CP_KEY_MODECHANGE = 0x1F,

    CP_KEY_SPACE = 0x20,
    CP_KEY_PRIOR = 0x21,
    CP_KEY_NEXT = 0x22,
    CP_KEY_END = 0x23,
    CP_KEY_HOME = 0x24,
    CP_KEY_LEFT = 0x25,
    CP_KEY_UP = 0x26,
    CP_KEY_RIGHT = 0x27,
    CP_KEY_DOWN = 0x28,
    CP_KEY_SELECT = 0x29,
    CP_KEY_PRINT = 0x2A,
    CP_KEY_EXECUTE = 0x2B,
    CP_KEY_SNAPSHOT = 0x2C,
    CP_KEY_INSERT = 0x2D,
    CP_KEY_TDELETE = 0x2E,
    CP_KEY_HELP = 0x2F,

    CP_KEY_A = 0x41,
    CP_KEY_B = 0x42,
    CP_KEY_C = 0x43,
    CP_KEY_D = 0x44,
    CP_KEY_E = 0x45,
    CP_KEY_F = 0x46,
    CP_KEY_G = 0x47,
    CP_KEY_H = 0x48,
    CP_KEY_I = 0x49,
    CP_KEY_J = 0x4A,
    CP_KEY_K = 0x4B,
    CP_KEY_L = 0x4C,
    CP_KEY_M = 0x4D,
    CP_KEY_N = 0x4E,
    CP_KEY_O = 0x4F,
    CP_KEY_P = 0x50,
    CP_KEY_Q = 0x51,
    CP_KEY_R = 0x52,
    CP_KEY_S = 0x53,
    CP_KEY_T = 0x54,
    CP_KEY_U = 0x55,
    CP_KEY_V = 0x56,
    CP_KEY_W = 0x57,
    CP_KEY_X = 0x58,
    CP_KEY_Y = 0x59,
    CP_KEY_Z = 0x5A,

    CP_KEY_LWIN = 0x5B,
    CP_KEY_RWIN = 0x5C,
    CP_KEY_APPS = 0x5D,

    CP_KEY_SLEEP = 0x5F,

    CP_KEY_NUMPAD0 = 0x60,
    CP_KEY_NUMPAD1 = 0x61,
    CP_KEY_NUMPAD2 = 0x62,
    CP_KEY_NUMPAD3 = 0x63,
    CP_KEY_NUMPAD4 = 0x64,
    CP_KEY_NUMPAD5 = 0x65,
    CP_KEY_NUMPAD6 = 0x66,
    CP_KEY_NUMPAD7 = 0x67,
    CP_KEY_NUMPAD8 = 0x68,
    CP_KEY_NUMPAD9 = 0x69,
    CP_KEY_MULTIPLY = 0x6A,
    CP_KEY_ADD = 0x6B,
    CP_KEY_SEPARATOR = 0x6C,
    CP_KEY_SUBTRACT = 0x6D,
    CP_KEY_DECIMAL = 0x6E,
    CP_KEY_DIVIDE = 0x6F,

    CP_KEY_F1 = 0x70,
    CP_KEY_F2 = 0x71,
    CP_KEY_F3 = 0x72,
    CP_KEY_F4 = 0x73,
    CP_KEY_F5 = 0x74,
    CP_KEY_F6 = 0x75,
    CP_KEY_F7 = 0x76,
    CP_KEY_F8 = 0x77,
    CP_KEY_F9 = 0x78,
    CP_KEY_F10 = 0x79,
    CP_KEY_F11 = 0x7A,
    CP_KEY_F12 = 0x7B,
    CP_KEY_F13 = 0x7C,
    CP_KEY_F14 = 0x7D,
    CP_KEY_F15 = 0x7E,
    CP_KEY_F16 = 0x7F,
    CP_KEY_F17 = 0x80,
    CP_KEY_F18 = 0x81,
    CP_KEY_F19 = 0x82,
    CP_KEY_F20 = 0x83,
    CP_KEY_F21 = 0x84,
    CP_KEY_F22 = 0x85,
    CP_KEY_F23 = 0x86,
    CP_KEY_F24 = 0x87,

    CP_KEY_NUMLOCK = 0x90,
    CP_KEY_SCROLL = 0x91,

    CP_KEY_NUMPAD_EQUAL = 0x92,

    CP_KEY_LSHIFT = 0xA0,
    CP_KEY_RSHIFT = 0xA1,
    CP_KEY_LCONTROL = 0xA2,
    CP_KEY_RCONTROL = 0xA3,
    CP_KEY_LALT = 0xA4,
    CP_KEY_RALT = 0xA5,

    CP_KEY_SEMICOLON = 0xBA,
    CP_KEY_PLUS = 0xBB,
    CP_KEY_COMMA = 0xBC,
    CP_KEY_MINUS = 0xBD,
    CP_KEY_PERIOD = 0xBE,
    CP_KEY_SLASH = 0xBF,
    CP_KEY_GRAVE = 0xC0,

    CP_KEY_MAX_KEYS = 0xFF
}
CP_KEY;

#endif // INPUT_H

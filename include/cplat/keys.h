/* zlib license
 * Copyright (C) 2025 J. Benson
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/


#ifndef INPUT_H
#define INPUT_H

#include "logger.h"

#include <stdint.h>

typedef enum : uint32_t
{
    CP_KEY_BACKSPACE = 0x08,
    CP_KEY_ENTER = 0x0D,
    CP_KEY_TAB = 0x09,

    CP_KEY_PAUSE = 0x13,
    CP_KEY_CAPITAL = 0x14,

    CP_KEY_ESCAPE = 0x1B,

    CP_KEY_CONVERT = 0x1C,
    CP_KEY_NONCONVERT = 0x1D,
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

    CP_KEY_0 = 0x30,
    CP_KEY_1 = 0x31,
    CP_KEY_2 = 0x32,
    CP_KEY_3 = 0x33,
    CP_KEY_4 = 0x34,
    CP_KEY_5 = 0x35,
    CP_KEY_6 = 0x36,
    CP_KEY_7 = 0x37,
    CP_KEY_8 = 0x38,
    CP_KEY_9 = 0x39,

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
    CP_KEY_FORWARD_SLASH = 0xBF,
    CP_KEY_GRAVE = 0xC0,

    CP_KEY_BACK_SLASH = 0xdc,
    CP_KEY_APOSTRAPHE = 0xde,

    CP_KEY_OPEN_BRACKET = 0xdb,
    CP_KEY_CLOSE_BRACKET = 0xdd,

    CP_KEY_BEGIN = 0x0c,

    CP_KEY_MAX_KEYS = 0xFF
}
CP_KEY;

static inline const char* CP_keyToString(CP_KEY key)
{
    switch(key)
    {
        case CP_KEY_BACKSPACE: return "CP_KEY_BACKSPACE";
        case CP_KEY_ENTER: return "CP_KEY_ENTER";
        case CP_KEY_TAB: return "CP_KEY_TAB";
        case CP_KEY_PAUSE: return "CP_KEY_PAUSE";
        case CP_KEY_CAPITAL: return "CP_KEY_CAPITAL";
        case CP_KEY_ESCAPE: return "CP_KEY_ESCAPE";
        case CP_KEY_CONVERT: return "CP_KEY_CONVERT";
        case CP_KEY_NONCONVERT: return "CP_KEY_NONCONVERT";
        case CP_KEY_MODECHANGE: return "CP_KEY_MODECHANGE";
        case CP_KEY_SPACE: return "CP_KEY_SPACE";
        case CP_KEY_PRIOR: return "CP_KEY_PRIOR";
        case CP_KEY_NEXT: return "CP_KEY_NEXT";
        case CP_KEY_END: return "CP_KEY_END";
        case CP_KEY_HOME: return "CP_KEY_HOME";
        case CP_KEY_LEFT: return "CP_KEY_LEFT";
        case CP_KEY_UP: return "CP_KEY_UP";
        case CP_KEY_RIGHT: return "CP_KEY_RIGHT";
        case CP_KEY_DOWN: return "CP_KEY_DOWN";
        case CP_KEY_SELECT: return "CP_KEY_SELECT";
        case CP_KEY_PRINT: return "CP_KEY_PRINT";
        case CP_KEY_EXECUTE: return "CP_KEY_EXECUTE";
        case CP_KEY_INSERT: return "CP_KEY_INSERT";
        case CP_KEY_TDELETE: return "CP_KEY_TDELETE";
        case CP_KEY_HELP: return "CP_KEY_HELP";
        case CP_KEY_A: return "CP_KEY_A";
        case CP_KEY_B: return "CP_KEY_B";
        case CP_KEY_C: return "CP_KEY_C";
        case CP_KEY_D: return "CP_KEY_D";
        case CP_KEY_E: return "CP_KEY_E";
        case CP_KEY_F: return "CP_KEY_F";
        case CP_KEY_G: return "CP_KEY_G";
        case CP_KEY_H: return "CP_KEY_H";
        case CP_KEY_I: return "CP_KEY_I";
        case CP_KEY_J: return "CP_KEY_J";
        case CP_KEY_K: return "CP_KEY_K";
        case CP_KEY_L: return "CP_KEY_L";
        case CP_KEY_M: return "CP_KEY_M";
        case CP_KEY_N: return "CP_KEY_N";
        case CP_KEY_O: return "CP_KEY_O";
        case CP_KEY_P: return "CP_KEY_P";
        case CP_KEY_Q: return "CP_KEY_Q";
        case CP_KEY_R: return "CP_KEY_R";
        case CP_KEY_S: return "CP_KEY_S";
        case CP_KEY_T: return "CP_KEY_T";
        case CP_KEY_U: return "CP_KEY_U";
        case CP_KEY_V: return "CP_KEY_V";
        case CP_KEY_W: return "CP_KEY_W";
        case CP_KEY_X: return "CP_KEY_X";
        case CP_KEY_Y: return "CP_KEY_Y";
        case CP_KEY_Z: return "CP_KEY_Z";
        case CP_KEY_LWIN: return "CP_KEY_LWIN";
        case CP_KEY_RWIN: return "CP_KEY_RWIN";
        case CP_KEY_APPS: return "CP_KEY_APPS";
        case CP_KEY_SLEEP: return "CP_KEY_SLEEP";
        case CP_KEY_NUMPAD0: return "CP_KEY_NUMPAD0";
        case CP_KEY_NUMPAD1: return "CP_KEY_NUMPAD1";
        case CP_KEY_NUMPAD2: return "CP_KEY_NUMPAD2";
        case CP_KEY_NUMPAD3: return "CP_KEY_NUMPAD3";
        case CP_KEY_NUMPAD4: return "CP_KEY_NUMPAD4";
        case CP_KEY_NUMPAD5: return "CP_KEY_NUMPAD5";
        case CP_KEY_NUMPAD6: return "CP_KEY_NUMPAD6";
        case CP_KEY_NUMPAD7: return "CP_KEY_NUMPAD7";
        case CP_KEY_NUMPAD8: return "CP_KEY_NUMPAD8";
        case CP_KEY_NUMPAD9: return "CP_KEY_NUMPAD9";
        case CP_KEY_0: return "CP_KEY_0";
        case CP_KEY_1: return "CP_KEY_1";
        case CP_KEY_2: return "CP_KEY_2";
        case CP_KEY_3: return "CP_KEY_3";
        case CP_KEY_4: return "CP_KEY_4";
        case CP_KEY_5: return "CP_KEY_5";
        case CP_KEY_6: return "CP_KEY_6";
        case CP_KEY_7: return "CP_KEY_7";
        case CP_KEY_8: return "CP_KEY_8";
        case CP_KEY_9: return "CP_KEY_9";
        case CP_KEY_MULTIPLY: return "CP_KEY_MULTIPLY";
        case CP_KEY_ADD: return "CP_KEY_ADD";
        case CP_KEY_SEPARATOR: return "CP_KEY_SEPARATOR";
        case CP_KEY_SUBTRACT: return "CP_KEY_SUBTRACT";
        case CP_KEY_DECIMAL: return "CP_KEY_DECIMAL";
        case CP_KEY_DIVIDE: return "CP_KEY_DIVIDE";
        case CP_KEY_F1: return "CP_KEY_F1";
        case CP_KEY_F2: return "CP_KEY_F2";
        case CP_KEY_F3: return "CP_KEY_F3";
        case CP_KEY_F4: return "CP_KEY_F4";
        case CP_KEY_F5: return "CP_KEY_F5";
        case CP_KEY_F6: return "CP_KEY_F6";
        case CP_KEY_F7: return "CP_KEY_F7";
        case CP_KEY_F8: return "CP_KEY_F8";
        case CP_KEY_F9: return "CP_KEY_F9";
        case CP_KEY_F10: return "CP_KEY_F10";
        case CP_KEY_F11: return "CP_KEY_F11";
        case CP_KEY_F12: return "CP_KEY_F12";
        case CP_KEY_F13: return "CP_KEY_F13";
        case CP_KEY_F14: return "CP_KEY_F14";
        case CP_KEY_F15: return "CP_KEY_F15";
        case CP_KEY_F16: return "CP_KEY_F16";
        case CP_KEY_F17: return "CP_KEY_F17";
        case CP_KEY_F18: return "CP_KEY_F18";
        case CP_KEY_F19: return "CP_KEY_F19";
        case CP_KEY_F20: return "CP_KEY_F20";
        case CP_KEY_F21: return "CP_KEY_F21";
        case CP_KEY_F22: return "CP_KEY_F22";
        case CP_KEY_F23: return "CP_KEY_F23";
        case CP_KEY_F24: return "CP_KEY_F24";
        case CP_KEY_NUMLOCK: return "CP_KEY_NUMLOCK";
        case CP_KEY_SCROLL: return "CP_KEY_SCROLL";
        case CP_KEY_NUMPAD_EQUAL: return "CP_KEY_NUMPAD_EQUAL";
        case CP_KEY_LSHIFT: return "CP_KEY_LSHIFT";
        case CP_KEY_RSHIFT: return "CP_KEY_RSHIFT";
        case CP_KEY_LCONTROL: return "CP_KEY_LCONTROL";
        case CP_KEY_RCONTROL: return "CP_KEY_RCONTROL";
        case CP_KEY_LALT: return "CP_KEY_LALT";
        case CP_KEY_RALT: return "CP_KEY_RALT";
        case CP_KEY_SEMICOLON: return "CP_KEY_SEMICOLON";
        case CP_KEY_PLUS: return "CP_KEY_PLUS";
        case CP_KEY_COMMA: return "CP_KEY_COMMA";
        case CP_KEY_MINUS: return "CP_KEY_MINUS";
        case CP_KEY_PERIOD: return "CP_KEY_PERIOD";
        case CP_KEY_FORWARD_SLASH: return "CP_KEY_FORWARD_SLASH";
        case CP_KEY_BACK_SLASH: return "CP_KEY_BACK_SLASH";
        case CP_KEY_GRAVE: return "CP_KEY_GRAVE";
        case CP_KEY_OPEN_BRACKET: return "CP_KEY_OPEN_BRACKET";
        case CP_KEY_CLOSE_BRACKET: return "CP_KEY_CLOSE_BRACKET";
        case CP_KEY_APOSTRAPHE: return "CP_KEY_APOSTRAPHE";
        case CP_KEY_BEGIN: return "CP_KEY_BEGIN";
        case CP_KEY_MAX_KEYS: return "CP_KEY_MAX_KEYS";
        default: 
            return "";
    }
}



#endif // INPUT_H

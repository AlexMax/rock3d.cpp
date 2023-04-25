/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

/**
 * This header uses significant amounts of code (scancodes and key codes)
 * from SDL, please see LICENSE-3RD-PARTY.txt for licensing info.
 */

#pragma once

namespace rock3d
{

/**
 * @brief Keyboard scan codes, based on SDL's scancode index which was in
 *        turn based on the official USB HID keyboard scan codes.
 *
 * @sa https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
 */
enum keyboardScan_e
{
    KSCAN_NONE = 0,
    KSCAN_A = 4,
    KSCAN_B = 5,
    KSCAN_C = 6,
    KSCAN_D = 7,
    KSCAN_E = 8,
    KSCAN_F = 9,
    KSCAN_G = 10,
    KSCAN_H = 11,
    KSCAN_I = 12,
    KSCAN_J = 13,
    KSCAN_K = 14,
    KSCAN_L = 15,
    KSCAN_M = 16,
    KSCAN_N = 17,
    KSCAN_O = 18,
    KSCAN_P = 19,
    KSCAN_Q = 20,
    KSCAN_R = 21,
    KSCAN_S = 22,
    KSCAN_T = 23,
    KSCAN_U = 24,
    KSCAN_V = 25,
    KSCAN_W = 26,
    KSCAN_X = 27,
    KSCAN_Y = 28,
    KSCAN_Z = 29,
    KSCAN_1 = 30,
    KSCAN_2 = 31,
    KSCAN_3 = 32,
    KSCAN_4 = 33,
    KSCAN_5 = 34,
    KSCAN_6 = 35,
    KSCAN_7 = 36,
    KSCAN_8 = 37,
    KSCAN_9 = 38,
    KSCAN_0 = 39,
    KSCAN_RETURN = 40,
    KSCAN_ESCAPE = 41,
    KSCAN_BACKSPACE = 42,
    KSCAN_TAB = 43,
    KSCAN_SPACE = 44,
    KSCAN_MINUS = 45,
    KSCAN_EQUALS = 46,
    KSCAN_LEFTBRACKET = 47,
    KSCAN_RIGHTBRACKET = 48,
    KSCAN_BACKSLASH = 49,
    KSCAN_NONUSHASH = 50, // ISO version of KSCAN_BACKSLASH, rarely used.
    KSCAN_SEMICOLON = 51,
    KSCAN_APOSTROPHE = 52,
    KSCAN_GRAVE = 53,
    KSCAN_COMMA = 54,
    KSCAN_PERIOD = 55,
    KSCAN_SLASH = 56,
    KSCAN_CAPSLOCK = 57,
    KSCAN_F1 = 58,
    KSCAN_F2 = 59,
    KSCAN_F3 = 60,
    KSCAN_F4 = 61,
    KSCAN_F5 = 62,
    KSCAN_F6 = 63,
    KSCAN_F7 = 64,
    KSCAN_F8 = 65,
    KSCAN_F9 = 66,
    KSCAN_F10 = 67,
    KSCAN_F11 = 68,
    KSCAN_F12 = 69,
    KSCAN_PRINTSCREEN = 70,
    KSCAN_SCROLLLOCK = 71,
    KSCAN_PAUSE = 72,
    KSCAN_INSERT = 73,
    KSCAN_HOME = 74,
    KSCAN_PAGEUP = 75,
    KSCAN_DELETE = 76,
    KSCAN_END = 77,
    KSCAN_PAGEDOWN = 78,
    KSCAN_RIGHT = 79,
    KSCAN_LEFT = 80,
    KSCAN_DOWN = 81,
    KSCAN_UP = 82,
    KSCAN_NUMLOCKCLEAR = 83,
    KSCAN_KP_DIVIDE = 84,
    KSCAN_KP_MULTIPLY = 85,
    KSCAN_KP_MINUS = 86,
    KSCAN_KP_PLUS = 87,
    KSCAN_KP_ENTER = 88,
    KSCAN_KP_1 = 89,
    KSCAN_KP_2 = 90,
    KSCAN_KP_3 = 91,
    KSCAN_KP_4 = 92,
    KSCAN_KP_5 = 93,
    KSCAN_KP_6 = 94,
    KSCAN_KP_7 = 95,
    KSCAN_KP_8 = 96,
    KSCAN_KP_9 = 97,
    KSCAN_KP_0 = 98,
    KSCAN_KP_PERIOD = 99,
    KSCAN_NONUSBACKSLASH = 100, // Special key on ISO keyboards next to Left Shift.
    KSCAN_APPLICATION = 101,    // Menu/Compose key, usually next to Right Winkey.
    KSCAN_KP_EQUALS = 103,
    KSCAN_F13 = 104,
    KSCAN_F14 = 105,
    KSCAN_F15 = 106,
    KSCAN_F16 = 107,
    KSCAN_F17 = 108,
    KSCAN_F18 = 109,
    KSCAN_F19 = 110,
    KSCAN_F20 = 111,
    KSCAN_F21 = 112,
    KSCAN_F22 = 113,
    KSCAN_F23 = 114,
    KSCAN_F24 = 115,
    KSCAN_LCTRL = 224,
    KSCAN_LSHIFT = 225,
    KSCAN_LALT = 226, // Alt, Option.
    KSCAN_LGUI = 227, // Winkey, Command, Meta.
    KSCAN_RCTRL = 228,
    KSCAN_RSHIFT = 229,
    KSCAN_RALT = 230, // Alt Gr, Option
    KSCAN_RGUI = 231, // Winkey, Command, Meta.
};

constexpr int KSCAN_MAX = 512;

namespace detail
{
template <typename T>
constexpr T ScanToKey(const rock3d::keyboardScan_e eKey)
{
    return T(int(eKey) | (1 << 30));
}
} // namespace detail

enum keyboardKey_e
{
    KKEY_UNKNOWN = 0,
    KKEY_RETURN = '\r',
    KKEY_ESCAPE = '\x1B',
    KKEY_BACKSPACE = '\b',
    KKEY_TAB = '\t',
    KKEY_SPACE = ' ',
    KKEY_EXCLAIM = '!',
    KKEY_QUOTEDBL = '"',
    KKEY_HASH = '#',
    KKEY_PERCENT = '%',
    KKEY_DOLLAR = '$',
    KKEY_AMPERSAND = '&',
    KKEY_QUOTE = '\'',
    KKEY_LEFTPAREN = '(',
    KKEY_RIGHTPAREN = ')',
    KKEY_ASTERISK = '*',
    KKEY_PLUS = '+',
    KKEY_COMMA = ',',
    KKEY_MINUS = '-',
    KKEY_PERIOD = '.',
    KKEY_SLASH = '/',
    KKEY_0 = '0',
    KKEY_1 = '1',
    KKEY_2 = '2',
    KKEY_3 = '3',
    KKEY_4 = '4',
    KKEY_5 = '5',
    KKEY_6 = '6',
    KKEY_7 = '7',
    KKEY_8 = '8',
    KKEY_9 = '9',
    KKEY_COLON = ':',
    KKEY_SEMICOLON = ';',
    KKEY_LESS = '<',
    KKEY_EQUALS = '=',
    KKEY_GREATER = '>',
    KKEY_QUESTION = '?',
    KKEY_AT = '@',
    KKEY_LEFTBRACKET = '[',
    KKEY_BACKSLASH = '\\',
    KKEY_RIGHTBRACKET = ']',
    KKEY_CARET = '^',
    KKEY_UNDERSCORE = '_',
    KKEY_BACKQUOTE = '`',
    KKEY_DELETE = '\x7F',
    KKEY_a = 'a',
    KKEY_b = 'b',
    KKEY_c = 'c',
    KKEY_d = 'd',
    KKEY_e = 'e',
    KKEY_f = 'f',
    KKEY_g = 'g',
    KKEY_h = 'h',
    KKEY_i = 'i',
    KKEY_j = 'j',
    KKEY_k = 'k',
    KKEY_l = 'l',
    KKEY_m = 'm',
    KKEY_n = 'n',
    KKEY_o = 'o',
    KKEY_p = 'p',
    KKEY_q = 'q',
    KKEY_r = 'r',
    KKEY_s = 's',
    KKEY_t = 't',
    KKEY_u = 'u',
    KKEY_v = 'v',
    KKEY_w = 'w',
    KKEY_x = 'x',
    KKEY_y = 'y',
    KKEY_z = 'z',

    // Scancode conversions.

    KKEY_CAPSLOCK = detail::ScanToKey<keyboardKey_e>(KSCAN_CAPSLOCK),
    KKEY_F1 = detail::ScanToKey<keyboardKey_e>(KSCAN_F1),
    KKEY_F2 = detail::ScanToKey<keyboardKey_e>(KSCAN_F2),
    KKEY_F3 = detail::ScanToKey<keyboardKey_e>(KSCAN_F3),
    KKEY_F4 = detail::ScanToKey<keyboardKey_e>(KSCAN_F4),
    KKEY_F5 = detail::ScanToKey<keyboardKey_e>(KSCAN_F5),
    KKEY_F6 = detail::ScanToKey<keyboardKey_e>(KSCAN_F6),
    KKEY_F7 = detail::ScanToKey<keyboardKey_e>(KSCAN_F7),
    KKEY_F8 = detail::ScanToKey<keyboardKey_e>(KSCAN_F8),
    KKEY_F9 = detail::ScanToKey<keyboardKey_e>(KSCAN_F9),
    KKEY_F10 = detail::ScanToKey<keyboardKey_e>(KSCAN_F10),
    KKEY_F11 = detail::ScanToKey<keyboardKey_e>(KSCAN_F11),
    KKEY_F12 = detail::ScanToKey<keyboardKey_e>(KSCAN_F12),
    KKEY_PRINTSCREEN = detail::ScanToKey<keyboardKey_e>(KSCAN_PRINTSCREEN),
    KKEY_SCROLLLOCK = detail::ScanToKey<keyboardKey_e>(KSCAN_SCROLLLOCK),
    KKEY_PAUSE = detail::ScanToKey<keyboardKey_e>(KSCAN_PAUSE),
    KKEY_INSERT = detail::ScanToKey<keyboardKey_e>(KSCAN_INSERT),
    KKEY_HOME = detail::ScanToKey<keyboardKey_e>(KSCAN_HOME),
    KKEY_PAGEUP = detail::ScanToKey<keyboardKey_e>(KSCAN_PAGEUP),
    KKEY_END = detail::ScanToKey<keyboardKey_e>(KSCAN_END),
    KKEY_PAGEDOWN = detail::ScanToKey<keyboardKey_e>(KSCAN_PAGEDOWN),
    KKEY_RIGHT = detail::ScanToKey<keyboardKey_e>(KSCAN_RIGHT),
    KKEY_LEFT = detail::ScanToKey<keyboardKey_e>(KSCAN_LEFT),
    KKEY_DOWN = detail::ScanToKey<keyboardKey_e>(KSCAN_DOWN),
    KKEY_UP = detail::ScanToKey<keyboardKey_e>(KSCAN_UP),
    KKEY_NUMLOCKCLEAR = detail::ScanToKey<keyboardKey_e>(KSCAN_NUMLOCKCLEAR),
    KKEY_KP_DIVIDE = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_DIVIDE),
    KKEY_KP_MULTIPLY = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_MULTIPLY),
    KKEY_KP_MINUS = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_MINUS),
    KKEY_KP_PLUS = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_PLUS),
    KKEY_KP_ENTER = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_ENTER),
    KKEY_KP_1 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_1),
    KKEY_KP_2 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_2),
    KKEY_KP_3 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_3),
    KKEY_KP_4 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_4),
    KKEY_KP_5 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_5),
    KKEY_KP_6 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_6),
    KKEY_KP_7 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_7),
    KKEY_KP_8 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_8),
    KKEY_KP_9 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_9),
    KKEY_KP_0 = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_0),
    KKEY_KP_PERIOD = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_PERIOD),
    KKEY_APPLICATION = detail::ScanToKey<keyboardKey_e>(KSCAN_APPLICATION),
    KKEY_KP_EQUALS = detail::ScanToKey<keyboardKey_e>(KSCAN_KP_EQUALS),
    KKEY_F13 = detail::ScanToKey<keyboardKey_e>(KSCAN_F13),
    KKEY_F14 = detail::ScanToKey<keyboardKey_e>(KSCAN_F14),
    KKEY_F15 = detail::ScanToKey<keyboardKey_e>(KSCAN_F15),
    KKEY_F16 = detail::ScanToKey<keyboardKey_e>(KSCAN_F16),
    KKEY_F17 = detail::ScanToKey<keyboardKey_e>(KSCAN_F17),
    KKEY_F18 = detail::ScanToKey<keyboardKey_e>(KSCAN_F18),
    KKEY_F19 = detail::ScanToKey<keyboardKey_e>(KSCAN_F19),
    KKEY_F20 = detail::ScanToKey<keyboardKey_e>(KSCAN_F20),
    KKEY_F21 = detail::ScanToKey<keyboardKey_e>(KSCAN_F21),
    KKEY_F22 = detail::ScanToKey<keyboardKey_e>(KSCAN_F22),
    KKEY_F23 = detail::ScanToKey<keyboardKey_e>(KSCAN_F23),
    KKEY_F24 = detail::ScanToKey<keyboardKey_e>(KSCAN_F24),
    KKEY_LCTRL = detail::ScanToKey<keyboardKey_e>(KSCAN_LCTRL),
    KKEY_LSHIFT = detail::ScanToKey<keyboardKey_e>(KSCAN_LSHIFT),
    KKEY_LALT = detail::ScanToKey<keyboardKey_e>(KSCAN_LALT),
    KKEY_LGUI = detail::ScanToKey<keyboardKey_e>(KSCAN_LGUI),
    KKEY_RCTRL = detail::ScanToKey<keyboardKey_e>(KSCAN_RCTRL),
    KKEY_RSHIFT = detail::ScanToKey<keyboardKey_e>(KSCAN_RSHIFT),
    KKEY_RALT = detail::ScanToKey<keyboardKey_e>(KSCAN_RALT),
    KKEY_RGUI = detail::ScanToKey<keyboardKey_e>(KSCAN_RGUI),
};

/**
 * @brief Mouse buttons.
 */
enum mouseButton_e
{
    MBTN_NONE = 0,
    MBTN_LEFT,
    MBTN_RIGHT,
    MBTN_MIDDLE,
    MBTN_X1,
    MBTN_X2,
};

constexpr int MBTN_MAX = int(MBTN_X2) + 1;

/**
 * @brief Gamepad axis - enumerated according to GameControllerDB standard.
 */
enum padAxis_e
{
    PADAXIS_INVALID = -1,
    PADAXIS_LEFTX,
    PADAXIS_LEFTY,
    PADAXIS_RIGHTX,
    PADAXIS_RIGHTY,
    PADAXIS_TRIGGERLEFT,
    PADAXIS_TRIGGERRIGHT,
};

constexpr int PADAXIS_MAX = int(PADAXIS_TRIGGERRIGHT) + 1;

/**
 * @brief Gamepad buttons - enumerated according to GameControllerDB standard.
 */
enum padButton_e
{
    PADBTN_INVALID = -1,
    PADBTN_FACE_DOWN,  // Xbox A, Nintendo B, Sony Cross
    PADBTN_FACE_RIGHT, // Xbox B, Nintendo A, Sony Circle
    PADBTN_FACE_LEFT,  // Xbox X, Nintendo Y, Sony Square
    PADBTN_FACE_TOP,   // Xbox Y, Nintendo X, Sony Triangle
    PADBTN_BACK,
    PADBTN_GUIDE,
    PADBTN_START,
    PADBTN_LEFTSTICK,     // PS L3
    PADBTN_RIGHTSTICK,    // PS R3
    PADBTN_LEFTSHOULDER,  // Xbox Left Bumper, Nintendo L, Sony L1
    PADBTN_RIGHTSHOULDER, // Xbox Right Bumper, Nintendo R, Sony R1
    PADBTN_DPAD_UP,
    PADBTN_DPAD_DOWN,
    PADBTN_DPAD_LEFT,
    PADBTN_DPAD_RIGHT,
    PADBTN_MISC1,            // Xbox Series Share, PS5 Mic, NX Capture
    PADBTN_PADDLE1,          // Xbox Elite Paddle
    PADBTN_PADDLE2,          // Xbox Elite Paddle
    PADBTN_PADDLE3,          // Xbox Elite Paddle
    PADBTN_PADDLE4,          // Xbox Elite Paddle
    PADBTN_TOUCHPAD,         // PS4/PS5 Touchpad Press
    PADBTN_LEFTTRIGGER = 22, // Xbox Left Trigger, Nintendo ZL, Sony L2 (digital)
    PADBTN_RIGHTTRIGGER,     // Xbox Right Trigger, Nintendo ZR, Sony R2 (digital)
};

/**
 * @brief PADBTN values beyond this one are synthesized from axis inputs and aren't standard.
 */
constexpr int PADBTN_NONSTD = int(PADBTN_TOUCHPAD) + 1;
constexpr int PADBTN_MAX = int(PADBTN_RIGHTTRIGGER) + 1;

struct eventKey_s
{
    keyboardScan_e scan = KSCAN_NONE;
    keyboardKey_e key = KKEY_UNKNOWN;
    bool pressed = false;
};

struct eventMouseMotion_s
{
    glm::ivec2 abs{0, 0};
    glm::ivec2 rel{0, 0};
};

struct eventMouseButton_s
{
    glm::ivec2 abs{0, 0};
    mouseButton_e button = MBTN_NONE;
    bool pressed = false;
};

struct eventPadAxis_s
{
    uint8_t axis = 0;
    int16_t value = 0;
};

struct eventPadButton_s
{
    uint8_t button = 0;
    bool pressed = false;
};

using event_t = std::variant<eventKey_s, eventMouseMotion_s, eventMouseButton_s, eventPadAxis_s, eventPadButton_s>;

class EventQueue
{
  public:
    EventQueue() {}
    virtual ~EventQueue() {}
    ROCK3D_NOCOPY(EventQueue);

    virtual auto Queue(event_t &&cEvent) -> void = 0;
    virtual auto Poll(event_t &cOutEvent) -> bool = 0;
};

auto GetEventQueue() -> EventQueue &;

} // namespace rock3d

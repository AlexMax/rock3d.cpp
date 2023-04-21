/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
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

struct keyboardKey_s
{
    keyboardScan_e scan = KSCAN_NONE;
    bool pressed = false;
};

struct mouseMotion_s
{
    glm::ivec2 abs{0, 0};
    glm::ivec2 rel{0, 0};
};

struct mouseButton_s
{
    glm::ivec2 abs{0, 0};
    mouseButton_e button = MBTN_NONE;
};

struct padAxis_s
{
    uint8_t axis = 0;
    int16_t value = 0;
};

struct padButton_s
{
    uint8_t button = 0;
    bool pressed = false;
};

using rockEvent_t = std::variant<keyboardKey_s, mouseMotion_s, mouseButton_s, padAxis_s, padButton_s>;

class EventQueue
{
  public:
    ROCK3D_NOCOPY(EventQueue);

    EventQueue()
    {
    }

    virtual ~EventQueue()
    {
    }

    virtual auto Queue(rockEvent_t &&cEvent) -> void = 0;
    virtual auto Poll(rockEvent_t &cOutEvent) -> bool = 0;
};

auto GetEventQueue() -> EventQueue &;

} // namespace rock3d

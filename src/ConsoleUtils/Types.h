#pragma once
#include <windows.h>

typedef char* MenuOption;

typedef enum {
  UNHANDLED_EVENT,
  KEY_DOWN_EVENT,
  KEY_UP_EVENT
} EventType;

typedef enum {
  TEXT_STYLE_DEFAULT = 0x0,
  TEXT_STYLE_BOLD = 0x1,
  TEXT_STYLE_GRAYED = 0x2,
  TEXT_STYLE_ITALIC = 0x4,
  TEXT_STYLE_UNDERLINED = 0x8,
  TEXT_STYLE_BLINKING = 0x10,
  TEXT_STYLE_INVERTED = 0x20,
  TEXT_STYLE_STRIKETHROUGH = 0x40,
  TEXT_STYLE_DOUBLE_UNDERLINED = 0x80
} Style;

typedef enum {
  TEXT_COLOR_DEFAULT = 39,
  TEXT_COLOR_BLACK = 30,
  TEXT_COLOR_RED = 31,
  TEXT_COLOR_GREEN = 32,
  TEXT_COLOR_YELLOW = 33,
  TEXT_COLOR_BLUE = 34,
  TEXT_COLOR_MAGENTA = 35,
  TEXT_COLOR_CYAN = 36,
  TEXT_COLOR_WHITE = 37,
  TEXT_COLOR_MOD_BRIGHT = 60
} TextColor;


typedef struct {
  int key;
  char asChar;
  bool ctrl;
  bool shift;
  bool alt;
  bool capsLock;
} KeyCode;

typedef struct {
  union {
    KeyCode keyCode;
  } params;
  EventType eventType;
} Event;

typedef struct {
  Style style;
  TextColor textColor;
  TextColor backgroundColor;
} TextStyle;

typedef struct {
  char* title;
  MenuOption* options;
  size_t optionsSize;
  size_t selectedOption;
  TextStyle style;
  TextStyle titleStyle;
  TextStyle selectedStyle;
} Menu;

typedef struct {
  DWORD input;
  DWORD output;
} Originals;

typedef void (*DisplayCode)(void);
typedef bool (*InputCode)(int, const MenuOption);
typedef bool (*EventCode)(Event*);

#pragma once
#include <windows.h>

typedef char* MenuOption;

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
  int eventType;
} Event;

typedef struct {
  int textColor;
  int backgroundColor;
  int style;
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

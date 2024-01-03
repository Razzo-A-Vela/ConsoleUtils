#include <windows.h>

typedef int bool;

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
  const char** options;
  size_t optionsSize;
  size_t selectedOption;
  TextStyle style;
  TextStyle selectedStyle;
} Menu;

typedef struct {
  DWORD input;
  DWORD output;
} Originals;

#pragma once
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "Constants.h"
#include "Types.h"


typedef void (*DisplayCode)(void);
typedef bool (*InputCode)(int, const char*);
typedef bool (*EventCode)(Event*);

#define clearScreen() system("cls")
#define pauseScreen() system("pause")
void setRawMode(Originals* original);
void resetRawMode(Originals* original);
POINT getCursorPos();
void setCursorPos(int x, int y);
void moveCursor(int dx, int dy);
//! Must be used at the beginning of main()
bool isOwnConsole();
void getInput(Event* ret);
void resetEvent(Event* event);
POINT getConsoleSize();
bool isChar(KeyCode code);
void setStyle(int style);
void setTextColor(int color);
void setBackgroundColor(int color);
void setColor(int textColor, int backgroundColor);
void setTextStyle(TextStyle textStyle);
//! if console starts with non default colors it will reset to default colors (to fix)
void resetTextStyle();
int printfWithStyle(TextStyle style, const char* format, ...);
bool handleCtrlQ(Event* event);
bool handleCursorWithArrows(Event* event);
void printMenu(Menu* menu);
bool handleMenuKeys(Event* event, Menu* menu);
bool getMenuSelection(Event* event, Menu* menu, int* selection);
void createMenu(Menu* ret, char** options, size_t optionsSize);
void menuLoop(Menu* menu, DisplayCode displayCode, InputCode inputCode, EventCode eventCode);
//! if console starts with non default colors it wont know (to fix)
TextStyle getTextStyle();
void toggleCursor(bool show);
#define showCursor() toggleCursor(true)
#define hideCursor() toggleCursor(false)

#ifdef __cplusplus
}
#endif

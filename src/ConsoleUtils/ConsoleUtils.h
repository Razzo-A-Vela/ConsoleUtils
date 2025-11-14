#pragma once
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "Constants.h"
#include "Types.h"

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
void setStyle(Style style);
void setTextColor(TextColor color);
void setBackgroundColor(TextColor color);
void setColor(TextColor textColor, TextColor backgroundColor);
void setTextStyle(TextStyle textStyle);
//! if console starts with non default colors it will reset to default colors (to fix)
void resetTextStyle();
int printfWithStyle(TextStyle style, const char* format, ...);
bool handleCtrlQ(Event* event);
bool handleCursorWithArrows(Event* event);
void printMenu(Menu* menu);
bool handleMenuKeys(Event* event, Menu* menu);
bool getMenuSelection(Event* event, Menu* menu, int* selection);
void _createMenu(Menu* ret, MenuOption* options, size_t optionsSize);
void menuLoop(Menu* menu, DisplayCode displayCode, InputCode inputCode, EventCode eventCode);
//! if console starts with non default colors it wont know (to fix)
TextStyle getTextStyle();
void toggleCursor(bool show);
#define showCursor() toggleCursor(true)
#define hideCursor() toggleCursor(false)
#define createMenu(ret, options) _createMenu(&ret, options, sizeof(options) / sizeof(options[0]))
#define createMenuWithTitle(ret, _title, options) createMenu(ret, options); ret.title = _title
RECT printBoxWithTitleAndStyle(const char* title, TextStyle titleStyle, int xSize, int ySize);
#define printBoxWithTitle(title, xSize, ySize) printBoxWithTitleAndStyle(title, getTextStyle(), xSize, ySize)
#define printBox(xSize, ySize) printBoxWithTitle("", xSize, ySize)
void printInBox(RECT box, const char* toPrint);
void printfInBox(RECT box, const char* format, ...);
TextStyle defaultTextStyle();

#ifdef __cplusplus
}
#endif

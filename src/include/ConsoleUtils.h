#include <windows.h>
#include <stdio.h>

#include "Constants.h"
#include "Types.h"

#define xor(x, y) (!(x && y) && (x || y))
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
void resetTextStyle();
int printfWithStyle(TextStyle style, const char* format, ...);
bool handleCtrlQ(Event* event);
bool handleCursorWithArrows(Event* event);
void printMenu(Menu* menu);
bool handleMenuKeys(Event* event, Menu* menu);
bool getMenuSelection(Event* event, Menu* menu, int* selection);
void createMenu(Menu* ret, char** options, size_t optionsSize);
void menuLoop(Menu* menu, void(*displayCode)(), bool(*inputCode)(int, const char*), bool(*eventCode)(Event*));

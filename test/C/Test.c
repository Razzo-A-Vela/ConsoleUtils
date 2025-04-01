#include <stdio.h>
#include <ConsoleUtils.h>


void cls() {
  clearScreen();
}

bool mainMenuInput(int selection, const char* option);
bool subMenuInput(int selection, const char* option);
bool inputMenuEvent(Event* event);

Menu mainMenu;
Menu subMenu;
Menu inputMenu;
char inputStr[16];
int cursor;


int main() {
  Originals original;
  setRawMode(&original);

  char* mainMenuOptions[] = {"", "Sub Menu", "", "Exit"};
  char* subMenuOptions[] = {inputStr, "", "Input", "Back"};
  char* inputMenuOptions[] = {inputStr};
  createMenu(&mainMenu, mainMenuOptions, sizeof(mainMenuOptions) / sizeof(char*));
  mainMenu.title = "Main menu";
  createMenu(&subMenu, subMenuOptions, sizeof(subMenuOptions) / sizeof(char*));
  createMenu(&inputMenu, inputMenuOptions, sizeof(inputMenuOptions) / sizeof(char*));

  menuLoop(&mainMenu, cls, mainMenuInput, NULL);

  resetRawMode(&original);
  clearScreen();
  return 0;
}


bool mainMenuInput(int selection, const char* option) {
  if (option == "Exit") return true;
  else if (option == "Sub Menu")
    menuLoop(&subMenu, cls, subMenuInput, NULL);

  return false;
}

bool subMenuInput(int selection, const char* option) {
  if (option == "Back") return true;
  else if (option == "Input") {
    cursor = 0;
    for (int i = 0; i < sizeof(inputStr) / sizeof(char); i++) inputStr[i] = 0;
    menuLoop(&inputMenu, cls, NULL, inputMenuEvent);
  }
  
  return false;
}

bool inputMenuEvent(Event* event) {
  if (event->eventType != KEY_DOWN_EVENT) return false;
  KeyCode code = event->params.keyCode;
  if (code.key == VK_RETURN) return true;
  
  if (code.key == VK_BACK && cursor != 0)
    inputStr[--cursor] = '\0';

  if (isChar(code) && cursor != sizeof(inputStr) / sizeof(char) - 2)
    inputStr[cursor++] = code.asChar;

  return false;
}


/*
#include <stdio.h>
#include <ConsoleUtils.h>

int main() {
  Originals original;
  setRawMode(&original);

  Menu menu;
  char* options[] = {"", "Option A", "Option B", "Option C", "", "Exit"};
  createMenu(&menu, options, sizeof(options) / sizeof(char*));
  menu.style.textColor = TEXT_COLOR_BLACK;
  menu.style.backgroundColor = TEXT_COLOR_WHITE;
  menu.selectedStyle.textColor = TEXT_COLOR_RED;
  menu.selectedStyle.backgroundColor = TEXT_COLOR_WHITE + TEXT_COLOR_MOD_BRIGHT;
  menu.title = "Main menu";
  menu.titleStyle = menu.style;

  Event event;
  while (true) {
    setBackgroundColor(TEXT_COLOR_BLUE);
    clearScreen();
    printMenu(&menu);

    getInput(&event);
    if (!getMenuSelection(&event, &menu, NULL)) continue;
    char* option = menu.options[menu.selectedOption];
    if (option == "Exit") break;

    clearScreen();
    setCursorPos(0, 0);
    printf("%s\n", option);
    pauseScreen();
  }

  resetRawMode(&original);
  return 0;
}
*/

/*
#include <stdio.h>
#include <ConsoleUtils.h>

int main() {
  Originals original;
  setRawMode(&original);

  TextStyle style;
  style.backgroundColor = TEXT_COLOR_RED + TEXT_COLOR_MOD_BRIGHT;
  style.textColor = TEXT_COLOR_WHITE + TEXT_COLOR_MOD_BRIGHT;
  style.style = TEXT_STYLE_BLINKING | TEXT_STYLE_UNDERLINED;
  printf("Explosion n: ");
  printfWithStyle(style, "%4d", 1003);
  printf(";");

  Event event;
  while (true) {
    setStyle(TEXT_STYLE_BLINKING);
    setColor(TEXT_COLOR_BLUE + TEXT_COLOR_MOD_BRIGHT, TEXT_COLOR_BLACK);
    getInput(&event);

    if (handleCtrlQ(&event)) break;
    if (handleCursorWithArrows(&event)) continue;
    if (event.eventType != KEY_DOWN_EVENT) continue;
    KeyCode code = event.params.keyCode;

    if (code.key == VK_BACK) {
      if (getCursorPos().x == 0) continue;
      resetTextStyle();
      
      moveCursor(-1, 0);
      putchar(' ');
      moveCursor(-1, 0);
      continue;
    }

    if (code.key == VK_RETURN) {
      setCursorPos(0, getCursorPos().y + 1);
      continue;
    }
    
    if (isChar(code)) putchar(code.asChar);
  }

  resetRawMode(&original);
  return 0;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>

int main() {
  for (int i = 0; i < 11; i++) {
    for (int j = 0; j < 10; j++) {
      int n = 10 * i + j;
      if (n > 108) break;
      
      printf("\033[%dm %3d\033[m", n, n);
    }
    printf("\n");
  }
  
  system("pause");
  return 0;
}
*/

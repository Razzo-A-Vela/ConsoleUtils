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
      resetStyle();
      
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

int main(void)
{
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

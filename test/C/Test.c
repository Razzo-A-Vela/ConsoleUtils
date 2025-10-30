// TextViewer
#include <stdio.h>
#include <ConsoleUtils.h>

float explorerPerc = ((float) 1) / ((float) 4);
char* selectedFilePath;
int selectedFileIndex = 0;
int cursorPos = 0;

int explorerLine() {
  POINT consoleSize = getConsoleSize();

  int x = explorerPerc * ((float) consoleSize.x);
  for (int y = 0; y < consoleSize.y; y++) {
    setCursorPos(x, y);
    putchar(VK_PIPE_UP_DOWN);
  }

  return x;
}

void printTxts() {
  WIN32_FIND_DATA findData;
  HANDLE hFind = NULL;
  int index = 0;
  selectedFilePath[0] = '\0';

  char dir[4096];
  GetCurrentDirectory(4096, dir);

  if (selectedFileIndex < 0)
    selectedFileIndex = 0;

  char path[4096];
  sprintf(path, "%s\\%s", dir, "*.txt");
  hFind = FindFirstFile(path, &findData);
  do {
    setCursorPos(0, index);
    if (index == selectedFileIndex) {
      printf("[X]");
      strcpy(selectedFilePath, findData.cFileName);
    } else
      printf("[ ]");

    printf("  %s", findData.cFileName);
    index++;
  } while (FindNextFile(hFind, &findData));
  if (selectedFileIndex >= index) {
    selectedFileIndex = index - 1;
    strcpy(selectedFilePath, findData.cFileName);
  }
  FindClose(hFind);
}

void renderFile(int x) {
  if (strcmp(selectedFilePath, "") == 0)
    return;

  if (cursorPos < 0)
    cursorPos = 0;
  
  POINT cursorPosVec = {0, 0};
  int line = 0;
  setCursorPos(x, line++);
  char c;
  int index = 0;
  FILE* file = fopen(selectedFilePath, "r");
  printf("%d ", line);
  cursorPosVec.x = 0;
  cursorPosVec.y = 0;
  while ((c = fgetc(file)) != EOF) {
    if (index == cursorPos) {
      cursorPosVec = getCursorPos();
      cursorPosVec.x -= x;
    }
    if (c == '\n') {
      setCursorPos(x, line++);
      printf("%d ", line);
    } else
      printf("%c", c);
    index++;
  }
  fclose(file);

  if (cursorPos >= index)
    cursorPos = index;
  else
    setCursorPos(cursorPosVec.x + x, cursorPosVec.y);
}


Menu exitMenu;
bool wantsExit = false;

bool exitMenuInput(int option, const MenuOption optionStr) {
  if (option == 1)
    wantsExit = true;
  return true;
}

int main() {
  Originals originals;
  setRawMode(&originals);
  clearScreen();
  selectedFilePath = malloc(4096 * sizeof(char)); //? sizeof(char) == 1

  TextStyle tempStyle;
  MenuOption exitMenuOptions[] = {"", "Yes", "No"};
  createMenu(&exitMenu, exitMenuOptions);
  exitMenu.title = "Are you sure?";
  tempStyle.style = TEXT_STYLE_UNDERLINED;
  tempStyle.backgroundColor = TEXT_COLOR_DEFAULT;
  tempStyle.textColor = TEXT_COLOR_DEFAULT;
  exitMenu.selectedStyle = tempStyle;

  Event event;
  while (true) {
    // render
    clearScreen();
    printTxts();
    renderFile(explorerLine() + 1);

    // tick
    getInput(&event);
    if (handleCtrlQ(&event)) {
      menuLoop(&exitMenu, NULL, exitMenuInput, NULL);

      if (wantsExit)
        break;
    } else if (event.eventType == KEY_DOWN_EVENT) {
      KeyCode c = event.params.keyCode;
      if (c.key == VK_UP)
        selectedFileIndex--;
      else if (c.key == VK_DOWN)
        selectedFileIndex++;
      else if (c.key == VK_LEFT)
        cursorPos--;
      else if (c.key == VK_RIGHT)
        cursorPos++;
    }
  }

  resetRawMode(&originals);
  clearScreen();
  return 0;
}


/*
#include <stdio.h>
#include <ConsoleUtils.h>

void renderColors(int xSize, int ySize, int colors[xSize][ySize]) {
  POINT size = getConsoleSize();
  float densityX = (float) size.x / (float) xSize;
  float densityY = (float) size.y / (float) ySize;

  TextStyle style;
  style.style = TEXT_STYLE_DEFAULT;
  style.textColor = TEXT_COLOR_DEFAULT;
  for (int x = 0; x < xSize; x++) {
    for (int y = 0; y < ySize; y++) {
      for (int xD = 0; xD < densityX; xD++) {
        for (int yD = 0; yD < densityY; yD++) {
          style.backgroundColor = colors[x][y];

          setCursorPos((int) ((float) x * densityX + (float) xD), (int) ((float) y * densityY + (float) yD));
          printfWithStyle(style, " ");
        }
      }
    }
  }
}

int main() {
  Originals original;
  setRawMode(&original);
  clearScreen();
  hideCursor();

  #define X_SIZE 16
  #define Y_SIZE 16
  int colors[X_SIZE][Y_SIZE];
  for (int x = 0; x < X_SIZE; x++) {
    for (int y = 0; y < Y_SIZE; y++) {
      int perc = (int) ((float) x / ((float) X_SIZE / 3.0f) + 0.1f);
      if (perc == 0) {
        colors[x][y] = TEXT_COLOR_GREEN + TEXT_COLOR_MOD_BRIGHT;
      } else if (perc == 1) {
        colors[x][y] = TEXT_COLOR_WHITE + TEXT_COLOR_MOD_BRIGHT;
      } else {
        colors[x][y] = TEXT_COLOR_RED + TEXT_COLOR_MOD_BRIGHT;
      }
    }
  }


  #define RENDER_COOLDOWN 5
  int renderCooldown = 0;
  Event event;
  while (true) {
    if (renderCooldown == 0) {
      renderColors(X_SIZE, Y_SIZE, colors);
      renderCooldown = RENDER_COOLDOWN;
    } else
      renderCooldown--;
    
    getInput(&event);
    if (handleCtrlQ(&event))
      break;
    else if (event.eventType == KEY_DOWN_EVENT && event.params.keyCode.key == VK_SPACE)
      renderCooldown = 0;
  }

  showCursor();
  resetRawMode(&original);
  clearScreen();
  return 0;
}
*/


/*
#include <stdio.h>
#include <ConsoleUtils.h>


void cls() {
  clearScreen();
}

bool mainMenuInput(int selection, const MenuOption option);
bool subMenuInput(int selection, const MenuOption option);
bool inputMenuEvent(Event* event);

Menu mainMenu;
Menu subMenu;
Menu inputMenu;
char inputStr[16];
int cursor;


int main() {
  Originals original;
  setRawMode(&original);

  MenuOption mainMenuOptions[] = {"", "Sub Menu", "", "Exit"};
  MenuOption subMenuOptions[] = {inputStr, "", "Input", "Back"};
  MenuOption inputMenuOptions[] = {inputStr};
  createMenu(&mainMenu, mainMenuOptions);
  mainMenu.title = "Main menu";
  createMenu(&subMenu, subMenuOptions);
  createMenu(&inputMenu, inputMenuOptions);

  menuLoop(&mainMenu, cls, mainMenuInput, NULL);

  resetRawMode(&original);
  clearScreen();
  return 0;
}


bool mainMenuInput(int selection, const MenuOption option) {
  if (option == "Exit") return true;
  else if (option == "Sub Menu")
    menuLoop(&subMenu, cls, subMenuInput, NULL);

  return false;
}

bool subMenuInput(int selection, const MenuOption option) {
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
*/


/*
#include <stdio.h>
#include <ConsoleUtils.h>

int main() {
  Originals original;
  setRawMode(&original);

  Menu menu;
  MenuOption options[] = {"", "Option A", "Option B", "Option C", "", "Exit"};
  createMenu(&menu, options);
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
    MenuOption option = menu.options[menu.selectedOption];
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

#include <ConsoleUtils.h>


void setRawMode(Originals* original) {
  GetConsoleMode(stdInput, &original->input);
  GetConsoleMode(stdOutput, &original->output);

  SetConsoleMode(stdInput, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
  SetConsoleMode(stdOutput, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
  SetConsoleCtrlHandler(NULL, TRUE);
}

void resetRawMode(Originals* original) {
  SetConsoleMode(stdInput, original->input);
  SetConsoleMode(stdOutput, original->output);
  SetConsoleCtrlHandler(NULL, FALSE);
}

POINT getCursorPos() {
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(stdOutput, &info);

  POINT ret;
  ret.x = info.dwCursorPosition.X;
  ret.y = info.dwCursorPosition.Y;
  return ret;
}

void setCursorPos(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(stdOutput, coord);
}

void moveCursor(int dx, int dy) {
  POINT pos = getCursorPos();
  setCursorPos(pos.x + dx, pos.y + dy);
}

bool isOwnConsole() {
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(stdOutput, &info);
  
  return !(info.dwCursorPosition.X) && !(info.dwCursorPosition.Y);
}

void resetEvent(Event* event) {
  event->eventType = UNHANDLED_EVENT;
  event->params.keyCode.alt = false;
  event->params.keyCode.capsLock = false;
  event->params.keyCode.ctrl = false;
  event->params.keyCode.shift = false;
  event->params.keyCode.key = 0;
  event->params.keyCode.asChar = 0;
}

void getInput(Event* ret) {
  resetEvent(ret);
  INPUT_RECORD event;
  DWORD numEvent;
  ReadConsoleInput(stdInput, &event, 1, &numEvent);
  
  switch (event.EventType) {
    case KEY_EVENT :
      KEY_EVENT_RECORD keyEvent = event.Event.KeyEvent;
      if (keyEvent.bKeyDown) ret->eventType = KEY_DOWN_EVENT;
      else ret->eventType = KEY_UP_EVENT;
      ret->params.keyCode.key = keyEvent.wVirtualKeyCode;
      ret->params.keyCode.asChar = keyEvent.uChar.AsciiChar;

      if (keyEvent.dwControlKeyState & SHIFT_PRESSED) ret->params.keyCode.shift = true;
      if (keyEvent.dwControlKeyState & CAPSLOCK_ON) ret->params.keyCode.capsLock = true;
      if (keyEvent.dwControlKeyState & LEFT_ALT_PRESSED ||
          keyEvent.dwControlKeyState & RIGHT_ALT_PRESSED) ret->params.keyCode.alt = true;
      if (keyEvent.dwControlKeyState & LEFT_CTRL_PRESSED ||
          keyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED) ret->params.keyCode.ctrl = true;
      break;
  }
}

POINT getConsoleSize() {
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(stdOutput, &info);
  
  POINT ret;
  ret.x = info.dwSize.X;
  ret.y = info.dwSize.Y;
  return ret;
}

bool isChar(KeyCode code) {
  const char specials[] = "\\|!\"£$%&/()=?^'ìè+òàù,.-é*ç°§;:_@#[]{}€<>";
  const int specialsLen = sizeof(specials) / sizeof(char); //? sizeof(char) = 1
  
  if ((code.key >= VK_A && code.key <= VK_Z) || code.key == VK_SPACE) return true;
  if (code.asChar >= '0' && code.asChar <= '9') return true;
  for (int i = 0; i < specialsLen; i++)
    if (code.asChar == specials[i]) return true;

  return false;
}

void setStyle(int style) {
  if (style == TEXT_STYLE_DEFAULT) printf("\033[m");

  if (style & TEXT_STYLE_BOLD) printf("\033[1m");
  if (style & TEXT_STYLE_GRAYED) printf("\033[2m");
  if (style & TEXT_STYLE_ITALIC) printf("\033[3m");
  if (style & TEXT_STYLE_UNDERLINED) printf("\033[4m");
  if (style & TEXT_STYLE_BLINKING) printf("\033[5m");
  if (style & TEXT_STYLE_INVERTED) printf("\033[7m");
  if (style & TEXT_STYLE_STRIKETHROUGH) printf("\033[9m");
  if (style & TEXT_STYLE_DOUBLE_UNDERLINED) printf("\033[21m");
}

void setTextColor(int color) {
  printf("\033[%dm", color);
}

void setBackgroundColor(int color) {
  printf("\033[%dm", color + 10);
}

void setColor(int textColor, int backgroundColor) {
  setTextColor(textColor);
  setBackgroundColor(backgroundColor);
}

void setTextStyle(TextStyle textStyle) {
  setStyle(textStyle.style);
  setColor(textStyle.textColor, textStyle.backgroundColor);
}

void resetTextStyle() {
  setStyle(TEXT_STYLE_DEFAULT);
  setColor(TEXT_COLOR_DEFAULT, TEXT_COLOR_DEFAULT);
}

int printfWithStyle(TextStyle style, const char* format, ...) {
  int ret;
  setTextStyle(style);

  va_list args;
  va_start(args, format);
  ret = vprintf(format, args);
  va_end(args);

  resetTextStyle();
  return ret;
}

bool handleCtrlQ(Event* event) {
  return event->eventType == KEY_DOWN_EVENT && event->params.keyCode.ctrl && event->params.keyCode.key == VK_Q;
}

bool handleCursorWithArrows(Event* event) {
  if (event->eventType != KEY_DOWN_EVENT) return false;
  KeyCode keyCode = event->params.keyCode;

  switch (keyCode.key) {
    case VK_UP :
      moveCursor(0, -1);
      break;
    case VK_RIGHT :
      moveCursor(1, 0);
      break;
    case VK_DOWN :
      moveCursor(0, 1);
      break;
    case VK_LEFT :
      moveCursor(-1, 0);
      break;
    
    default :
      return false;
  }

  return true;
}

void printMenu(Menu* menu) {
  const POINT consoleSize = getConsoleSize();
  const int upSpace = consoleSize.y / 2 - menu->optionsSize;
  const bool hasTitle = menu->title != "";

  int maxWidth = 0;
  for (int i = 0; i < menu->optionsSize; i++) {
    int width = strlen(menu->options[i]);
    if (width > maxWidth) maxWidth = width;
  }

  int titleWidth;
  if (hasTitle) {
    titleWidth = strlen(menu->title) + 2;
    if (titleWidth > maxWidth) maxWidth = titleWidth;
  }
  const int leftSpace = consoleSize.x / 2 - maxWidth;
  maxWidth += 2;


  setCursorPos(leftSpace, upSpace);
  setTextStyle(menu->style);

  putchar(VK_PIPE_DOWN_RIGHT);
  for (int i = 0; i < maxWidth; i++) putchar(VK_PIPE_LEFT_RIGHT);
  putchar(VK_PIPE_DOWN_LEFT);
  
  if (hasTitle) {
    setCursorPos(leftSpace + (maxWidth / 2) - (titleWidth / 2), upSpace);
    setTextStyle(menu->titleStyle);
    printf("%c %s %c", VK_PIPE_UP_DOWN_LEFT, menu->title, VK_PIPE_UP_DOWN_RIGHT);
    setTextStyle(menu->style);
  }

  for (int i = 0; i < menu->optionsSize; i++) {
    const bool isSelected = i == menu->selectedOption;
    const int width = strlen(menu->options[i]);
    setCursorPos(leftSpace, upSpace + i + 1);

    putchar(VK_PIPE_UP_DOWN);
    for (int j = 0; j < (maxWidth - width) / 2; j++) printf(" ");

    if (isSelected) setTextStyle(menu->selectedStyle);
    printf(menu->options[i]);
    if (isSelected) setTextStyle(menu->style);

    int jj = (maxWidth - width) / 2;
    for (int j = 0; j < (((maxWidth - width) % 2 == 0) ? jj : jj + 1); j++) printf(" ");
    putchar(VK_PIPE_UP_DOWN);
  }

  setCursorPos(leftSpace, upSpace + menu->optionsSize + 1);
  putchar(VK_PIPE_UP_RIGHT);
  for (int i = 0; i < maxWidth; i++) putchar(VK_PIPE_LEFT_RIGHT);
  putchar(VK_PIPE_UP_LEFT);

  setCursorPos(0, 0);
  resetTextStyle();
}

bool handleMenuKeys(Event* event, Menu* menu) {
  if (event->eventType != KEY_DOWN_EVENT) return false;
  int currKey = event->params.keyCode.key;

  switch (currKey) {
    case VK_UP :
      if (menu->selectedOption != 0) menu->selectedOption--;
      break;
    
    case VK_DOWN :
      if (menu->selectedOption != menu->optionsSize - 1) menu->selectedOption++;
      break;

    case VK_RETURN :
      break;
    
    default :
      return false;
  }
  
  return true;
}

bool getMenuSelection(Event* event, Menu* menu, int* selection) {
  if (!handleMenuKeys(event, menu)) return false;
  if (event->params.keyCode.key != VK_RETURN) return false;
  
  *selection = menu->selectedOption;
  return true;
}

void createMenu(Menu* ret, char** options, size_t optionsSize) {
  ret->title = "";
  ret->selectedOption = 0;
  ret->options = options;
  ret->optionsSize = optionsSize;


  TextStyle defaultStyle;
  defaultStyle.backgroundColor = TEXT_COLOR_DEFAULT;
  defaultStyle.textColor = TEXT_COLOR_DEFAULT;
  defaultStyle.style = TEXT_STYLE_DEFAULT;
  ret->style = defaultStyle;
  ret->titleStyle = defaultStyle;

  defaultStyle.style = TEXT_STYLE_INVERTED;
  ret->selectedStyle = defaultStyle;
}

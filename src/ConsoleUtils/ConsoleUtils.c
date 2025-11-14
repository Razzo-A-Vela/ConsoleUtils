#include "ConsoleUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

static TextStyle currentTextStyle = { TEXT_STYLE_DEFAULT, TEXT_COLOR_DEFAULT, TEXT_COLOR_DEFAULT };

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
  resetTextStyle();
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
    
    case WINDOW_BUFFER_SIZE_EVENT :
      GetNumberOfConsoleInputEvents(stdInput, &numEvent);
      if (numEvent > 0) {
        do {
          PeekConsoleInput(stdInput, &event, 1, &numEvent);
          if (event.EventType == WINDOW_BUFFER_SIZE_EVENT)
            ReadConsoleInput(stdInput, &event, 1, &numEvent);
          GetNumberOfConsoleInputEvents(stdInput, &numEvent);
        } while (numEvent > 0 && event.EventType == WINDOW_BUFFER_SIZE_EVENT);
      }

      WINDOW_BUFFER_SIZE_RECORD windowEvent = event.Event.WindowBufferSizeEvent;
      ret->eventType = RESIZE_EVENT;
      ret->params.windowSize.x = windowEvent.dwSize.X;
      ret->params.windowSize.y = windowEvent.dwSize.Y;
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
  const int specialsLen = strlen(specials);
  
  if ((code.key >= VK_A && code.key <= VK_Z) || (code.key >= VK_0 && code.key <= VK_9) || code.key == VK_SPACE)
    return true;
  
  for (int i = 0; i < specialsLen; i++) {
    if (code.asChar == specials[i])
      return true;
  }

  return false;
}

void setStyle(Style style) {
  currentTextStyle.style = style;
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

void setTextColor(TextColor color) {
  currentTextStyle.textColor = color;
  printf("\033[%dm", color);
}

void setBackgroundColor(TextColor color) {
  currentTextStyle.backgroundColor = color;
  printf("\033[%dm", color + 10);
}

void setColor(TextColor textColor, TextColor backgroundColor) {
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
  const TextStyle previousTextStyle = getTextStyle();
  setTextStyle(style);

  va_list args;
  va_start(args, format);
  ret = vprintf(format, args);
  va_end(args);

  setTextStyle(previousTextStyle);
  return ret;
}

bool handleCtrlQ(Event* event) {
  return event->eventType == KEY_DOWN_EVENT && event->params.keyCode.ctrl && event->params.keyCode.key == VK_Q;
}

bool handleCursorWithArrows(Event* event) {
  if (event->eventType != KEY_DOWN_EVENT)
    return false;
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
  const TextStyle previousTextStyle = getTextStyle();
  setTextStyle(menu->style);

  int xSize = 0;
  for (int i = 0; i < menu->optionsSize; i++) {
    int len = strlen(menu->options[i]);
    if (len > xSize)
      xSize = len;
  }
  
  RECT box = printBoxWithTitleAndStyle(menu->title, menu->titleStyle, xSize + 2, menu->optionsSize + 2);

  int boxX = box.left;
  int boxY = box.top;
  int boxXSize = box.right - box.left;
  int boxYSize = box.bottom - box.top;
  box.left++;
  box.top++;
  box.right--;
  box.bottom--;

  for (int i = 0; i < menu->optionsSize; i++) {
    const bool isSelected = i == menu->selectedOption;
    const int width = strlen(menu->options[i]);
    
    if (isSelected)
      setTextStyle(menu->selectedStyle);
    
    box.left += (boxXSize - 2 - width + 1) / 2;
    printInBox(box, menu->options[i]);
    box.left -= (boxXSize - 2 - width + 1) / 2;
    
    if (isSelected)
      setTextStyle(menu->style);
    box.top++;
  }

  setCursorPos(0, 0);
  setTextStyle(previousTextStyle);
}

bool handleMenuKeys(Event* event, Menu* menu) {
  if (event->eventType != KEY_DOWN_EVENT)
    return false;
  int currKey = event->params.keyCode.key;

  switch (currKey) {
    case VK_UP :
      for (int i = menu->selectedOption - 1; i >= 0; i--) {
        if (menu->options[i][0] != '\0') {
          menu->selectedOption = i;
          break;
        }
      }
      break;
    
    case VK_DOWN :
      for (int i = menu->selectedOption + 1; i < menu->optionsSize; i++) {
        if (menu->options[i][0] != '\0') {
          menu->selectedOption = i;
          break;
        }
      }
      break;

    case VK_RETURN :
      break;
    
    default :
      return false;
  }
  
  return true;
}

bool getMenuSelection(Event* event, Menu* menu, int* selection) {
  if (!handleMenuKeys(event, menu))
    return false;
  if (event->params.keyCode.key != VK_RETURN)
    return false;
  
  if (selection != NULL)
    *selection = menu->selectedOption;
  return true;
}

void _createMenu(Menu* ret, MenuOption* options, size_t optionsSize) {
  for (int i = 0; i < optionsSize; i++) {
    if (options[i][0] != '\0') {
      ret->selectedOption = i;
      break;
    }
  }

  ret->title = "";
  ret->options = options;
  ret->optionsSize = optionsSize;

  ret->style = defaultTextStyle();
  ret->titleStyle = defaultTextStyle();
  ret->selectedStyle = defaultTextStyle();
  ret->selectedStyle.style = TEXT_STYLE_INVERTED;
}

void menuLoop(Menu* menu, DisplayCode displayCode, InputCode inputCode, EventCode eventCode) {
  if (inputCode == NULL && eventCode == NULL) return;

  Event event;
  while (true) {
    if (displayCode != NULL) displayCode();
    printMenu(menu);

    getInput(&event);
    if (eventCode != NULL && eventCode(&event)) break;
    if (!getMenuSelection(&event, menu, NULL)) continue;
    if (inputCode != NULL && inputCode(menu->selectedOption, menu->options[menu->selectedOption])) break;
  }
}

TextStyle getTextStyle() {
  return currentTextStyle;
}

void toggleCursor(bool show) {
  CONSOLE_CURSOR_INFO info;
  GetConsoleCursorInfo(stdOutput, &info);
  info.bVisible = show;
  SetConsoleCursorInfo(stdOutput, &info);
}

RECT printBoxWithTitleAndStyle(const char* title, TextStyle titleStyle, int xSize, int ySize) {
  const POINT consoleSize = getConsoleSize();
  const bool hasTitle = title[0] != '\0';
  int maxWidth = xSize;

  int titleWidth;
  if (hasTitle) {
    titleWidth = strlen(title) + 4;
    if (titleWidth > maxWidth)
      maxWidth = titleWidth;
  }
  const int x = consoleSize.x / 2 - maxWidth / 2;
  const int y = consoleSize.y / 2 - ySize / 2;

  setCursorPos(x - 1, y - 1);
  putchar(VK_PIPE_DOWN_RIGHT);
  for (int i = 0; i < maxWidth; i++)
    putchar(VK_PIPE_LEFT_RIGHT);
  putchar(VK_PIPE_DOWN_LEFT);
  
  if (hasTitle) {
    setCursorPos(x + (maxWidth / 2) - (titleWidth / 2), y - 1);
    putchar(VK_PIPE_UP_DOWN_LEFT);

    const TextStyle previousTextStyle = getTextStyle();
    setTextStyle(titleStyle);
    printf(" %s ", title);    
    setTextStyle(previousTextStyle);
    
    putchar(VK_PIPE_UP_DOWN_RIGHT);
  }

  for (int i = 0; i < ySize ; i++) {
    setCursorPos(x - 1, y + i);
    putchar(VK_PIPE_UP_DOWN);
    moveCursor(maxWidth, 0);
    putchar(VK_PIPE_UP_DOWN);
  }

  setCursorPos(x - 1, y + ySize);
  putchar(VK_PIPE_UP_RIGHT);
  for (int i = 0; i < maxWidth; i++)
    putchar(VK_PIPE_LEFT_RIGHT);
  putchar(VK_PIPE_UP_LEFT);

  RECT ret = { x, y, x + maxWidth - 1, y + ySize - 1 };
  return ret;
}

void printInBox(RECT box, const char* toPrint) {
  int len = strlen(toPrint);
  int x = box.left;
  int y = box.top;
  setCursorPos(x, y);
  for (int i = 0; i < len; i++) {
    char c = toPrint[i];

    if (c == '\n') {
      x = box.left;
      y++;

      if (y > box.bottom)
        break;
      setCursorPos(x, y);

    } else if (c == '\r')
      x = box.left;
    else if (x <= box.right) {
      putchar(c);
      x++;
    }
  }
}

void printfInBox(RECT box, const char* format, ...) {
  char buff[4096];

  va_list args;
  va_start(args, format);
  vsprintf(buff, format, args);
  va_end(args);

  printInBox(box, buff);
}

TextStyle defaultTextStyle() {
  TextStyle ret = { TEXT_STYLE_DEFAULT, TEXT_COLOR_DEFAULT, TEXT_COLOR_DEFAULT };
  return ret;
}

#ifdef __cplusplus
}
#endif

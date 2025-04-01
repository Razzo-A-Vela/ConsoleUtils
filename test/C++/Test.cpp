#include <iostream>
#include <ConsoleUtils.h>

class Window {
public:
  Window() {
    setRawMode(&original);
    clearScreen();
  }

  void run() {
    Event event;
    do {
      getInput(&event);
    } while (mainLoop(&event));
  }

  ~Window() {
    resetRawMode(&original);
    clearScreen();
  }

protected:
  virtual bool mainLoop(Event* event) = 0;

private:
  Originals original;
};

class CustomWindow : public Window {
protected:
  virtual bool mainLoop(Event* event) {
    if (handleCtrlQ(event))
      return false;
    else if (handleCursorWithArrows(event));
    else if (event->eventType == KEY_DOWN_EVENT) {
      if (isChar(event->params.keyCode))
        std::cout << event->params.keyCode.asChar;
    }
      
    return true;
  }
};


int main() {
  CustomWindow window;
  window.run();
  return 0;
}

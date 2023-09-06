#pragma once

class Display {
public:
  static Display* getDisplay(void*) {
    return new Display;
  }
  void setCurrent(void*) {
  }
};

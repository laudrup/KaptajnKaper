#pragma once

#include "Graphics.h"

class Image {
public:
  static Image* createImage(int, int) {
    return new Image;
  }
  static Image* createImage(const char*) {
    return new Image;
  }
  static Graphics* getGraphics() {
    return new Graphics;
  }
};

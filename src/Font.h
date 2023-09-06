#pragma once

class Font {
public:
  inline static int FACE_MONOSPACE = 42;
  inline static int STYLE_PLAIN = 42;
  inline static int SIZE_SMALL = 42;
  inline static int STYLE_BOLD = 42;
  inline static int SIZE_MEDIUM = 42;

  static Font* getFont(int, int, int) {
    return new Font;
  }
};

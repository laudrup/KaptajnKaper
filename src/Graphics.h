#pragma once

class Image;
class Font;

class Graphics {
public:
  void setClip(int, int, int, int) {
  }
  void drawImage(Image*, int, int, int) {
  }
  void drawRect(int, int, int, int) {
  }
  void setColor(int, int, int) {
  }
  void fillRect(int, int, int, int) {
  }
  void drawString(const char*, int, int, int) {
  }
  void drawLine(int, int, int, int) {
  }
  void setFont(Font*) {
  }
  void drawChars(const char*, int, int, int, int, int) {
  }

  inline static int LEFT = 4;
  inline static int TOP = 16;
};

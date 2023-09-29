#pragma once

#include "Font.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

class Image;

class Graphics {
public:
  Graphics(sf::RenderTarget* target);
  void clear();
  void setClip(int x, int y, int width, int height);
  void drawImage(const Image& img, int x, int y, int anchor);
  void drawRect(int x, int y, int width, int height);
  void setColor(int red, int green, int blue);
  void fillRect(int x, int y, int width, int height);
  void drawString(const std::string& str, int x, int y, int anchor);
  void drawLine(int x1, int y1, int x2, int y2);
  void setFont(const Font& font);
  void drawChars(const char* data, int offset, int length, int x, int y, int anchor);

  inline static int LEFT = 4;
  inline static int TOP = 16;

private:
  sf::RenderTarget* target_;
  sf::IntRect clip_;
  sf::Color color_;
  Font font_;

  std::vector<std::unique_ptr<sf::Drawable>> drawables_;
};

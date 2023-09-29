#include "Font.h"

#include <stdexcept>

namespace {

sf::Text::Style to_sfml_style(int style) {
  switch (style) {
    case Font::STYLE_PLAIN:
      return sf::Text::Style::Regular;
    case Font::STYLE_BOLD:
      return sf::Text::Style::Bold;
    case Font::STYLE_ITALIC:
      return sf::Text::Style::Italic;
    case Font::STYLE_UNDERLINED:
      return sf::Text::Style::Underlined;
  }
  abort();
}

unsigned int size_to_pixels(int size) {
  switch (size) {
    case Font::SIZE_MEDIUM:
      return 10;
    case Font::SIZE_SMALL:
      return 8;
    case Font::SIZE_LARGE:
      return 12;
  }
  abort();
}

const std::string font_file{"../res/Skranji-Regular.ttf"};
}

Font::Font(int face, int style, int size)
  : face_(face)
  , style_(to_sfml_style(style))
  , size_(size_to_pixels(size)) {
  const std::string path{"../res/" + font_file};
  if (!font_.loadFromFile(path)) {
    throw std::runtime_error{std::string{"Unable to load: "} + path};
  }
}

Font Font::getFont(int face, int style, int size) {
  return Font{face, style, size};
}

int Font::face() const {
  return face_;
}

sf::Text::Style Font::style() const {
  return style_;
}

unsigned int Font::size() const {
  return size_;
}

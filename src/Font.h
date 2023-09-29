#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class Font {
public:
  Font() = default;
  Font(int face, int style, int size);
  static Font getFont(int face, int style, int size);

  int face() const;
  sf::Text::Style style() const;
  unsigned int size() const;

  inline static const int FACE_SYSTEM = 0;
  inline static const int FACE_MONOSPACE = 32;
  inline static const int FACE_PROPORTIONAL = 64;

  inline static const int STYLE_PLAIN = 0;
  inline static const int STYLE_BOLD = 1;
  inline static const int STYLE_ITALIC = 2;
  inline static const int STYLE_UNDERLINED = 4;

  inline static const int SIZE_MEDIUM = 0;
  inline static const int SIZE_SMALL = 8;
  inline static const int SIZE_LARGE = 16;

private:
  friend class Graphics;
  int face_ = 0;
  sf::Text::Style style_;
  int size_ = 0;


  sf::Font font_;
};

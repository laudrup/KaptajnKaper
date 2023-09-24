#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <stdexcept>
#include <variant>

class Graphics;

class Image {
public:
  Image() = default;
  Graphics* getGraphics();
  static Image createImage(int width, int height);
  static Image createImage(const char* path);
  operator bool() const;
  std::unique_ptr<sf::Sprite> getSprite(int x, int y, const sf::IntRect& clip) const;

private:
  Image(int width, int height);
  Image(const char* path);

  std::variant<sf::Texture, std::shared_ptr<sf::RenderTexture>> texture_;
  std::shared_ptr<Graphics> graphics_;
  bool valid_ = false;
};

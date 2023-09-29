#include "Graphics.h"
#include "Image.h"

#include <SFML/Graphics.hpp>

#include <cassert>

Graphics::Graphics(sf::RenderTarget* target)
  : target_(target)
  , clip_(0, 0, static_cast<int>(target_->getSize().x), static_cast<int>(target_->getSize().y)) {
}

void Graphics::clear() {
  drawables_.clear();
}

void Graphics::setClip(int x, int y, int width, int height) {
  assert(width > 0);
  assert(height > 0);
  clip_ = sf::IntRect{x, y, width, height};
}

void Graphics::drawImage(const Image& img, int x, int y, [[maybe_unused]] int anchor) {
  assert(anchor == (TOP | LEFT));
  assert(clip_.left > x);
  assert(clip_.top > y);
  auto sprite = img.getSprite(x, y, clip_);
  target_->draw(*sprite);
  drawables_.push_back(std::move(sprite));
}

void Graphics::drawRect(int x, int y, int width, int height) {
  auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f{static_cast<float>(width), static_cast<float>(height)});
  rectangle->setOutlineThickness(1.f);
  rectangle->setOutlineColor(color_);
  rectangle->setPosition(static_cast<float>(x), static_cast<float>(y));
  target_->draw(*rectangle);
  drawables_.push_back(std::move(rectangle));
}

void Graphics::setColor(int red, int green, int blue) {
  color_ = sf::Color{static_cast<sf::Uint8>(red), static_cast<sf::Uint8>(green), static_cast<sf::Uint8>(blue)};
}

void Graphics::fillRect(int x, int y, int width, int height) {
  auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f{static_cast<float>(width), static_cast<float>(height)});
  rectangle->setFillColor(color_);
  rectangle->setPosition(static_cast<float>(x), static_cast<float>(y));
  target_->draw(*rectangle);
  drawables_.push_back(std::move(rectangle));
}

void Graphics::drawString(const std::string& str, int x, int y, [[maybe_unused]] int anchor) {
  assert(anchor == (TOP | LEFT));
  auto text = std::make_unique<sf::Text>(str, font_.font_, font_.size());
  text->setStyle(font_.style());
  text->setFillColor(color_);
  text->setPosition(static_cast<float>(x), static_cast<float>(y));
  target_->draw(*text);
  drawables_.push_back(std::move(text));
}

void Graphics::drawLine(int x1, int y1, int x2, int y2) {
  auto line = std::make_unique<sf::VertexArray>(sf::Lines, 2);
  (*line)[0].position = sf::Vector2f(static_cast<float>(x1), static_cast<float>(y1));
  (*line)[0].color  = color_;
  (*line)[1].position = sf::Vector2f(static_cast<float>(x2), static_cast<float>(y2));
  (*line)[1].color  = color_;
  target_->draw(*line);
  drawables_.push_back(std::move(line));
}

void Graphics::setFont(const Font& font) {
  font_ = font;
}

void Graphics::drawChars(const char* data, int offset, int length, int x, int y, [[maybe_unused]] int anchor) {
  drawString(std::string{data + offset, static_cast<size_t>(length)}, x, y, anchor);
}

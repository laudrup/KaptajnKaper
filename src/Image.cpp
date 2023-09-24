#include "Image.h"
#include "Graphics.h"

#include <variant>

Graphics* Image::getGraphics() {
  auto texture = std::get<std::shared_ptr<sf::RenderTexture>>(texture_);
  graphics_ = std::make_shared<Graphics>(texture.get());
  return graphics_.get();
}

Image Image::createImage(int width, int height) {
  return Image{width, height};
}

Image Image::createImage(const char* path) {
  return Image{path};
}

Image::operator bool() const {
  return valid_;
}

std::unique_ptr<sf::Sprite> Image::getSprite(int x, int y, const sf::IntRect& clip) const {
  if (std::holds_alternative<sf::Texture>(texture_)) {
    auto texture = std::get<sf::Texture>(texture_);
    auto sprite = std::make_unique<sf::Sprite>(std::get<sf::Texture>(texture_),
                                               sf::IntRect{
                                                 clip.left > x ? clip.left - x : 0,
                                                 clip.top > y ? clip.top - y : 0,
                                                 std::min(clip.width, static_cast<int>(texture.getSize().x)),
                                                 std::min(clip.height, static_cast<int>(texture.getSize().y))
                                               });
    sprite->setPosition(static_cast<float>(clip.left > x ? clip.left : x),
                        static_cast<float>(clip.top > y ? clip.top : y));
    return sprite;
  } else {
    auto texture = std::get<std::shared_ptr<sf::RenderTexture>>(texture_);
    auto sprite = std::make_unique<sf::Sprite>(std::get<std::shared_ptr<sf::RenderTexture>>(texture_)->getTexture());
    sprite->setPosition(static_cast<float>(x), static_cast<float>(y));
    return sprite;
  }
}

Image::Image(int width, int height) {
  texture_.emplace<std::shared_ptr<sf::RenderTexture>>(std::make_shared<sf::RenderTexture>());
  if (!std::get<std::shared_ptr<sf::RenderTexture>>(texture_)->create(static_cast<unsigned int>(width), static_cast<unsigned int>(height))) {
    throw std::runtime_error{std::string{"Unable to create texture"}};
  }
  valid_ = true;
}

Image::Image(const char* path) {
  std::string full_path{std::string{"../res/"} + path};
  texture_.emplace<sf::Texture>(sf::Texture());
  if (!std::get<sf::Texture>(texture_).loadFromFile(full_path)) {
    throw std::runtime_error{std::string{"Unable to load: "} + full_path};
  }
  valid_ = true;
}

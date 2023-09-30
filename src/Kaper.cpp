#include "Kaper.h"
#include "Display.h"
#include "Graphics.h"
#include "Canvas.h"

#include <SFML/Window.hpp>

#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>

namespace {
using std::ostringstream;

std::optional<int> translate_key(sf::Keyboard::Key key) {
  using Key = sf::Keyboard::Key;
  switch(key) {
    case Key::Num0:
    case Key::Numpad0:
      return Canvas::KEY_NUM0;
    case Key::Num1:
    case Key::Numpad1:
      return Canvas::KEY_NUM1;
    case Key::Num2:
    case Key::Numpad2:
      return Canvas::KEY_NUM2;
    case Key::Num3:
    case Key::Numpad3:
      return Canvas::KEY_NUM3;
    case Key::Num4:
    case Key::Numpad4:
      return Canvas::KEY_NUM4;
    case Key::Num5:
    case Key::Numpad5:
      return Canvas::KEY_NUM5;
    case Key::Num6:
    case Key::Numpad6:
      return Canvas::KEY_NUM6;
    case Key::Num7:
    case Key::Numpad7:
      return Canvas::KEY_NUM7;
    case Key::Num8:
    case Key::Numpad8:
      return Canvas::KEY_NUM8;
    case Key::Num9:
    case Key::Numpad9:
      return Canvas::KEY_NUM9;
    case Key::Right:
      return Canvas::RIGHT;
    case Key::Left:
      return Canvas::LEFT;
    case Key::Up:
      return Canvas::UP;
    case Key::Down:
      return Canvas::DOWN;
    case Key::Space:
    case Key::Enter:
      return Canvas::FIRE;
    case Key::Escape:
      return Canvas::KEY_STAR;
    default:
      return std::nullopt;
  }
}

sf::VideoMode get_video_mode() {
  return {sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2};
}

void save_screenshot(const sf::RenderWindow& window) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  ostringstream os;
  os << std::put_time(&tm, "screenshot_%Y%m%d_%H%M%S.png");
  const auto fname = os.str();

  sf::Texture tex;
  tex.create(window.getSize().x, window.getSize().y);
  tex.update(window);

  sf::Image img = tex.copyToImage();
  img.saveToFile(fname);
  std::cerr << "Screenshot saved to '" << fname << "'\n";
}

} // namespace

Kaper::Kaper()
  : window_{get_video_mode(), "MobileKaper"} {
  m_oKaperCanvas = nullptr;
  m_bSnd = false;
}

void Kaper::pauseApp() {
  m_bPaused = true;
  m_bSnd = false;
  if (m_oKaperCanvas != nullptr) {
    m_oKaperCanvas->SoftButtonSND(m_bSnd);
  }
}

void Kaper::destroyApp(bool /*unconditional*/) {
  m_bPaused = true;

  if (m_oKaperCanvas != nullptr) {
    if (m_oKaperCanvas->m_cGameMode == 2) {
      m_oKaperCanvas->m_oStateGame->SaveGame();
    }
  }
  window_.close();
}

void Kaper::startApp() {
  m_bPaused = false;
  window_.setVerticalSyncEnabled(true);
  sf::View view(sf::FloatRect(0, 0, 400, 300));
  window_.setView(view);

  m_oKaperCanvas = new KaperCanvas(view);
  m_oKaperCanvas->m_oKaperApp = this;
  m_oKaperCanvas->setCommandListener(this);
  m_oKaperCanvas->Init(0);
  Graphics g{&window_};

  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window_.close();
          break;
        case sf::Event::KeyPressed: {
          if (event.key.code == sf::Keyboard::Key::F12) {
            save_screenshot(window_);
            continue;
          }
          auto key = translate_key(event.key.code);
          if (key) {
            m_oKaperCanvas->keyPressed(*key);
          }
          break;
        }
        case sf::Event::KeyReleased: {
          auto key = translate_key(event.key.code);
          if (key) {
            m_oKaperCanvas->keyReleased(*key);
          }
          break;
        }
        default:
          break;
      }
    }
    m_oKaperCanvas->run();
    m_oKaperCanvas->paint(&g);
    m_oKaperCanvas->repaint();
    m_oKaperCanvas->serviceRepaints();
    window_.display();
  }
}

void Kaper::commandAction(Command*, Displayable*) {
}

int main(int /*argc*/, const char* const /*argv*/[]) {
  Kaper kaper;
  kaper.startApp();
}

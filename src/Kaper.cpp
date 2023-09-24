#include "Kaper.h"
#include "Display.h"
#include "Graphics.h"
#include "Canvas.h"

#include <SFML/Window.hpp>


Kaper::Kaper() {
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
}

void Kaper::startApp() {
  m_bPaused = false;
  const auto default_mode = sf::VideoMode(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);
  sf::RenderWindow window(default_mode, "MobileKaper");
  window.setVerticalSyncEnabled(true);
  sf::View view(sf::FloatRect(0, 0, 200, 300));
  window.setView(view);

  Display* m_oDisplay = Display::getDisplay(this);
  m_oKaperCanvas = new KaperCanvas(view);
  m_oKaperCanvas->m_oKaperApp = this;
  m_oKaperCanvas->setCommandListener(this);
  m_oDisplay->setCurrent(m_oKaperCanvas);
  m_oKaperCanvas->Init(0);
  Graphics g{&window};

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        default:
          break;
      }
    }
    m_oKaperCanvas->run();
    m_oKaperCanvas->paint(&g);
    m_oKaperCanvas->repaint();
    m_oKaperCanvas->serviceRepaints();
    window.display();
  }
}

void Kaper::commandAction(Command*, Displayable*) {
}

int main(int /*argc*/, const char* const /*argv*/[]) {
  Kaper kaper;
  kaper.startApp();
}

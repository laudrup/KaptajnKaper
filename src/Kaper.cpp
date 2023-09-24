#include "Kaper.h"
#include "Display.h"
#include "Thread.h"

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

  if (m_oKaperCanvas == nullptr) {
    Display* m_oDisplay = Display::getDisplay(this);
    m_oKaperCanvas = new KaperCanvas();
    m_oKaperCanvas->m_oKaperApp = this;

    m_oKaperCanvas->Init(0);
    Thread* runner = new Thread(m_oKaperCanvas);
    runner->start();

    m_oDisplay->setCurrent(m_oKaperCanvas);

    m_oKaperCanvas->setCommandListener(this);

    m_oKaperCanvas->repaint();
    m_oKaperCanvas->serviceRepaints();

    delete runner;
  }
}

void Kaper::commandAction(Command*, Displayable*) {
}

int main(int /*argc*/, const char* const /*argv*/[]) {
  Kaper kaper;
  kaper.startApp();
}

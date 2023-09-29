#pragma once

#include "KaperCanvas.h"

class Command;
class Displayable;

class Kaper {
public:
  Kaper();
  void pauseApp();
  void destroyApp(bool unconditional);
  void startApp();
  void commandAction(Command* c, Displayable* s);
  void notifyDestroyed() {
  }

  bool m_bPaused = false;
  bool m_bSnd = false;
  KaperCanvas* m_oKaperCanvas;

private:
  sf::RenderWindow window_;
};

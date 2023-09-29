#pragma once

#include "Font.h"
#include "Image.h"
#include "StateMenu.h"
#include "StateGame.h"

#include "stringbuilder.h"

#include <stdexcept>
#include <string>
#include <vector>

class Random;
class Player;
class Graphics;
class Kaper;
class StateGame;
class StateMenu;

class KaperCanvas {
public:
  KaperCanvas(sf::View& view);
  void paint(Graphics* oFrontBuffer);
  void Init(int cGameMode);
  void run();
  void SoftButtonSND(bool bState);
  void PlaySound(const std::string& sName, bool bRepeat);
  void StopSound();
  void ShutDown();

  int getGameAction(int) {
    return 42;
  }

  void setCommandListener(void*) {
  }

  void repaint() {
  }

  void serviceRepaints() {
  }

  int getWidth() {
    return static_cast<int>(view_.getSize().x);
  }

  int getHeight() {
    return static_cast<int>(view_.getSize().y);
  }

  void keyPressed(int keyCode);
  void keyReleased(int keyCode);

  void DrawString(std::string sText, int x, int y, bool bBold, int iColor);
  void DrawString(int iNumbers, int x, int y, bool bBold, int iColor);

  Kaper* m_oKaperApp;

  int m_iWidth = 0;
  int m_iHeight = 0;
  Random* m_oRand;

  std::vector<Image> m_oImageArray;

  StateMenu* m_oStateMenu;
  StateGame* m_oStateGame;

  Font m_oFontSmallP;
  Font m_oFontSmallB;
  Font m_oFontMediumB;

  bool m_bSpeedOptimized = false;
  long long m_lSpeed = 0;
  bool m_bFontBitmap = false;
  bool m_bWantToQuit = false;
  char m_cGameMode = '\0';

private:
  Player* m_oPlayer;
  long long m_lIntroWait = 0;
  bool m_bIntroWaitb = false;
  Graphics* m_oFrontBuffer;
  StringBuilder* m_stringBuffer;
  std::vector<char> m_stringChars;

  sf::View& view_;
};

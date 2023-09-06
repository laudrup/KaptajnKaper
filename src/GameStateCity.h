#pragma once

#include "KaperCanvas.h"

#include <cmath>
#include <string>

class CGameStateCity {
public:
  CGameStateCity(KaperCanvas* oKaperCanvas);
  void Init();
  void DeInit();
  void Draw(Graphics* g);
  int Update();
  int SoftKey(int iKey);

  std::string m_sCityName;
  int m_iBlink = 0;
  char m_cTarget = '\0';

private:
  KaperCanvas* m_oCanvas;
  int m_iFoodPriceBuy = 0;
  int m_iPiratePriceBuy = 0;
  int m_iCannonPriceBuy = 0;
  int m_iFoodPriceSell = 0;
  int m_iPiratePriceSell = 0;
  int m_iCannonPriceSell = 0;
  int m_iTarget = 0;
  int m_iAmount = 0;
  bool m_bLocked = false;
};

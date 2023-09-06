#pragma once

#include "GameStateBoard.h"
#include <cmath>
#include <vector>

class KaperCanvas;
class Graphics;

class CGameStateSailingCity {
public:
  CGameStateSailingCity(KaperCanvas* oKaperCanvas);
  void Init();
  void Draw(Graphics* g);
  int Update();
  void SoftKey(int iKey);

  bool m_bPaused = false;
  int m_iStatus = 0;

private:
  KaperCanvas* m_oCanvas;
  int m_iShipX = 0;
  int m_iShipY = 0;
  std::vector<int> m_iObjectsX;
  std::vector<int> m_iObjectsY;
  int m_iNumberOfObliteries = 0;
  int m_iWind = 0;
  bool m_bRight = false;
  bool m_bLeft = false;
  bool m_bDown = false;
  bool m_bLocked = false;
};

#pragma once

#include <cmath>
#include <vector>

class Graphics;
class KaperCanvas;

class CGameStateAttack {
public:
  CGameStateAttack(KaperCanvas* oKaperCanvas);
  void Init(int iEnemiesCannons, int iEnemiesSoldier);
  void Draw(Graphics* g);
  int Update();
  int SoftKey(int iKey);
  void SoftKeyRelease(int iKey);

  bool m_bRetreat = false;
  int m_iKeyPause = 0;

private:
  void DrawBalls(Graphics* g);
  void DrawBoost(Graphics* g);
  void DrawBoats(Graphics* g);
  void DrawInterface(Graphics* g);
  void Move();
  void KeyCheck(int iKey);
  void TheBrain();
  void DrawSplash(Graphics* g);
  void DrawWakes(Graphics* g);
  void DrawFlash(Graphics* g);
  void CalulateSpeedEnemy();
  void CalulateSpeedFriend();

  KaperCanvas* m_oCanvas;

  int m_iEnemyCannons = 0;
  int m_iEnemySoldiers = 0;
  int m_iFriendlyCannons = 0;
  int m_iFriendlyPirates = 0;

  int m_iFriendlyFireFlash = 0;
  int m_iEnemyFireFlash = 0;
  int m_iFriendlyFireFlashX = 0;
  int m_iEnemyFireFlashX = 0;
  int m_iFriendlyFireFlashY = 0;
  int m_iEnemyFireFlashY = 0;

  int m_iEnemyBoatX = 0;
  int m_iEnemyBoatY = 0;
  int m_iEnemyBoatBoost = 0;
  int m_iFriendlyBoatX = 0;
  int m_iFriendlyBoatY = 0;
  int m_iFriendlyBoatBoost = 0;
  int m_iFriendlyCoolDown = 0;
  int m_iEnemyCoolDown = 0;

  int m_iBrainX = 0;
  int m_iBrainY = 0;
  int m_iBrainFire = 0;
  int m_iBrainNew = 0;

  int m_iEnemyYSpeed = 0;
  int m_iEnemyXSpeed = 0;
  std::vector<int> m_iEnemyCannonBallsX;
  std::vector<int> m_iEnemyCannonBallsY;
  std::vector<int> m_iEnemyCannonBallsXSpeed;
  std::vector<int> m_iEnemyCannonBallsYSpeed;
  std::vector<int> m_iEnemyCannonBallsBoost;
  std::vector<int> m_iFriendlyCannonBallsX;
  std::vector<int> m_iFriendlyCannonBallsY;
  std::vector<int> m_iFriendlyCannonBallsXSpeed;
  std::vector<int> m_iFriendlyCannonBallsYSpeed;
  std::vector<int> m_iFriendlyCannonBallsBoost;
  int m_iFriendYSpeed = 0;
  int m_iFriendXSpeed = 0;

  std::vector<int> m_iWaterSplashX;
  std::vector<int> m_iWaterSplashY;
  std::vector<int> m_iWaterSplashLife;
  std::vector<int> m_iExplosionX;
  std::vector<int> m_iExplosionY;
  std::vector<int> m_iExplosionLife;

  int m_iWaterMoveOffset = 0;
  bool m_bGameOver = false;
  bool m_bBoard = false;
  bool m_bLeft = false;
  bool m_bRight = false;
  bool m_bUp = false;
  bool m_bDown = false;
  bool m_bFire = false;

  std::vector<int> m_iShipEnemyWaveA;
  std::vector<int> m_iShipEnemyWaveX;
  std::vector<int> m_iShipEnemyWaveY;
  std::vector<int> m_iShipFriendlyWaveA;
  std::vector<int> m_iShipFriendlyWaveX;
  std::vector<int> m_iShipFriendlyWaveY;
  int m_iFriendlyShipType = 0;
  bool m_bLocked = false;
};

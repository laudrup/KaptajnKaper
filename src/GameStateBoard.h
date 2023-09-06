#pragma once

#include <cmath>
#include <vector>

class KaperCanvas;
class Graphics;

class CGameStateBoard {
public:
  CGameStateBoard(KaperCanvas* oKaperCanvas);
  void Init(int iEnemiesSoldier);
  void DeInit();
  void Draw(Graphics* g);
  int Update();
  int SoftKey(int iKey);
  void SoftKeyRelease(int iKey);

  bool m_bRetreat = false;
  int m_iKeyPause = 0;

private:
  void DrawBackGround(Graphics* g);
  void DrawMen(Graphics* g);
  void DrawFlash(Graphics* g);
  void DrawBullets(Graphics* g);
  void DrawText(Graphics* g);
  void CreateSoldier(bool bFriendly, bool bEnemy);
  bool CheckObjectCollision(int iX, int iY);
  void MoveToCommandPoint();
  void HandleAI();
  void AIAvoidShoot();
  bool AILineOfSight(int iXDes, int iXSrc, int iYSrc);
  void AIAnalyzeShoot();
  void AIShoot();
  void AIAnalyzeMove();
  void DrawCooldown(Graphics* g);

  KaperCanvas* m_oCanvas;
  int m_iEnemySoldier = 0;
  int m_iFriendlySoldier = 0;

  bool m_bAllowToShot = false;
  bool m_bFriendlyLeaderPause8 = false;
  bool m_bFriendlyLeaderPause2 = false;
  bool m_bFriendlyLeaderPause4 = false;
  bool m_bFriendlyLeaderPause6 = false;
  bool m_bEnemyLeaderPause8 = false;
  bool m_bEnemyLeaderPause4 = false;
  bool m_bEnemyLeaderPause2 = false;
  bool m_bEnemyLeaderPause6 = false;

  int m_iFriendlyMovePause = 0;
  int m_iEnemyMovePause = 0;
  int m_iFriendlyFireFlash = 0;
  int m_iEnemyFireFlash = 0;
  int m_iFriendlyCoolDown = 0;
  int m_iEnemyCoolDown = 0;

  int m_iFriendlyLeaderX = 0;
  int m_iFriendlyLeaderY = 0;
  std::vector<int> m_iFriendlySoldierFight;
  std::vector<int> m_iFriendlySoldierX;
  std::vector<int> m_iFriendlySoldierY;
  std::vector<bool> m_bFriendlySoldierDead;
  std::vector<int> m_iFriendlySoldierDeadAnim;
  std::vector<int> m_iEnemySoldierFight;
  int m_iEnemyLeaderX = 0;
  int m_iEnemyLeaderY = 0;
  std::vector<int> m_iEnemySoldierX;
  std::vector<int> m_iEnemySoldierY;
  std::vector<bool> m_bEnemySoldierDead;
  std::vector<int> m_iEnemySoldierDeadAnim;
  bool m_bGameOver = false;
  int m_iDifficult = 0;

  std::vector<int> m_iFriendlyBulletX;
  std::vector<int> m_iFriendlyBulletY;
  std::vector<bool> m_bFriendlyBulletDead;
  std::vector<int> m_iEnemyBulletX;
  std::vector<int> m_iEnemyBulletY;
  std::vector<bool> m_bEnemyBulletDead;

  std::vector<int> m_iObjectType;
  std::vector<int> m_iObjectX;
  std::vector<int> m_iObjectY;

  int m_iFriendlyBezerk = 0;
  int m_iEnemyBezerk = 0;

  int m_iEnemyBullets = 0;
  int m_iFriendlyBullets = 0;

  int m_iFriendlyAnim = 0;
  int m_iEnemyAnim = 0;

  int m_iFriendlySkyX1 = 0;
  int m_iFriendlySkyX2 = 0;

  bool m_bLeft = false;
  bool m_bRight = false;
  bool m_bUp = false;
  bool m_bDown = false;

  bool m_bAILeft = false;
  bool m_bAIRight = false;
  bool m_bAIUp = false;
  bool m_bAIDown = false;
  int m_iAIAimTargets = 0;
  int m_iAIFlee = 0;

  bool m_bLocked = false;
  char m_iNumberOfMenToCreate = L'\0';
};

#pragma once

#include "GameStateBoard.h"
#include "GameStateAttack.h"
#include "KaperCanvas.h"
#include "GameStateSailingCity.h"

#include "stringbuilder.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

class RegisterHighscore;
class CGameStateSailingCity;
class CGameStateCity;
class CGameStateAttack;
class InputStream;
class HttpConnection;

class StateGame {
public:
  RegisterHighscore* m_oRegisterHigh;

  int m_iShipX = 0;
  int m_iShipY = 0;
  int m_iShipGraphicWay = 0;
  bool m_bShipMoving = false;

  std::vector<int> m_iShipWaveA;
  std::vector<int> m_iShipWaveX;
  std::vector<int> m_iShipWaveY;

  ////////////////////////////////////////////////////////////
  // EnemyShips
  ////////////////////////////////////////////////////////////
  std::vector<int> m_iEnemyShipX;
  std::vector<int> m_iEnemyShipY;
  std::vector<int> m_iEnemyShipShipType;
  std::vector<int> m_iEnemyShipCannons;
  std::vector<int> m_iEnemyShipSoldiers;

  std::vector<char> m_iEnemyShipGraphicWay;

  std::vector<std::vector<int>> m_iEnemyShipWaveA;
  std::vector<std::vector<int>> m_iEnemyShipWaveX;
  std::vector<std::vector<int>> m_iEnemyShipWaveY;

  ////////////////////////////////////////////////////////////
  // TheMap
  ////////////////////////////////////////////////////////////
  int m_iMapScreenX = 0;
  int m_iMapScreenY = 0;
  int m_iMapOffScreenX = 0;
  int m_iMapOffScreenY = 0;
  bool m_bMapMoving = false;
  std::vector<std::vector<char>> m_aoGameMap;

  int m_iMapWay = 0;
  bool m_bMapScrollN = false;
  bool m_bMapScrollS = false;
  bool m_bMapScrollE = false;
  bool m_bMapScrollW = false;

  ////////////////////////////////////////////////////////////
  // Skies
  ////////////////////////////////////////////////////////////
  std::vector<int> m_iSkyX;
  std::vector<int> m_iSkyY;
  std::vector<int> m_iSkyType;
  int m_iSkyCounterY = 0;

  ////////////////////////////////////////////////////////////
  // Event
  ////////////////////////////////////////////////////////////
  int m_iAddFood = 0;
  int m_iAddGold = 0;
  int m_iAddPirates = 0;
  int m_iAddCannons = 0;
  int m_iEventOld = 0;
  std::string m_sEventMsg1;
  std::string m_sEventMsg2;

  int m_iShipNr = 0;
  int m_iEvent = 0;
  bool m_bWait = false;
  int m_iPiratesOnCapturedShip = 0;

  int m_iFleePunishKapers = 0;
  int m_iFleePunishCannons = 0;

  ////////////////////////////////////////////////////////////
  // Interface
  ////////////////////////////////////////////////////////////
  int m_iResourceGold = 0;
  int m_iResourceCorn = 0;
  int m_iResourceCannon = 0;
  int m_iResourcePirate = 0;
  int m_iTurn = 0;
  int m_iScore = 0;
  int m_iScoreTurn100 = 0;
  int m_iScoreTurn300 = 0;
  int m_iShipLvl = 0;

  ////////////////////////////////////////////////////////////
  // StateGame
  ////////////////////////////////////////////////////////////
  int m_iGameState = 0;
  CGameStateBoard* m_oStateBoard;
  CGameStateAttack* m_oStateAttack;
  CGameStateCity* m_oStateCity;
  int m_iCapturedShip = 0;

  std::string m_sPlayerName;

  KaperCanvas* m_oCanvas;
  bool m_bWaitingOnMove = false;
  bool m_bClose = false;
  bool m_bToCity = false;

  int m_iWebRankShow = 0;

  CGameStateSailingCity* m_oStateSailingToCity;

  int m_iPlayerNamePos = 0;

  int m_iKeyCountDown = 0;

  bool m_bKeyLeft = false;
  bool m_bKeyRight = false;
  bool m_bKeyUp = false;
  bool m_bKeyDown = false;

  bool m_bLocked = false;

  // Events in water
  std::vector<int> m_iEventX;
  std::vector<int> m_iEventY;
  int m_iEventAnim = 0;

  int m_iEnemySoldiers = 0;

  bool m_bShotButtonGuide = false;

  StateGame(KaperCanvas* oKaperCanvas);
  void Init();

  ////////////////////////////////////////////////////////////
  // TheMap
  ////////////////////////////////////////////////////////////
  bool MapIncSetXY(int iWay);
  int MapCheckCollision(int iX, int iY, bool bScreenOffset);
  void MapDraw(Graphics* g);
  void MapUpdate();

  ////////////////////////////////////////////////////////////
  // KaperShip
  ////////////////////////////////////////////////////////////
  void ShipDraw(Graphics* g);
  void ShipUpdate();

  // False: Move ship
  // True: Move map with ship
  int ShipMove(int iWay, bool bForce);

  ////////////////////////////////////////////////////////////
  // EnemyShip
  ////////////////////////////////////////////////////////////
  void EnemyShipUpdate();
  void EnemyShipMove();
  bool EnemyShipCollosion(int iX, int iY);

  ////////////////////////////////////////////////////////////
  // Interface
  ////////////////////////////////////////////////////////////
  bool InterfaceNewTurn();
  void InterfaceDraw(Graphics* g, bool bTurn);
  void Draw(Graphics* g);
  void LoadGame();
  void SaveGame();
  void SaveHighscore(const std::string& sName, int iScore);
  void CleanGame(bool CreateDummy);
  bool Update();
  void CloseCombatWon();
  void NormalButton(int iKey);
  bool CheckBoatsCollision(bool bRemoveThem);
  void GameOver();
  int KeyCheck(int iKey);
  char SwitchChar(char cChar, bool bUp);
  void KeyPressedEvent(int iKey);
  void KeyHandling();
  void CalculateFleePunish();
  void DrawEvent(Graphics* g);
  void DrawBlackBackGround(Graphics* g);
  void DrawFrame(Graphics* g);
  bool CheckEventCollision();
};

class RegisterHighscore {
public:
  RegisterHighscore(const std::string& temp);
  void Deinit();
  void run();

  InputStream* m_Stream = nullptr;
  HttpConnection* m_Http = nullptr;
  StringBuilder* m_sBuf = new StringBuilder();
  std::string sLink;
  char m_cWebDataReady = L'\0';

  std::vector<std::string> m_sWebHighAllPlayers;
  std::vector<std::string> m_sWebHighAllScore;
  std::vector<std::string> m_sWebHighDayPlayers;
  std::vector<std::string> m_sWebHighDayScore;
  std::vector<std::string> m_sWebHighMonPlayers;
  std::vector<std::string> m_sWebHighMonScore;

  std::vector<std::string> m_sWebHigh100AllPlayers;
  std::vector<std::string> m_sWebHigh100AllScore;
  std::vector<std::string> m_sWebHigh100DayPlayers;
  std::vector<std::string> m_sWebHigh100DayScore;
  std::vector<std::string> m_sWebHigh100MonPlayers;
  std::vector<std::string> m_sWebHigh100MonScore;

  std::vector<std::string> m_sWebHigh300AllPlayers;
  std::vector<std::string> m_sWebHigh300AllScore;
  std::vector<std::string> m_sWebHigh300DayPlayers;
  std::vector<std::string> m_sWebHigh300DayScore;
  std::vector<std::string> m_sWebHigh300MonPlayers;
  std::vector<std::string> m_sWebHigh300MonScore;

  std::vector<std::string> m_sWebHighAllPlace;
  std::vector<std::string> m_sWebHighDayPlace;
  std::vector<std::string> m_sWebHighMonPlace;
  std::vector<std::string> m_sWebHigh100AllPlace;
  std::vector<std::string> m_sWebHigh100DayPlace;
  std::vector<std::string> m_sWebHigh100MonPlace;
  std::vector<std::string> m_sWebHigh300AllPlace;
  std::vector<std::string> m_sWebHigh300DayPlace;
  std::vector<std::string> m_sWebHigh300MonPlace;
};

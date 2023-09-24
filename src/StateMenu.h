#pragma once

#include "StateGame.h"
#include "stringbuilder.h"

#include <stdexcept>
#include <string>
#include <vector>

class InputStream;
class HttpConnection;
class KaperCanvas;
class RegisterHighscore;

class StateMenu {
public:
  StateMenu(KaperCanvas* oKaperCanvas);
  void Draw(Graphics* g);
  int Update();
  void NormalButton(int iKey);

  bool m_bLocked = false;
  bool m_bFirstTime = false;
  bool m_bAnyContinueGame = false;
  int m_iMenuSelect = 0;
  int m_iSubmitCodeStatus = 0;
  std::string m_sWebCode;

private:
  void LoadScore();
  char SwitchChar(char cChar, bool bUp);

  int m_iMenuY = 0;
  int m_iWebCodePos = 0;
  int m_iWebRankShow = 0;
  std::vector<std::string> m_sHighPlayers;
  std::vector<std::string> m_sHighScore;
  KaperCanvas* m_oCanvas;
  int m_iKeyOldState = 0;
  char m_cLineInstructions = '\0';
  bool m_bShowInstructions = false;
  bool m_bShowHighscore = false;
  bool m_bShowWebHighscore = false;
  bool m_bShowCredits = false;
  bool m_bShowWebCode = false;
  RegisterHighscore* m_oHighscore;
  int m_iScrollX = 0;
};

class SubmitWebScore {
public:
  SubmitWebScore(StateMenu* oFather);
  void run();

  StateMenu* m_oFather;
  InputStream* m_Stream = nullptr;
  StringBuilder* m_sBuf = new StringBuilder();
  HttpConnection* m_Http = nullptr;
};

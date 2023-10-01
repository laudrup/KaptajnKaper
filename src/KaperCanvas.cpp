#include "KaperCanvas.h"
#include "Canvas.h"
#include "Font.h"
#include "GameStateCity.h"
#include "Graphics.h"
#include "Image.h"
#include "Kaper.h"
#include "Player.h"
#include "Random.h"
#include "RecordStore.h"

#include <algorithm>
#include <cctype>
#include <chrono>

namespace {
  std::string to_upper(std::string source) {
    std::transform(source.begin(), source.end(), source.begin(), [](char c) {
      return std::toupper(c);
    });
    return source;
  }
}

KaperCanvas::KaperCanvas(sf::View& view)
  : view_(view) {
  m_bWantToQuit = false;

  m_stringBuffer = new StringBuilder("0000");
  m_stringChars = std::vector<char>(m_stringBuffer->length());
  m_lIntroWait = 1999999999;
  m_cGameMode = 0;

  m_iWidth = getWidth();
  m_iHeight = getHeight();

  m_oRand = new Random(/*System::currentTimeMillis()*/);

  m_oFontSmallP = Font::getFont(Font::FACE_MONOSPACE, Font::STYLE_PLAIN, Font::SIZE_SMALL);
  m_oFontSmallB = Font::getFont(Font::FACE_MONOSPACE, Font::STYLE_BOLD, Font::SIZE_SMALL);
  m_oFontMediumB = Font::getFont(Font::FACE_MONOSPACE, Font::STYLE_BOLD, Font::SIZE_MEDIUM);

  m_oPlayer = nullptr;
  m_oStateMenu = nullptr;
  m_oStateGame = nullptr;

  m_bFontBitmap = true;
  m_bIntroWaitb = false;
}

void KaperCanvas::paint(Graphics* oFrontBuffer) {
  m_oFrontBuffer = oFrontBuffer; // To use with Font
  if (!m_bWantToQuit) {
    switch (m_cGameMode) {
    case 0:
    case 6: {

      oFrontBuffer->setColor(42, 102, 139);
      oFrontBuffer->fillRect(0, 0, m_iWidth, m_iHeight);
      oFrontBuffer->setClip((m_iWidth / 2) - 50, (m_iHeight / 2) - 50, 89, 71);
      oFrontBuffer->drawImage(
          m_oImageArray[1], (m_iWidth / 2) - 50, (m_iHeight / 2) - 50, Graphics::TOP | Graphics::LEFT);
      oFrontBuffer->setClip(0, 0, m_iWidth, m_iHeight);
      if (0 == m_cGameMode) {
        int iXCenter = (m_iWidth / 2) - 67;
        int iYCenter = (m_iHeight / 2) - 20;
        int iMove = static_cast<int>(
            (std::chrono::milliseconds(m_lIntroWait) - std::chrono::system_clock::now().time_since_epoch())
                .count()); // 2000-0

        oFrontBuffer->setColor(42, 102, 139);
        if (iMove >= 3000) {
          break;
        } else {
          oFrontBuffer->fillRect(0, 0, m_iWidth, m_iHeight);
        }

        oFrontBuffer->setColor(255, 200, 200);
        if (iMove <= 0) {
          iMove = 1;
        }
        if (iMove > 2000) {
          iMove = 2000;
        }

        // M
        oFrontBuffer->drawLine(iXCenter + 20, 2 + iYCenter, iXCenter + 20, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 20, 2 + iYCenter, iXCenter + 23 - (iMove / 600), 7 - (iMove / 400) + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 23, 7 - (iMove / 800) + iYCenter, iXCenter + 26 - (iMove / 600), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 26, 2 + iYCenter, iXCenter + 26, 12 - (iMove / 200) + iYCenter);
        // O
        oFrontBuffer->drawLine(iXCenter + 28, 2 + iYCenter, iXCenter + 28, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 34, 2 + iYCenter, iXCenter + 34, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 28, 2 + iYCenter, iXCenter + 34 - (iMove / 200), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 28, 12 + iYCenter, iXCenter + 34 - (iMove / 200), 12 + iYCenter);
        // B
        oFrontBuffer->drawLine(iXCenter + 36, 2 + iYCenter, iXCenter + 36, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 42, 2 + iYCenter, iXCenter + 42, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 36, 2 + iYCenter, iXCenter + 42 - (iMove / 200), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 36, 12 + iYCenter, iXCenter + 42 - (iMove / 200), 12 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 36, 6 + iYCenter, iXCenter + 42 - (iMove / 200), 6 + iYCenter);
        // I
        oFrontBuffer->drawLine(iXCenter + 44, 2 + iYCenter, iXCenter + 50 - (iMove / 300), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 44, 12 + iYCenter, iXCenter + 50 - (iMove / 300), 12 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 47, 2 + iYCenter, iXCenter + 47, 12 - (iMove / 200) + iYCenter);
        // L
        oFrontBuffer->drawLine(iXCenter + 52, 2 + iYCenter, iXCenter + 52, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 52, 12 + iYCenter, iXCenter + 58 - (iMove / 300), 12 + iYCenter);
        // K
        oFrontBuffer->drawLine(iXCenter + 60, 2 + iYCenter, iXCenter + 60, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 60, 7 - (iMove / 400) + iYCenter, iXCenter + 66 - (iMove / 400), 2 + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 60, 7 + iYCenter, iXCenter + 66 - (iMove / 400), 12 - (iMove / 400) + iYCenter);
        // A
        oFrontBuffer->drawLine(
            iXCenter + 68, 12 - (iMove / 200) + iYCenter, iXCenter + 71 - (iMove / 600), 2 + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 71, 2 + iYCenter, iXCenter + 74 - (iMove / 600), 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 69, 10 + iYCenter, iXCenter + 73 - (iMove / 500), 10 + iYCenter);
        // P
        oFrontBuffer->drawLine(iXCenter + 76, 12 - (iMove / 200) + iYCenter, iXCenter + 76, 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 76, 2 + iYCenter, iXCenter + 82 - (iMove / 400), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 82, 2 + iYCenter, iXCenter + 82, 7 - (iMove / 400) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 76, 7 + iYCenter, iXCenter + 82 - (iMove / 400), 7 + iYCenter);
        // E
        oFrontBuffer->drawLine(iXCenter + 84, 12 - (iMove / 200) + iYCenter, iXCenter + 84, 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 84, 2 + iYCenter, iXCenter + 90 - (iMove / 300), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 84, 7 + iYCenter, iXCenter + 90 - (iMove / 300), 7 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 84, 12 + iYCenter, iXCenter + 90 - (iMove / 300), 12 + iYCenter);
        // R
        oFrontBuffer->drawLine(iXCenter + 92, 12 - (iMove / 200) + iYCenter, iXCenter + 92, 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 92, 2 + iYCenter, iXCenter + 98 - (iMove / 300), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 98, 2 + iYCenter, iXCenter + 98, 7 - (iMove / 500) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 92, 7 + iYCenter, iXCenter + 98 - (iMove / 300), 7 + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 92, 7 + iYCenter, iXCenter + 98 - (iMove / 300), 12 - (iMove / 500) + iYCenter);
        // .
        oFrontBuffer->drawLine(iXCenter + 100, 10 + iYCenter, iXCenter + 102 - (iMove / 800), 10 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 100, 11 + iYCenter, iXCenter + 102 - (iMove / 800), 11 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 100, 12 + iYCenter, iXCenter + 102 - (iMove / 800), 12 + iYCenter);
        // D
        oFrontBuffer->drawLine(iXCenter + 105, 2 + iYCenter, iXCenter + 105, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 110, 2 + iYCenter, iXCenter + 110, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 104, 2 + iYCenter, iXCenter + 110 - (iMove / 200), 2 + iYCenter);
        oFrontBuffer->drawLine(iXCenter + 104, 12 + iYCenter, iXCenter + 110 - (iMove / 200), 12 + iYCenter);
        // K
        oFrontBuffer->drawLine(iXCenter + 112, 2 + iYCenter, iXCenter + 112, 12 - (iMove / 200) + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 112, 7 - (iMove / 300) + iYCenter, iXCenter + 118 - (iMove / 300), 2 + iYCenter);
        oFrontBuffer->drawLine(
            iXCenter + 112, 7 + iYCenter, iXCenter + 118 - (iMove / 300), 12 - (iMove / 300) + iYCenter);

        oFrontBuffer->setColor(220, 102, 139);

        // o
        oFrontBuffer->drawLine(iXCenter + 15, iYCenter - 3, iXCenter + 123, iYCenter - 3);
        oFrontBuffer->drawLine(iXCenter + 15, iYCenter + 18, iXCenter + 123, iYCenter + 18);
        oFrontBuffer->drawLine(iXCenter + 15, iYCenter + 18, iXCenter + 15, iYCenter - 3);
        oFrontBuffer->drawLine(iXCenter + 123, iYCenter - 3, iXCenter + 123, iYCenter + 18);

        oFrontBuffer->drawLine(iXCenter + 13, iYCenter - 5, iXCenter + 125, iYCenter - 5);
        oFrontBuffer->drawLine(iXCenter + 13, iYCenter + 20, iXCenter + 125, iYCenter + 20);
        oFrontBuffer->drawLine(iXCenter + 13, iYCenter + 20, iXCenter + 13, iYCenter - 5);
        oFrontBuffer->drawLine(iXCenter + 125, iYCenter - 5, iXCenter + 125, iYCenter + 20);
      }

      if (6 == m_cGameMode) {
        oFrontBuffer->setColor(255, 0, 0);
        oFrontBuffer->drawString("Not enough mem... [0]", 3, m_iHeight - 20, Graphics::TOP | Graphics::LEFT);
        oFrontBuffer->drawLine(1, m_iHeight - 4, 110, m_iHeight - 4);
        oFrontBuffer->drawLine(1, m_iHeight - 20, 110, m_iHeight - 20);
        oFrontBuffer->drawLine(1, m_iHeight - 20, 1, m_iHeight - 4);
        oFrontBuffer->drawLine(110, m_iHeight - 20, 110, m_iHeight - 4);
      }
    } break;

    case 1: {
      m_oStateMenu->Draw(oFrontBuffer);
    } break;

    case 2: {
      m_oStateGame->Draw(oFrontBuffer);
    } break;
    }
  }
  if (m_bWantToQuit) {
    oFrontBuffer->setColor(42, 102, 139);
    oFrontBuffer->fillRect(0, 0, m_iWidth, m_iHeight);
    oFrontBuffer->setClip((m_iWidth / 2) - 77, 2, 149, 59);
    oFrontBuffer->drawImage(m_oImageArray[0], (m_iWidth / 2) - 77, 2, 0);
    oFrontBuffer->setClip(0, 0, m_iWidth, m_iHeight);
    DrawString("[Game paused]", (m_iWidth / 2) - 42, (m_iHeight / 2) - 5, false, 2);
    DrawString("Press again to quit", (m_iWidth / 2) - 52, (m_iHeight / 2) - 30, false, 2);
  }
}

void KaperCanvas::Init(int cGameMode) {
  switch (cGameMode) {
  case 0: {
      m_oImageArray = std::vector<Image>(25);
      m_oImageArray[1] = Image::createImage("/2.png");
      m_oImageArray[24] = Image::createImage("/8.png");
  } break;

  case 1: {
    if (m_oStateMenu == nullptr) {
      m_oStateMenu = new StateMenu(this);
    }

    // Check for Continue Game
    RecordStore* nameRecord = RecordStore::openRecordStore("KaperGame", true);
    if (nameRecord->getNumRecords() < 10) {
      m_oStateMenu->m_bAnyContinueGame = false;
    } else {
      m_oStateMenu->m_bAnyContinueGame = true;
    }
    nameRecord->closeRecordStore();

    if (m_oStateMenu->m_bAnyContinueGame) {
      m_oStateMenu->m_iMenuSelect = 1;
    } else {
      m_oStateMenu->m_iMenuSelect = 0;
    }
  } break;

  case 2: {
    if (m_oStateGame == nullptr) {
      m_oStateGame = new StateGame(this);
    }
    m_oStateGame->Init();
  } break;
  }
}

void KaperCanvas::run() {
  if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
           .count() -
       m_lSpeed) >= 60) {
    m_lSpeed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();

    switch (m_cGameMode) {
    case 0: {
      if (m_bIntroWaitb == false) {
        m_lIntroWait = 3500 + std::chrono::duration_cast<std::chrono::milliseconds>(
                                  std::chrono::system_clock::now().time_since_epoch())
                                  .count();
        m_oImageArray[0] = Image::createImage("/1.png");
        Init(1);
        m_bIntroWaitb = true;
      } else if ((m_lIntroWait + 1000) < std::chrono::duration_cast<std::chrono::milliseconds>(
                                             std::chrono::system_clock::now().time_since_epoch())
                                             .count()) {
        m_cGameMode = 1;
      }
    } break;

    case 1: {
      if (m_oKaperApp->m_bPaused) {
        break;
      }

      int iStatus = m_oStateMenu->Update();

      if (iStatus == 0) // Start new fresh game
      {
        m_cGameMode = 5;
        Init(2);
        m_oStateGame->CleanGame(true);
        m_cGameMode = 2;
      }

      if (iStatus == 1) // Continue game
      {
        m_cGameMode = 5;
        Init(2);
        m_cGameMode = 2;
        m_oStateGame->LoadGame();
      }

      if (iStatus == 3) // Start new fresh game with +500g
      {
        m_cGameMode = 5;
        Init(2);
        m_oStateGame->CleanGame(true);
        m_cGameMode = 2;

        m_oStateGame->m_iResourceGold += 500;
      }

    } break;

    case 2: {
      if (m_oKaperApp->m_bPaused) {
        break;
      }
      if (m_bWantToQuit) {
        break;
      }

      if (m_oStateGame->Update() == true) { // End game
        Init(1);
        m_cGameMode = 1;
      }

    } break;
    }
  }
}

void KaperCanvas::SoftButtonSND(bool bState) {
  if (!bState) {
    if (m_oPlayer != nullptr) {
      StopSound();
    }
  } else {
    if (m_oPlayer != nullptr) {
      m_oPlayer->start();
    } else if (m_cGameMode == 1) {
      PlaySound("a.mid", false);
    } else if (m_cGameMode == 2) {
      if (m_oStateGame->m_iGameState == 1) {
        PlaySound("d.mid", true);
      }
    }
  }
}

void KaperCanvas::PlaySound(const std::string& /*sName*/, bool /*bRepeat*/) {
  /*
        try
        {
                if (!m_oKaperApp->m_bSnd)
                {
                        return;
                }

                StopSound();

                InputStream *is = getClass().getResourceAsStream(sName);
                                   m_oPlayer = Manager::createPlayer(is,"audio/midi");
                if (bRepeat)
                {
                        m_oPlayer->setLoopCount(-1);
                }
                else
                {
                        m_oPlayer->setLoopCount(1);
                }
                is = nullptr;

                m_oPlayer->realize();
                m_oPlayer->prefetch();

                m_oPlayer->start();
        }
        catch (const IOException &ioe)
        {
        }
        catch (const MediaException &e)
        {
        }
  */
}

void KaperCanvas::StopSound() {
  if (m_oPlayer != nullptr) {
    if (m_oPlayer->getState() == Player::STARTED) {
      m_oPlayer->stop();
    }
    m_oPlayer->deallocate();
    m_oPlayer->close();
    m_oPlayer = nullptr;
  }
}

void KaperCanvas::ShutDown() {
  m_bWantToQuit = false;

  if (m_cGameMode == 2) {
    if (m_oStateGame->m_iGameState == 1) {
      m_oStateGame->m_oStateCity.DeInit();
      m_oStateGame->m_iGameState = 0;
    } else if (m_oStateGame->m_iGameState == 4) {
      m_oStateGame->m_oStateAttack.m_bRetreat = true;
      m_oStateGame->m_oStateAttack.m_iKeyPause = 0;
      m_oStateGame->m_oStateAttack.SoftKey(Canvas::KEY_NUM0);
    } else if (m_oStateGame->m_iGameState == 3) {
      m_oStateGame->m_oStateBoard.m_bRetreat = true;
      m_oStateGame->m_oStateBoard.m_iKeyPause = 0;
      m_oStateGame->m_oStateBoard.SoftKey(Canvas::KEY_NUM0);
    }
    m_oStateGame->SaveGame();
    Init(1);
    m_cGameMode = 1;
  } else {
    m_oKaperApp->destroyApp(false);
    m_oKaperApp->notifyDestroyed();
  }
}

void KaperCanvas::keyPressed(int keyCode) {
  if (0 == m_cGameMode) {
    m_lIntroWait -= 3000;
  }

  if (2 == m_cGameMode) {
    if (m_oStateGame != nullptr) {
      if (m_oStateGame->m_bLocked == false) {
        m_oStateGame->NormalButton(keyCode);
      }
    }
  }
  if (1 == m_cGameMode) {
    if (m_oStateMenu != nullptr) {
      if (m_oStateMenu->m_bLocked == false) {
        m_oStateMenu->NormalButton(keyCode);
      }
    }
  }

  if (Canvas::KEY_STAR == keyCode || Canvas::KEY_POUND == keyCode) {
    if (m_bWantToQuit == true) {
      if (m_oKaperApp->m_oKaperCanvas != nullptr) {
        ShutDown();
      } else {
        m_oKaperApp->destroyApp(false);
        m_oKaperApp->notifyDestroyed();
      }

    } else {
      if (2 == m_cGameMode) {
        if (m_oStateGame->m_bShipMoving != true && m_oStateGame->m_bMapMoving != true &&
            m_oStateGame->m_bKeyUp == false && m_oStateGame->m_bKeyDown == false &&
            m_oStateGame->m_bKeyRight == false && m_oStateGame->m_bKeyLeft == false) {
          m_bWantToQuit = true;
        }
      } else {
        m_bWantToQuit = true;
      }
    }
  } else {
    m_bWantToQuit = false;
  }
}

void KaperCanvas::keyReleased(int keyCode) {
  if (6 == m_cGameMode && keyCode == Canvas::KEY_NUM0) {
    ShutDown();
  }

  if (2 == m_cGameMode) {
    if (m_oStateGame != nullptr) {
      if (m_oStateGame->m_bLocked == false) {
        if (m_oStateGame->m_iGameState == 4) {
          m_oStateGame->m_oStateAttack.SoftKeyRelease(keyCode);
        } else if (m_oStateGame->m_iGameState == 3) {
          m_oStateGame->m_oStateBoard.SoftKeyRelease(keyCode);
        } else {
          m_oStateGame->NormalButton(128);
        }
      }
    }
  }
  if (1 == m_cGameMode) {
    if (m_oStateMenu != nullptr) {
      if (m_oStateMenu->m_bLocked == false) {
        m_oStateMenu->NormalButton(128);
      }
    }
  }
}

void KaperCanvas::DrawString(std::string str, int x, int y, bool bBold, int iColor) {
  const auto sText = to_upper(str);

  if (m_bFontBitmap == false || !m_oImageArray[24]) {
    if (bBold) {
      m_oFrontBuffer->setFont(m_oFontMediumB);
    } else {
      m_oFrontBuffer->setFont(m_oFontSmallP);
    }
    if (iColor == 1) {
      m_oFrontBuffer->setColor(255, 255, 255);
    } else if (iColor == 0) {
      m_oFrontBuffer->setColor(0, 0, 0);
    } else if (iColor == 2) {
      m_oFrontBuffer->setColor(255, 0, 0);
    } else if (iColor >= 3) {
      m_oFrontBuffer->setColor(200, 200, 0);
    }

    y += 2;

    m_oFrontBuffer->drawString(sText.c_str(), x, y, Graphics::TOP | Graphics::LEFT);
  } else {
    int iXoffset = 0;
    int iYoffset = 0;
    bool bDraw = false;

    y += 5;

    for (int i = 0; i < static_cast<int>(sText.length()); i++) {
      iYoffset = 0;

      if (sText[i] >= '0' && sText[i] <= ':') {
        iXoffset = (sText[i] - '0') * 5;
      } else if (sText[i] >= 'A' && sText[i] <= 'H') {
        iXoffset = (sText[i] - 'A') * 5 + 55;
      } else if (sText[i] >= 'I' && sText[i] <= 'Z') {
        iXoffset = (sText[i] - 'I') * 5;
        iYoffset = 7;
      } else if (sText[i] == '.') {
        bDraw = true;
      } else if (sText[i] == '[') {
        bDraw = true;
      } else if (sText[i] == ']') {
        bDraw = true;
      } else if (sText[i] == '<') {
        bDraw = true;
      } else if (sText[i] == '>') {
        bDraw = true;
      } else {
        continue;
      }

      if (iColor == 1) {
        iYoffset += 14;
      }
      if (iColor == 2) {
        iYoffset += 28;
      }
      if (iColor >= 3) {
        iYoffset += 42;
      }

      m_oFrontBuffer->setClip(x + (i * 5), y, 4, 7);

      if (bDraw) {
        if (iColor == 1) {
          m_oFrontBuffer->setColor(255, 255, 255);
        } else if (iColor == 0) {
          m_oFrontBuffer->setColor(0, 0, 0);
        } else if (iColor >= 2) {
          m_oFrontBuffer->setColor(255, 0, 0);
        }
        m_oFrontBuffer->setClip(0, 0, m_iWidth, m_iHeight);

        if (sText[i] == '.') {
          m_oFrontBuffer->drawLine(x + 1 + (i * 5), y + 5, x + (i * 5) + 2, y + 5);
          m_oFrontBuffer->drawLine(x + 1 + (i * 5), y + 6, x + (i * 5) + 2, y + 6);
        } else if (sText[i] == '>') {
          m_oFrontBuffer->drawLine(x + (i * 5), y, x + (i * 5) + 4, y + 3);
          m_oFrontBuffer->drawLine(x + (i * 5), y + 6, x + (i * 5) + 4, y + 3);
        } else if (sText[i] == '<') {
          m_oFrontBuffer->drawLine(x + (i * 5), y + 3, x + (i * 5) + 4, y);
          m_oFrontBuffer->drawLine(x + (i * 5), y + 3, x + (i * 5) + 4, y + 6);
        } else if (sText[i] == '[') {
          m_oFrontBuffer->drawLine(x + 1 + (i * 5), y, 1 + x + (i * 5), y + 6);
          m_oFrontBuffer->drawLine(x + 1 + (i * 5), y, x + (i * 5) + 3, y);
          m_oFrontBuffer->drawLine(x + 1 + (i * 5), y + 6, x + (i * 5) + 3, y + 6);
        } else if (sText[i] == ']') {
          m_oFrontBuffer->drawLine(x + 3 + (i * 5), y, x + 3 + (i * 5), y + 6);
          m_oFrontBuffer->drawLine(x + 3 + (i * 5), y, x + 1 + (i * 5), y);
          m_oFrontBuffer->drawLine(x + 3 + (i * 5), y + 6, x + 1 + (i * 5), y + 6);
        }

        bDraw = false;
        continue;
      } else {
        m_oFrontBuffer->drawImage(
            m_oImageArray[24], x + (i * 5) - iXoffset, y - iYoffset, Graphics::TOP | Graphics::LEFT);
      }

      if (bBold) {
        m_oFrontBuffer->setClip(1 + x + (i * 5), y, 4, 7);
        m_oFrontBuffer->drawImage(
            m_oImageArray[24], 1 + x + (i * 5) - iXoffset, y - iYoffset, Graphics::TOP | Graphics::LEFT);
      }
    }

    m_oFrontBuffer->setClip(0, 0, m_iWidth, m_iHeight);
  }
}

void KaperCanvas::DrawString(int iNumbers, int x, int y, bool bBold, int iColor) {
  m_stringBuffer->remove(0, m_stringBuffer->length());
  m_stringBuffer->append(iNumbers);
  m_stringBuffer->getChars(0, m_stringBuffer->length(), m_stringChars, 0);

  if (m_bFontBitmap == false || !m_oImageArray[24]) {
    if (bBold) {
      m_oFrontBuffer->setFont(m_oFontMediumB);
    } else {
      m_oFrontBuffer->setFont(m_oFontSmallP);
    }
    if (iColor == 1) {
      m_oFrontBuffer->setColor(255, 255, 255);
    } else if (iColor == 0) {
      m_oFrontBuffer->setColor(0, 0, 0);
    } else if (iColor == 2) {
      m_oFrontBuffer->setColor(255, 0, 0);
    } else if (iColor >= 3) {
      m_oFrontBuffer->setColor(200, 200, 0);
    }

    y += 2;

    m_oFrontBuffer->drawChars(m_stringChars.data(), 0, m_stringBuffer->length(), x, y, Graphics::TOP | Graphics::LEFT);
  } else {
    int iXoffset = 0;
    int iYoffset = 0;

    y += 5;
    x += 1;

    for (int i = 0; i < 4; i++) {
      iYoffset = 0;

      if (static_cast<int>(m_stringBuffer->length()) <= i) {
        break;
      }

      if (m_stringChars[i] >= '0' && m_stringChars[i] <= '9') {
        iXoffset = (m_stringChars[i] - '0') * 5;
      } else {
        continue;
      }

      if (iColor == 1) {
        iYoffset += 14;
      }
      if (iColor == 2) {
        iYoffset += 28;
      }
      if (iColor >= 3) {
        iYoffset += 42;
      }

      m_oFrontBuffer->setClip(x + (i * 5), y, 4, 7);
      m_oFrontBuffer->drawImage(
          m_oImageArray[24], x + (i * 5) - iXoffset, y - iYoffset, Graphics::TOP | Graphics::LEFT);
    }
  }
  m_oFrontBuffer->setClip(0, 0, m_iWidth, m_iHeight);
}

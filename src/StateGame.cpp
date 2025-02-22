#include "Graphics.h"
#include "StateGame.h"
#include "Canvas.h"
#include "Connector.h"
#include "GameStateCity.h"
#include "Image.h"
#include "Random.h"
#include "RecordStore.h"
#include "Thread.h"

StateGame::StateGame(KaperCanvas* oKaperCanvas)
  : m_oStateBoard{oKaperCanvas}
  , m_oStateAttack{oKaperCanvas}
  , m_oStateCity{oKaperCanvas}
  , m_oStateSailingToCity{oKaperCanvas}
  , m_oCanvas{oKaperCanvas} {
  m_bLocked = true;

  ////////////////////////////////////////////////////////////
  // Skies
  ////////////////////////////////////////////////////////////
  m_iSkyX = std::vector<int>(19);
  m_iSkyY = std::vector<int>(19);
  m_iSkyType = std::vector<int>(19);

  ////////////////////////////////////////////////////////////
  // KaperShip
  ////////////////////////////////////////////////////////////
  m_iShipWaveA = std::vector<int>(3);
  m_iShipWaveX = std::vector<int>(3);
  m_iShipWaveY = std::vector<int>(3);

  ////////////////////////////////////////////////////////////
  // StateGame
  ////////////////////////////////////////////////////////////
  m_iEventX = std::vector<int>(15);
  m_iEventY = std::vector<int>(15);

  m_bShotButtonGuide = true;

  for (int i = 0; i < 19; i++) {
    m_iSkyType[i] = std::abs(m_oCanvas->m_oRand->nextInt() % 3);

    m_iSkyX[i] = -50 + std::abs(m_oCanvas->m_oRand->nextInt() % (29 * 25));
    m_iSkyY[i] = std::abs(m_oCanvas->m_oRand->nextInt() % (14 * 25));
  }

  m_aoGameMap = std::vector<std::vector<char>>{
      std::vector<char>{1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
      std::vector<char>{1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
      std::vector<char>{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 1, 1, 1},
      std::vector<char>{1, 1, 1, 0, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
      std::vector<char>{1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
      std::vector<char>{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1},
      std::vector<char>{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 5, 0, 1, 1, 1},
      std::vector<char>{1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 6, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
      std::vector<char>{1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
      std::vector<char>{0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1},
      std::vector<char>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
      std::vector<char>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 0, 0, 0, 0},
      std::vector<char>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
      std::vector<char>{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}};

  ////////////////////////////////////////////////////////////
  // EnemyShip
  ////////////////////////////////////////////////////////////
  m_iEnemyShipX = std::vector<int>(10);
  m_iEnemyShipY = std::vector<int>(10);
  m_iEnemyShipShipType = std::vector<int>(10);
  m_iEnemyShipCannons = std::vector<int>(10);
  m_iEnemyShipSoldiers = std::vector<int>(10);
  m_iEnemyShipGraphicWay = std::vector<short>(10);

  m_iEnemyShipWaveA = std::vector<std::vector<int>>(10, std::vector<int>(3));
  m_iEnemyShipWaveX = std::vector<std::vector<int>>(10, std::vector<int>(3));
  m_iEnemyShipWaveY = std::vector<std::vector<int>>(10, std::vector<int>(3));

  //------------------------------------//
  //----------- MAP / States -----------//
  //------------------------------------//

  m_oCanvas->m_oImageArray[4] = Image::createImage("/5.png");
  m_oCanvas->m_oImageArray[3] = Image::createImage("/4.png");
  m_oCanvas->m_oImageArray[23] = Image::createImage("/7.png");

  if (m_oCanvas->m_bSpeedOptimized == false) {
    m_oCanvas->m_oImageArray[2] = Image::createImage("/31.png");
    m_oCanvas->m_oImageArray[6] = Image::createImage("/32.png");
    m_oCanvas->m_oImageArray[7] = Image::createImage("/33.png");
    m_oCanvas->m_oImageArray[8] = Image::createImage("/34.png");
    m_oCanvas->m_oImageArray[9] = Image::createImage("/35.png");
    m_oCanvas->m_oImageArray[10] = Image::createImage("/36.png");
    m_oCanvas->m_oImageArray[11] = Image::createImage("/37.png");
    m_oCanvas->m_oImageArray[12] = Image::createImage("/38.png");
    m_oCanvas->m_oImageArray[13] = Image::createImage("/39.png");
    m_oCanvas->m_oImageArray[14] = Image::createImage("/310.png");
    m_oCanvas->m_oImageArray[15] = Image::createImage("/311.png");
    m_oCanvas->m_oImageArray[16] = Image::createImage("/312.png");
    m_oCanvas->m_oImageArray[17] = Image::createImage("/313.png");
    m_oCanvas->m_oImageArray[18] = Image::createImage("/314.png");
    m_oCanvas->m_oImageArray[19] = Image::createImage("/315.png");
    m_oCanvas->m_oImageArray[20] = Image::createImage("/316.png");
    m_oCanvas->m_oImageArray[21] = Image::createImage("/317.png");
    m_oCanvas->m_oImageArray[22] = Image::createImage("/318.png");

    m_oCanvas->m_oImageArray[5] = Image::createImage("/6.png");
  } else {
    m_oCanvas->m_oImageArray[2] = Image::createImage(0, 0); // nullptr;
    m_oCanvas->m_oImageArray[5] = Image::createImage(0, 0); // nullptr;
  }
}

void StateGame::Init() {
  m_bLocked = true;

  m_bWaitingOnMove = false;
  m_bClose = false;
  m_bToCity = false;
  m_bWait = false;

  game_state = StateGame::state::board;

  m_iWebRankShow = 1;

  m_sPlayerName = "a";

  ////////////////////////////////////////////////////////////
  // Map
  ////////////////////////////////////////////////////////////
  m_iMapScreenX = 7;
  m_iMapScreenY = 3;

  ////////////////////////////////////////////////////////////
  // Event
  ////////////////////////////////////////////////////////////
  m_iEventOld = -1;

  m_sEventMsg1 = "Empty...";
  m_sEventMsg2 = "";

  ////////////////////////////////////////////////////////////
  // KaperShip
  ////////////////////////////////////////////////////////////
  m_bShipMoving = false;

  m_iShipX = 25;
  m_iShipY = 25;

  m_iShipGraphicWay = 1;

  ////////////////////////////////////////////////////////////
  // Interface
  ////////////////////////////////////////////////////////////
  m_iResourceGold = 25;
  m_iResourceCorn = 100;
  m_iResourceCannon = 5;
  m_iResourcePirate = 30;
  m_iShipLvl = 1; // 1 standard
  m_iCapturedShip = 0;

  m_iAddFood = 0;
  m_iAddGold = 0;
  m_iAddPirates = 0;
  m_iAddCannons = 0;

  m_iTurn = 0;
  m_iScore = 0;
  m_iScoreTurn100 = 0;
  m_iScoreTurn300 = 0;

  for (int i = 0; i < 15; i++) {
    if (m_iEventX[i] != 0 && m_iEventY[i] != 0) {
      continue;
    }
    do {
      m_iEventX[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (29));
      m_iEventY[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (14));
    } while (MapCheckCollision((m_iEventX[i] / 25) + 0, (m_iEventY[i] / 25) - 0, false) == 0 ||
             ((m_iEventX[i] / 25) == 8 && (m_iEventY[i] / 25) == 4));
  }

  ////////////////////////////////////////////////////////////
  // EnemyShip
  ////////////////////////////////////////////////////////////
  int iRandom;

  for (int i = 0; i < 10; i++) {
    // Placing random place
    do {
      m_iEnemyShipX[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (29));
      m_iEnemyShipY[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (14));
    } while (m_oCanvas->m_oStateGame->MapCheckCollision(
                 (m_iEnemyShipX[i] / 25) + 0, (m_iEnemyShipY[i] / 25) - 0, false) == 0 ||
             ((m_iEnemyShipX[i] / 25) == 8 && (m_iEnemyShipY[i] / 25) == 4));

    m_iEnemyShipGraphicWay[i] = 1;

    iRandom = std::abs(m_oCanvas->m_oRand->nextInt() % (m_iShipLvl + 1));

    m_iEnemyShipShipType[i] = 12 + iRandom;

    if (iRandom == 0) {
      m_iEnemyShipCannons[i] =
          (std::abs(m_oCanvas->m_oRand->nextInt() % 3) + 1 + ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 10) + 5 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    }

    else if (iRandom == 1) {
      m_iEnemyShipCannons[i] =
          (std::abs(m_oCanvas->m_oRand->nextInt() % 5) + 2 + ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 21) + 15 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    } else if (iRandom == 2) {
      m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 14) + 13 +
                                ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 26) + 35 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    } else if (iRandom == 3) {
      m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 26) + 25 +
                                ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 71) + 40 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    } else if (iRandom == 4) {
      m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 51) + 30 +
                                ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 351) + 100 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    } else if (iRandom == 5) {
      m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 101) + 100 +
                                ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 501) + 500 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    } else //(iRandom == 6)
    {
      m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 101) + 50 +
                                ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
      m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 501) + 50 +
                                 ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
    }
  }

  //------------------------------------//
  //-------------- States --------------//
  //------------------------------------//

  m_oCanvas->StopSound();

  m_bLocked = false;
}

bool StateGame::MapIncSetXY(int iWay) {
  if (m_bMapMoving == true) {
    return false;
  }

  // Check if Moving map is allowed
  if (iWay == 1) {
    if (((m_oCanvas->m_iWidth - 20) / 25) + m_iMapScreenX > 27) {
      return true;
    }
  }
  if (iWay == 2) {
    if (m_iMapScreenX <= 0) {
      return true;
    }
  }
  if (iWay == 8) {
    if (((m_oCanvas->m_iHeight - 20) / 25) + m_iMapScreenY > 13) {
      return true;
    }
  }
  if (iWay == 4) {
    if (m_iMapScreenY <= 0) {
      return true;
    }
  }

  m_bMapScrollN = false;
  m_bMapScrollS = false;
  m_bMapScrollE = false;
  m_bMapScrollW = false;

  if (iWay == 0x20) {
    if (!(m_iMapScreenY <= 0)) {
      m_bMapScrollN = true;
    }
    if (!(m_iMapScreenX <= 0)) {
      m_bMapScrollW = true;
    }

    if (m_iShipY > 20) {
      m_bMapScrollS = false;
    }
    if (m_iShipX > 20) {
      m_bMapScrollE = false;
    }

    if (m_bMapScrollN && m_bMapScrollW) {
      m_bMapScrollN = false;
      m_bMapScrollW = false;
    } else {
      if (!m_bMapScrollN && m_bMapScrollW) {
        ShipMove(4, true);
      }
      if (!m_bMapScrollW && m_bMapScrollN) {
        ShipMove(2, true);
      }
      if (!m_bMapScrollN && !m_bMapScrollW) {
        ShipMove(0x20, true);
        return true;
      }
      m_iShipGraphicWay = 0x20;
    }
  }

  if (iWay == 0x80) {
    if (!(m_iMapScreenY <= 0)) {
      m_bMapScrollN = true;
    }
    if (!(((m_oCanvas->m_iWidth - 20) / 25) + m_iMapScreenX > 27)) {
      m_bMapScrollE = true;
    }

    if (m_iShipY > 20) {
      m_bMapScrollS = false;
    }
    if (m_iShipX < 20) {
      m_bMapScrollE = false;
    }

    if (m_bMapScrollN && m_bMapScrollE) {
      m_bMapScrollN = false;
      m_bMapScrollE = false;
    } else {
      if (!m_bMapScrollN && m_bMapScrollE) {
        ShipMove(4, true);
      }
      if (!m_bMapScrollE && m_bMapScrollN) {
        ShipMove(1, true);
      }
      if (!m_bMapScrollN && !m_bMapScrollE) {
        ShipMove(0x80, true);
        return true;
      }
      m_iShipGraphicWay = 0x80;
    }
  }

  if (iWay == 0x10) {
    if (!(((m_oCanvas->m_iHeight - 20) / 25) + m_iMapScreenY > 13)) {
      m_bMapScrollS = true;
    }
    if (!(m_iMapScreenX <= 0)) {
      m_bMapScrollW = true;
    }

    if (m_iShipY < 20) {
      m_bMapScrollS = false;
    }
    if (m_iShipX > 20) {
      m_bMapScrollE = false;
    }

    if (m_bMapScrollS && m_bMapScrollW) {
      m_bMapScrollS = false;
      m_bMapScrollW = false;
    } else {
      if (!m_bMapScrollS && m_bMapScrollW) {
        ShipMove(8, true);
      }
      if (!m_bMapScrollW && m_bMapScrollS) {
        ShipMove(2, true);
      }
      if (!m_bMapScrollS && !m_bMapScrollW) {
        ShipMove(0x10, true);
        return true;
      }
      m_iShipGraphicWay = 0x10;
    }
  }

  if (iWay == 0x40) {
    if (!(((m_oCanvas->m_iHeight - 20) / 25) + m_iMapScreenY > 13)) {
      m_bMapScrollS = true;
    }
    if (!(((m_oCanvas->m_iWidth - 20) / 25) + m_iMapScreenX > 27)) {
      m_bMapScrollE = true;
    }

    if (m_iShipY < 20) {
      m_bMapScrollS = false;
    }
    if (m_iShipX < 20) {
      m_bMapScrollE = false;
    }

    if (m_bMapScrollS && m_bMapScrollE) {
      m_bMapScrollS = false;
      m_bMapScrollE = false;
    } else {
      if (!m_bMapScrollS && m_bMapScrollE) {
        ShipMove(8, true);
      }
      if (!m_bMapScrollE && m_bMapScrollS) {
        ShipMove(1, true);
      }
      if (!m_bMapScrollS && !m_bMapScrollE) {
        ShipMove(0x40, true);
        return true;
      }
      m_iShipGraphicWay = 0x40;
    }
  }

  // Move Screen
  m_iMapOffScreenX = 0;
  m_iMapOffScreenY = 0;
  m_iMapWay = iWay;
  m_bMapMoving = true;
  return false;
}

int StateGame::MapCheckCollision(int iX, int iY, bool bScreenOffset) {
  if (bScreenOffset == true) {
    iX += m_iMapScreenX;
    iY += m_iMapScreenY;
  }

  if (iX < 0 || iY < 0) {
    return 0;
  }
  if (iX > 28 || iY > 13) {
    return 0;
  }

  if (0 == m_aoGameMap[iY][iX]) {
    return 1;
  }
  if (1 < m_aoGameMap[iY][iX]) {
    return 2;
  }

  return 0;
}

void StateGame::MapDraw(Graphics* g) {
  if (m_oCanvas->m_oImageArray[2] && m_oCanvas->m_oImageArray[22]) {
    int iPointx, iPointy;

    iPointx = -((m_iMapScreenX * 25) - m_iMapOffScreenX);
    iPointy = -((m_iMapScreenY * 25) - m_iMapOffScreenY);

    if (((iPointx + 128) >= 0 && iPointx <= m_oCanvas->m_iWidth) &&
        ((iPointy + 128) >= 0 && iPointy <= m_oCanvas->m_iHeight)) {
      g->drawImage(m_oCanvas->m_oImageArray[2], iPointx, iPointy, Graphics::TOP | Graphics::LEFT);
    }

    int i;
    for (i = 1; i < 6; i++) {
      iPointx = (i * 128) - ((m_iMapScreenX * 25) - m_iMapOffScreenX);
      iPointy = -((m_iMapScreenY * 25) - m_iMapOffScreenY);

      if (((iPointx + 128) >= 0 && iPointx <= m_oCanvas->m_iWidth) &&
          ((iPointy + 128) >= 0 && iPointy <= m_oCanvas->m_iHeight)) {
        g->drawImage(m_oCanvas->m_oImageArray[5 + i], iPointx, iPointy, Graphics::TOP | Graphics::LEFT);
      }
    }
    for (i = 0; i < 6; i++) {
      iPointx = (i * 128) - ((m_iMapScreenX * 25) - m_iMapOffScreenX);
      iPointy = (128) - ((m_iMapScreenY * 25) - m_iMapOffScreenY);

      if (((iPointx + 128) >= 0 && iPointx <= m_oCanvas->m_iWidth) &&
          ((iPointy + 128) >= 0 && iPointy <= m_oCanvas->m_iHeight)) {
        g->drawImage(m_oCanvas->m_oImageArray[11 + i], iPointx, iPointy, Graphics::TOP | Graphics::LEFT);
      }
    }
    for (i = 0; i < 6; i++) {
      iPointx = (i * 128) - ((m_iMapScreenX * 25) - m_iMapOffScreenX);
      iPointy = (256) - ((m_iMapScreenY * 25) - m_iMapOffScreenY);

      if (((iPointx + 128) >= 0 && iPointx <= m_oCanvas->m_iWidth) &&
          ((iPointy + 128) >= 0 && iPointy <= m_oCanvas->m_iHeight)) {
        g->drawImage(m_oCanvas->m_oImageArray[17 + i], iPointx, iPointy, Graphics::TOP | Graphics::LEFT);
      }
    }
  }

  else {
    ///////////////////////////////
    // Optimized Map
    ///////////////////////////////

    // Non speed Version
    if (!m_oCanvas->m_bSpeedOptimized) {
      for (int iY = -1; iY < (14 - m_iMapScreenY) + 1; iY++) {
        for (int iX = -1; iX < (29 - m_iMapScreenX) + 1; iX++) {
          if ((iY + m_iMapScreenY) < 0) {
            continue;
          }
          if ((iX + m_iMapScreenX) < 0) {
            continue;
          }
          if ((iY + m_iMapScreenY) >= 14) {
            continue;
          }
          if ((iX + m_iMapScreenX) >= 29) {
            continue;
          }

          if ((m_oCanvas->m_iWidth / 25) < iX) {
            continue;
          }
          if ((m_oCanvas->m_iHeight / 25) < iY) {
            continue;
          }

          g->setClip((iX * 25) + m_iMapOffScreenX, (iY * 25) + m_iMapOffScreenY, 25, 25);
          if (m_aoGameMap[iY + m_iMapScreenY][iX + m_iMapScreenX] == 0) {
            g->drawImage(m_oCanvas->m_oImageArray[23],
                         (iX * 25) + m_iMapOffScreenX,
                         (iY * 25) + m_iMapOffScreenY,
                         Graphics::TOP | Graphics::LEFT);
          } else if (m_aoGameMap[iY + m_iMapScreenY][iX + m_iMapScreenX] == 1) {
            g->drawImage(m_oCanvas->m_oImageArray[23],
                         (iX * 25) + m_iMapOffScreenX - 26,
                         (iY * 25) + m_iMapOffScreenY,
                         Graphics::TOP | Graphics::LEFT);
          } else {
            g->drawImage(m_oCanvas->m_oImageArray[23],
                         (iX * 25) + m_iMapOffScreenX - 50,
                         (iY * 25) + m_iMapOffScreenY,
                         Graphics::TOP | Graphics::LEFT);
          }
        }
      }

    }

    // Speed Version
    else {
      g->setColor(0, 0, 200);
      g->fillRect(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

      for (int iY = -1; iY < (14 - m_iMapScreenY) + 1; iY++) {
        for (int iX = -1; iX < (29 - m_iMapScreenX) + 1; iX++) {
          if ((iY + m_iMapScreenY) < 0) {
            continue;
          }
          if ((iX + m_iMapScreenX) < 0) {
            continue;
          }
          if ((iY + m_iMapScreenY) >= 14) {
            continue;
          }
          if ((iX + m_iMapScreenX) >= 29) {
            continue;
          }

          if ((m_oCanvas->m_iWidth / 25) < iX) {
            continue;
          }
          if ((m_oCanvas->m_iHeight / 25) < iY) {
            continue;
          }

          if (m_aoGameMap[iY + m_iMapScreenY][iX + m_iMapScreenX] == 0) {
            continue;
          }
          if (m_aoGameMap[iY + m_iMapScreenY][iX + m_iMapScreenX] == 1) {
            g->setColor(0, 200, 0);
          }
          if (m_aoGameMap[iY + m_iMapScreenY][iX + m_iMapScreenX] > 1) {
            g->setColor(200, 0, 0);
          }
          g->fillRect((iX * 25) + m_iMapOffScreenX, (iY * 25) + m_iMapOffScreenY, 25, 25);
        }
      }
    }
  }
}

void StateGame::MapUpdate() {
  if (m_bMapMoving == false) {
    return;
  }
  m_bShipMoving = false;

  if (4 == m_iMapWay || m_bMapScrollN) {
    m_iMapOffScreenY++;
    if (m_iShipGraphicWay == 0x80) {
      m_iShipX++;
    }
    if (m_iShipGraphicWay == 0x20) {
      m_iShipX--;
    }

    if (m_iMapOffScreenY >= 25) {
      m_iMapScreenY--;
      m_bMapMoving = false;
      m_iMapOffScreenY = 0;
      return;
    }
  }

  else if (8 == m_iMapWay || m_bMapScrollS) {
    m_iMapOffScreenY--;
    if (m_iShipGraphicWay == 0x40) {
      m_iShipX++;
    }
    if (m_iShipGraphicWay == 0x10) {
      m_iShipX--;
    }

    if (m_iMapOffScreenY <= -25) {
      m_iMapScreenY++;
      m_bMapMoving = false;
      m_iMapOffScreenY = 0;
      return;
    }
  }

  else if (2 == m_iMapWay || m_bMapScrollW) {
    m_iMapOffScreenX++;
    if (m_iShipGraphicWay == 0x20) {
      m_iShipY--;
    }
    if (m_iShipGraphicWay == 0x10) {
      m_iShipY++;
    }

    if (m_iMapOffScreenX >= 25) {
      m_iMapScreenX--;
      m_bMapMoving = false;
      m_iMapOffScreenX = 0;
      return;
    }
  }

  else if (1 == m_iMapWay || m_bMapScrollE) {
    m_iMapOffScreenX--;
    if (m_iShipGraphicWay == 0x80) {
      m_iShipY--;
    }
    if (m_iShipGraphicWay == 0x40) {
      m_iShipY++;
    }

    if (m_iMapOffScreenX <= -25) {
      m_iMapScreenX++;
      m_bMapMoving = false;
      m_iMapOffScreenX = 0;
      return;
    }

  }

  else if (0x20 == m_iMapWay) {
    m_iMapOffScreenX++;
    m_iMapOffScreenY++;

    if (m_iMapOffScreenX >= 25) {
      m_iMapScreenX--;
      m_iMapScreenY--;
      m_bMapMoving = false;
      m_iMapOffScreenX = 0;
      m_iMapOffScreenY = 0;
      return;
    }

  } else if (0x80 == m_iMapWay) {
    m_iMapOffScreenX--;
    m_iMapOffScreenY++;

    if (m_iMapOffScreenX <= -25) {
      m_iMapScreenX++;
      m_iMapScreenY--;
      m_bMapMoving = false;
      m_iMapOffScreenX = 0;
      m_iMapOffScreenY = 0;
      return;
    }
  } else if (0x40 == m_iMapWay) {
    m_iMapOffScreenX--;
    m_iMapOffScreenY--;

    if (m_iMapOffScreenX <= -25) {
      m_iMapScreenX++;
      m_iMapScreenY++;
      m_bMapMoving = false;
      m_iMapOffScreenX = 0;
      m_iMapOffScreenY = 0;
      return;
    }
  } else if (0x10 == m_iMapWay) {
    m_iMapOffScreenX++;
    m_iMapOffScreenY--;

    if (m_iMapOffScreenX >= 25) {
      m_iMapScreenX--;
      m_iMapScreenY++;
      m_bMapMoving = false;
      m_iMapOffScreenX = 0;
      m_iMapOffScreenY = 0;
      return;
    }
  }
}

void StateGame::ShipDraw(Graphics* g) {
  int iGraphic = 0;

  if (m_oCanvas->m_bSpeedOptimized == false) {
    for (int i = 0; i < 3; i++) {
      if (m_iShipWaveA[0] == 18) {
        m_iShipWaveA[1] = 25;
      }
      if (m_iShipWaveA[0] == 10) {
        m_iShipWaveA[2] = 25;
      }

      if (m_iShipWaveA[i] < 0) {
        continue;
      }
      if (m_iShipWaveA[i] == 25) {
        m_iShipWaveX[i] = m_iShipX; // - m_iMapOffScreenX;
        m_iShipWaveY[i] = m_iShipY; // - m_iMapOffScreenY;

        if (m_iShipGraphicWay == 1 || m_iShipGraphicWay == 2) {
          m_iShipWaveY[i] += 5;
        }
      }

      if (m_iShipWaveA[i] < 6) {
        iGraphic = 3;
      } else if (m_iShipWaveA[i] < 12) {
        iGraphic = 2;
      } else if (m_iShipWaveA[i] < 18) {
        iGraphic = 1;
      } else if (m_iShipWaveA[i] < 25) {
        iGraphic = 0;
      }

      m_iShipWaveA[i]--;
      if (m_iShipWaveA[i] >= 25) {
        continue;
      }

      int iOffsetX = m_iMapOffScreenX;
      int iOffsetY = m_iMapOffScreenY;

      if (iOffsetX < 0) {
        m_iShipWaveX[i]--;
      }
      if (iOffsetX > 0) {
        m_iShipWaveX[i]++;
      }
      if (iOffsetY < 0) {
        m_iShipWaveY[i]--;
      }
      if (iOffsetY > 0) {
        m_iShipWaveY[i]++;
      }

      g->setClip(m_iShipWaveX[i], m_iShipWaveY[i], 19, 19);

      if (iGraphic == 0) {
        g->drawImage(
            m_oCanvas->m_oImageArray[0], m_iShipWaveX[i] - 100, m_iShipWaveY[i] - 60, Graphics::TOP | Graphics::LEFT);
      }
      if (iGraphic == 1) {
        g->drawImage(
            m_oCanvas->m_oImageArray[0], m_iShipWaveX[i] - 80, m_iShipWaveY[i] - 60, Graphics::TOP | Graphics::LEFT);
      }
      if (iGraphic == 2) {
        g->drawImage(
            m_oCanvas->m_oImageArray[0], m_iShipWaveX[i] - 60, m_iShipWaveY[i] - 60, Graphics::TOP | Graphics::LEFT);
      }
      if (iGraphic == 3) {
        g->drawImage(
            m_oCanvas->m_oImageArray[0], m_iShipWaveX[i] - 40, m_iShipWaveY[i] - 60, Graphics::TOP | Graphics::LEFT);
      }
    }
  }

  g->setClip(m_iShipX, m_iShipY, 19, 19);

  if (2 >= m_iShipLvl) {
    if (m_iShipGraphicWay == 1) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 140, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 2) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 8) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 60, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 4) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 120, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x10) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 20, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x40) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 40, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x80) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 100, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x20) {
      g->drawImage(m_oCanvas->m_oImageArray[0], m_iShipX - 80, m_iShipY - 80, Graphics::TOP | Graphics::LEFT);
    }
  }

  else if (2 < m_iShipLvl && 4 >= m_iShipLvl) {
    if (m_iShipGraphicWay == 1) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 140, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 2) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 8) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 60, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 4) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 120, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x10) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 20, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x40) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 40, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x80) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 100, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x20) {
      g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 80, m_iShipY - 60, Graphics::TOP | Graphics::LEFT);
    }
  }

  else if (4 < m_iShipLvl) {
    if (m_iShipGraphicWay == 1) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 140, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 2) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 8) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 60, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 4) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 120, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x10) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 20, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x40) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 40, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x80) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 100, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iShipGraphicWay == 0x20) {
      g->drawImage(m_oCanvas->m_oImageArray[4], m_iShipX - 80, m_iShipY, Graphics::TOP | Graphics::LEFT);
    }
  }
}

void StateGame::ShipUpdate() {
  if (m_bShipMoving == true && false == m_bMapMoving) {
    if (m_iShipGraphicWay == 2) {
      m_iShipX--;
    }
    if (m_iShipGraphicWay == 1) {
      m_iShipX++;
    }
    if (m_iShipGraphicWay == 4) {
      m_iShipY--;
    }
    if (m_iShipGraphicWay == 8) {
      m_iShipY++;
    }

    if (m_iShipGraphicWay == 0x80) {
      m_iShipY--;
      m_iShipX++;
    }
    if (m_iShipGraphicWay == 0x20) {
      m_iShipY--;
      m_iShipX--;
    }
    if (m_iShipGraphicWay == 0x10) {
      m_iShipY++;
      m_iShipX--;
    }
    if (m_iShipGraphicWay == 0x40) {
      m_iShipY++;
      m_iShipX++;
    }

    if ((m_iShipX % 25) == 0 && (m_iShipY % 25) == 0) {
      m_bShipMoving = false;
    }
  }
}

int StateGame::ShipMove(int iWay, bool bForce) {
  if (!bForce && m_bShipMoving) {
    return 0x02;
  }

  m_bShipMoving = true;
  int iReturnValue = 0;

  m_iShipWaveA[0] = 25;

  if (iWay == 2) {
    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) - 1, (m_iShipY / 25) - 0, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) - 1, (m_iShipY / 25) - 0, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 2;

    if (!bForce && m_iShipX <= (25 * 2)) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 1) {
    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) + 1, (m_iShipY / 25) - 0, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) + 1, (m_iShipY / 25) - 0, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 1;

    if (!bForce && m_iShipX >= (m_oCanvas->m_iWidth - 60 - 25)) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 4) {
    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) - 0, (m_iShipY / 25) - 1, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) - 0, (m_iShipY / 25) - 1, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 4;

    if (!bForce && m_iShipY <= (25 * 2)) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 8) {

    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) - 0, (m_iShipY / 25) + 1, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) - 0, (m_iShipY / 25) + 1, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 8;

    if (!bForce && m_iShipY >= (m_oCanvas->m_iHeight - 60 - 20 - 25)) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 0x80) {

    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) + 1, (m_iShipY / 25) - 1, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) + 1, (m_iShipY / 25) - 1, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 0x80;

    if (!bForce && m_iShipY <= (25 * 2)) {
      return (iReturnValue | 0x01);
    }
    if (!bForce && m_iShipX >= m_oCanvas->m_iWidth - 60 - 25) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 0x20) {

    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) - 1, (m_iShipY / 25) - 1, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) - 1, (m_iShipY / 25) - 1, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 0x20;

    if (!bForce && m_iShipY <= (25 * 2)) {
      return (iReturnValue | 0x01);
    }
    if (!bForce && m_iShipX <= (25 * 2)) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 0x10) {

    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) - 1, (m_iShipY / 25) + 1, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) - 1, (m_iShipY / 25) + 1, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 0x10;

    if (!bForce && m_iShipY >= m_oCanvas->m_iHeight - 60 - 20 - 25) {
      return (iReturnValue | 0x01);
    }
    if (!bForce && m_iShipX <= (25 * 2)) {
      return (iReturnValue | 0x01);
    }
  }

  else if (iWay == 0x40) {

    // End of map or crashing to ground
    if (!bForce && 0 == MapCheckCollision((m_iShipX / 25) + 1, (m_iShipY / 25) + 1, true)) {
      m_bShipMoving = false;
      return 0x02;
    }

    // City collision
    if (!bForce && 2 == MapCheckCollision((m_iShipX / 25) + 1, (m_iShipY / 25) + 1, true)) {
      iReturnValue |= 0x04;
    }

    m_iShipGraphicWay = 0x40;

    if (!bForce && m_iShipY >= m_oCanvas->m_iHeight - 60 - 20 - 25) {
      return (iReturnValue | 0x01);
    }
    if (!bForce && m_iShipX >= m_oCanvas->m_iWidth - 60 - 25) {
      return (iReturnValue | 0x01);
    }
  }

  return iReturnValue;
}

void StateGame::EnemyShipUpdate() {
  for (int i = 0; i < 10; i++) {
    if (!((m_iEnemyShipX[i] % 25) == 0 && (m_iEnemyShipY[i] % 25) == 0)) {
      if (m_iEnemyShipGraphicWay[i] == 2) {
        m_iEnemyShipX[i]--;
      }
      if (m_iEnemyShipGraphicWay[i] == 1) {
        m_iEnemyShipX[i]++;
      }
      if (m_iEnemyShipGraphicWay[i] == 4) {
        m_iEnemyShipY[i]--;
      }
      if (m_iEnemyShipGraphicWay[i] == 8) {
        m_iEnemyShipY[i]++;
      }

      if (m_iEnemyShipGraphicWay[i] == 0x20) {
        m_iEnemyShipX[i]--;
        m_iEnemyShipY[i]--;
      }
      if (m_iEnemyShipGraphicWay[i] == 0x80) {
        m_iEnemyShipX[i]++;
        m_iEnemyShipY[i]--;
      }
      if (m_iEnemyShipGraphicWay[i] == 0x10) {
        m_iEnemyShipX[i]--;
        m_iEnemyShipY[i]++;
      }
      if (m_iEnemyShipGraphicWay[i] == 0x40) {
        m_iEnemyShipX[i]++;
        m_iEnemyShipY[i]++;
      }
    }
  }
}

void StateGame::EnemyShipMove() {
  int iWay;

  for (int i = 0; i < 10; i++) {
    //------------------------
    // Player near, then don't move
    //------------------------
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX + 1) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY)) {
      continue;
    }
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX - 1) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY)) {
      continue;
    }
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX + 0) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY + 1)) {
      continue;
    }
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX + 0) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY - 1)) {
      continue;
    }

    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX + 1) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY + 1)) {
      continue;
    }
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX - 1) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY - 1)) {
      continue;
    }
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX + 1) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY - 1)) {
      continue;
    }
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX - 1) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY + 1)) {
      continue;
    }

    //------------------------
    // Move random
    //------------------------
    iWay = std::abs(m_oCanvas->m_oRand->nextInt() % 15);

    if (iWay == 0) // North
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) + 0, (m_iEnemyShipY[i] / 25) - 1, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) + 0, (m_iEnemyShipY[i] / 25) - 1) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 4;

      m_iEnemyShipY[i]--;
    } else if (iWay == 1) // South
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) + 0, (m_iEnemyShipY[i] / 25) + 1, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) + 0, (m_iEnemyShipY[i] / 25) + 1) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 8;

      m_iEnemyShipY[i]++;
    } else if (iWay == 2) // East
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) + 1, (m_iEnemyShipY[i] / 25) - 0, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) + 1, (m_iEnemyShipY[i] / 25) - 0) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 1;

      m_iEnemyShipX[i]++;
    } else if (iWay == 3) // West
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) - 1, (m_iEnemyShipY[i] / 25) - 0, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) - 1, (m_iEnemyShipY[i] / 25) - 0) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 2;

      m_iEnemyShipX[i]--;
    } else if (iWay == 4) // 0x20
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) - 1, (m_iEnemyShipY[i] / 25) - 1, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) - 1, (m_iEnemyShipY[i] / 25) - 1) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 0x20;

      m_iEnemyShipX[i]--;
      m_iEnemyShipY[i]--;
    } else if (iWay == 5) // 0x80
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) + 1, (m_iEnemyShipY[i] / 25) - 1, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) + 1, (m_iEnemyShipY[i] / 25) - 1) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 0x80;

      m_iEnemyShipX[i]++;
      m_iEnemyShipY[i]--;
    } else if (iWay == 6) // 0x10
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) - 1, (m_iEnemyShipY[i] / 25) + 1, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) - 1, (m_iEnemyShipY[i] / 25) + 1) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 0x10;

      m_iEnemyShipX[i]--;
      m_iEnemyShipY[i]++;
    } else if (iWay == 7) // 0x40
    {
      if (MapCheckCollision((m_iEnemyShipX[i] / 25) + 1, (m_iEnemyShipY[i] / 25) + 1, false) == 0) {
        continue;
      }
      if (EnemyShipCollosion((m_iEnemyShipX[i] / 25) + 1, (m_iEnemyShipY[i] / 25) + 1) == true) {
        continue;
      }

      m_iEnemyShipGraphicWay[i] = 0x40;

      m_iEnemyShipX[i]++;
      m_iEnemyShipY[i]++;
    } else {
      continue;
    }
    m_iEnemyShipWaveA[i][0] = 25;
  }
}

bool StateGame::EnemyShipCollosion(int iX, int iY) {
  // Enemy ships
  for (int i = 0; i < 10; i++) {
    if ((m_iEnemyShipX[i]) / 25 == iX) {
      if ((m_iEnemyShipY[i]) / 25 == iY) {
        return true;
      }
    }
  }

  // Kaper ships
  if (((m_iShipX) / 25) + m_iMapScreenX == iX) {
    if (((m_iShipY) / 25) + m_iMapScreenY == iY) {
      return true;
    }
  }

  return false;
}

bool StateGame::InterfaceNewTurn() {
  m_iTurn++;

  // Turn registr
  if (m_iTurn == 100) {
    m_iEvent = 21;
    m_bWait = true;
    m_iScoreTurn100 = m_iScore;
  }

  if (m_iTurn == 300) {
    m_iEvent = 22;
    m_bWait = true;
    m_iScoreTurn300 = m_iScore;
  }

  if (m_iTurn >= 500) {
    m_iEvent = 24;
    m_bWait = true;
  }

  //---------------------//
  //----- DEMO CODE -----//
  //---------------------//
  /*
                  if(m_iTurn > 100)
                  {
                          m_iEvent = 22;
                          m_bWait  = true;
                  }
  */
  // Starving
  if (m_iResourceCorn <= 0) {
    m_iResourcePirate /= 1;
    m_iResourcePirate--;
  }

  // GameOver
  if (m_iResourcePirate <= 0) {
    return true;
  }

  // Calculate Food back
  if (m_iResourceCorn > 0) {
    int iTemp = (m_iResourcePirate / 10) + 1;
    m_iResourceCorn -= (std::abs(m_oCanvas->m_oRand->nextInt() % iTemp) + iTemp) % iTemp;
    if (m_iResourceCorn < 0) {
      m_iResourceCorn = 0;
    }
  }

  return false;
}

void StateGame::InterfaceDraw(Graphics* g, bool bTurn) {
  // Draw black background
  g->setColor(0, 0, 0);
  g->fillRect(0, m_oCanvas->m_iHeight - 20, m_oCanvas->m_iWidth, 20);

  if (m_oCanvas->m_bSpeedOptimized == false) {
    // Draw interface-Icon
    g->setClip(0, (m_oCanvas->m_iHeight - 17), 13, 14);
    g->drawImage(
        m_oCanvas->m_oImageArray[1], (0 - 90), (m_oCanvas->m_iHeight - 17 - 49), Graphics::TOP | Graphics::LEFT);
    g->setClip(32, (m_oCanvas->m_iHeight - 17), 14, 14);
    g->drawImage(
        m_oCanvas->m_oImageArray[1], (32 - 119), (m_oCanvas->m_iHeight - 17 - 49), Graphics::TOP | Graphics::LEFT);
    g->setClip(66, (m_oCanvas->m_iHeight - 17), 14, 14);
    g->drawImage(
        m_oCanvas->m_oImageArray[1], (66 - 134), (m_oCanvas->m_iHeight - 17 - 49), Graphics::TOP | Graphics::LEFT);
    g->setClip(100, (m_oCanvas->m_iHeight - 17), 14, 14);
    g->drawImage(
        m_oCanvas->m_oImageArray[1], (100 - 104), (m_oCanvas->m_iHeight - 17 - 49), Graphics::TOP | Graphics::LEFT);
  } else {
    g->setColor(0, 255, 0);
    g->drawString("F", 4, (m_oCanvas->m_iHeight - 15), Graphics::TOP | Graphics::LEFT);
    g->setColor(255, 255, 0);
    g->drawString("G", 36, (m_oCanvas->m_iHeight - 15), Graphics::TOP | Graphics::LEFT);
    g->setColor(0, 0, 255);
    g->drawString("M", 70, (m_oCanvas->m_iHeight - 15), Graphics::TOP | Graphics::LEFT);
    g->setColor(100, 100, 100);
    g->drawString("C", 104, (m_oCanvas->m_iHeight - 15), Graphics::TOP | Graphics::LEFT);
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

  if (bTurn) {
    // Turn
    g->setColor(0, 0, 0);

    m_oCanvas->DrawString("Turn:", m_oCanvas->m_iWidth - 50, 0, false, 1);
    m_oCanvas->DrawString(m_iTurn, m_oCanvas->m_iWidth - 23, 0, false, 1);
    m_oCanvas->DrawString("Score:", 18, 0, false, 1);
    m_oCanvas->DrawString(m_iScore, 51, 0, false, 1);
  }

  // Corn
  g->setColor(255, 255, 255);

  if (game_state == StateGame::state::city) // City State
  {
    if (m_oStateCity.m_cTarget == 1 && ((m_oStateCity.m_iBlink % 2) == 1)) {
      g->setColor(255, 0, 0);
    }
  }
  m_oCanvas->DrawString(m_iResourceCorn, 15, m_oCanvas->m_iHeight - 15, false, 1);
  if ((100 * m_iShipLvl) == (m_iResourceCorn)) {
    m_oCanvas->DrawString(m_iResourceCorn, 15, m_oCanvas->m_iHeight - 15, false, 2);
  }

  // Gold
  g->setColor(255, 255, 255);
  m_oCanvas->DrawString(m_iResourceGold, 48, m_oCanvas->m_iHeight - 15, false, 1);

  // Pirates
  g->setColor(255, 255, 255);

  if (game_state == StateGame::state::city) // City State
  {
    if (m_oStateCity.m_cTarget == 2 && ((m_oStateCity.m_iBlink % 2) == 1)) {
      g->setColor(255, 0, 0);
    }
  }

  if (35 == (m_iResourcePirate) && m_iShipLvl == 1) {
    m_oCanvas->DrawString(m_iResourcePirate, 83, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (50 == (m_iResourcePirate) && m_iShipLvl == 2) {
    m_oCanvas->DrawString(m_iResourcePirate, 83, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (100 == (m_iResourcePirate) && m_iShipLvl == 3) {
    m_oCanvas->DrawString(m_iResourcePirate, 83, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (200 == (m_iResourcePirate) && m_iShipLvl == 4) {
    m_oCanvas->DrawString(m_iResourcePirate, 83, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (350 == (m_iResourcePirate) && m_iShipLvl == 5) {
    m_oCanvas->DrawString(m_iResourcePirate, 83, m_oCanvas->m_iHeight - 15, false, 2);
  } else {
    m_oCanvas->DrawString(m_iResourcePirate, 83, m_oCanvas->m_iHeight - 15, false, 1);
  }

  // Cannons
  g->setColor(255, 255, 255);

  if (game_state == StateGame::state::city) // City State
  {
    if (m_oStateCity.m_cTarget == 3 && ((m_oStateCity.m_iBlink % 2) == 1)) {
      g->setColor(255, 0, 0);
    }
  }
  if (7 == (m_iResourceCannon) && m_iShipLvl == 1) {
    m_oCanvas->DrawString(m_iResourceCannon, 117, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (15 == (m_iResourceCannon) && m_iShipLvl == 2) {
    m_oCanvas->DrawString(m_iResourceCannon, 117, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (30 == (m_iResourceCannon) && m_iShipLvl == 3) {
    m_oCanvas->DrawString(m_iResourceCannon, 117, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (60 == (m_iResourceCannon) && m_iShipLvl == 4) {
    m_oCanvas->DrawString(m_iResourceCannon, 117, m_oCanvas->m_iHeight - 15, false, 2);
  } else if (100 == (m_iResourceCannon) && m_iShipLvl == 5) {
    m_oCanvas->DrawString(m_iResourceCannon, 117, m_oCanvas->m_iHeight - 15, false, 2);
  } else {
    m_oCanvas->DrawString(m_iResourceCannon, 117, m_oCanvas->m_iHeight - 15, false, 1);
  }

  // Captured ship
  if (m_iCapturedShip != 0) {
    if (m_iCapturedShip == 12 || m_iCapturedShip == 13) {
      g->setClip(130, (m_oCanvas->m_iHeight - 20), 19, 19);
      g->drawImage(
          m_oCanvas->m_oImageArray[4], (130 - 140), (m_oCanvas->m_iHeight - 20) - 40, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iCapturedShip == 14 || m_iCapturedShip == 15 || m_iCapturedShip == 18) {
      g->setClip(130, (m_oCanvas->m_iHeight - 20), 19, 19);
      g->drawImage(
          m_oCanvas->m_oImageArray[4], (130 - 140), (m_oCanvas->m_iHeight - 20) - 20, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iCapturedShip == 16 || m_iCapturedShip == 17) {
      g->setClip(130, (m_oCanvas->m_iHeight - 20), 19, 19);
      g->drawImage(
          m_oCanvas->m_oImageArray[4], (130 - 140), (m_oCanvas->m_iHeight - 20) - 60, Graphics::TOP | Graphics::LEFT);
    }

    g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
  }

  // UpdateFromInterface
  if (m_bWait == false) {

    if (m_iShipLvl == 1) {
      if ((100 < m_iResourceCorn)) {
        m_iResourceCorn = 100;
        m_iAddFood = 0;
      }
      if ((35 < m_iResourcePirate)) {
        m_iResourcePirate = 35;
        m_iAddPirates = 0;
      }
      if ((7 < m_iResourceCannon)) {
        m_iResourceCannon = 7;
        m_iAddCannons = 0;
      }
    }
    if (m_iShipLvl == 2) {
      if ((200 < m_iResourceCorn)) {
        m_iResourceCorn = 200;
        m_iAddFood = 0;
      }
      if ((50 < m_iResourcePirate)) {
        m_iResourcePirate = 50;
        m_iAddPirates = 0;
      }
      if ((15 < m_iResourceCannon)) {
        m_iResourceCannon = 15;
        m_iAddCannons = 0;
      }
    }
    if (m_iShipLvl == 3) {
      if ((300 < m_iResourceCorn)) {
        m_iResourceCorn = 300;
        m_iAddFood = 0;
      }
      if ((100 < m_iResourcePirate)) {
        m_iResourcePirate = 100;
        m_iAddPirates = 0;
      }
      if ((30 < m_iResourceCannon)) {
        m_iResourceCannon = 30;
        m_iAddCannons = 0;
      }
    }
    if (m_iShipLvl == 4) {
      if ((400 < m_iResourceCorn)) {
        m_iResourceCorn = 400;
        m_iAddFood = 0;
      }
      if ((200 < m_iResourcePirate)) {
        m_iResourcePirate = 200;
        m_iAddPirates = 0;
      }
      if ((60 < m_iResourceCannon)) {
        m_iResourceCannon = 60;
        m_iAddCannons = 0;
      }
    }
    if (m_iShipLvl == 5) {
      if ((500 < m_iResourceCorn)) {
        m_iResourceCorn = 500;
        m_iAddFood = 0;
      }
      if ((350 < m_iResourcePirate)) {
        m_iResourcePirate = 350;
        m_iAddPirates = 0;
      }
      if ((100 < m_iResourceCannon)) {
        m_iResourceCannon = 100;
        m_iAddCannons = 0;
      }
    }

    if (m_iAddFood != 0) {
      if (m_iAddFood < 0) {
        if (m_iAddFood < -100) {
          m_iResourceCorn -= 5;
          m_iAddFood += 5;
        } else if (m_iAddFood < -50) {
          m_iResourceCorn -= 2;
          m_iAddFood += 2;
        } else {
          m_iResourceCorn--;
          m_iAddFood++;
        }
      }

      if (m_iAddFood > 0) {
        if (m_iAddFood > 100) {
          m_iResourceCorn += 5;
          m_iAddFood -= 5;
        } else if (m_iAddFood > 50) {
          m_iResourceCorn += 2;
          m_iAddFood -= 2;
        } else {
          m_iResourceCorn++;
          m_iAddFood--;
        }
      }
    }

    if (m_iAddGold != 0) {
      if (m_iAddGold < 0) {
        if (m_iAddGold < -60) {
          m_iResourceGold -= 5;
          m_iAddGold += 5;
        } else if (m_iAddGold < -10) {
          m_iResourceGold -= 2;
          m_iAddGold += 2;
        } else {
          m_iResourceGold--;
          m_iAddGold++;
        }
      }

      if (m_iAddGold > 0) {
        if (m_iAddGold > 60) {
          m_iResourceGold += 5;
          m_iAddGold -= 5;
        } else if (m_iAddGold > 10) {
          m_iResourceGold += 2;
          m_iAddGold -= 2;
        } else {
          m_iResourceGold++;
          m_iAddGold--;
        }
      }
    }

    if (m_iAddPirates != 0) {
      if (m_iAddPirates < 0) {
        if (m_iAddPirates < -100) {
          m_iResourcePirate -= 5;
          m_iAddPirates += 5;
        } else if (m_iAddPirates < -50) {
          m_iResourcePirate -= 2;
          m_iAddPirates += 2;
        } else {
          m_iResourcePirate--;
          m_iAddPirates++;
        }
      }

      if (m_iAddPirates > 0) {
        if (m_iAddPirates > 100) {
          m_iResourcePirate += 5;
          m_iAddPirates -= 5;
        } else if (m_iAddPirates > 50) {
          m_iResourcePirate += 2;
          m_iAddPirates -= 2;
        } else {
          m_iResourcePirate++;
          m_iAddPirates--;
        }
      }
    }

    if (m_iAddCannons != 0) {
      if (m_iAddCannons < 0) {
        if (m_iAddCannons < -100) {
          m_iResourceCannon -= 5;
          m_iAddCannons += 5;
        } else if (m_iAddCannons < -50) {
          m_iResourceCannon -= 2;
          m_iAddCannons += 2;
        } else {
          m_iResourceCannon--;
          m_iAddCannons++;
        }
      }

      if (m_iAddCannons > 0) {
        if (m_iAddCannons > 100) {
          m_iResourceCannon += 5;
          m_iAddCannons -= 5;
        } else if (m_iAddCannons > 50) {
          m_iResourceCannon += 2;
          m_iAddCannons -= 2;
        } else {
          m_iResourceCannon++;
          m_iAddCannons--;
        }
      }
    }
    if (m_iResourceCorn < 0) {
      m_iResourceCorn = 0;
    }
    if (m_iResourceCannon < 0) {
      m_iResourceCannon = 0;
    }
    if (m_iResourceGold < 0) {
      m_iResourceGold = 0;
    }
  }
}

void StateGame::Draw(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  if (game_state == StateGame::state::high_score) {
    MapDraw(g);
    DrawBlackBackGround(g);

    g->setColor(255, 255, 255);
    if (m_iWebRankShow == 1) {
      m_oCanvas->DrawString("Highscore   <Month>", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 2) {
      m_oCanvas->DrawString("Highscore   <Today>", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 3) {
      m_oCanvas->DrawString("Highscore   < All >", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 4) {
      m_oCanvas->DrawString("Highscore100<Month>", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 5) {
      m_oCanvas->DrawString("Highscore100< Day >", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 6) {
      m_oCanvas->DrawString("Highscore100< All >", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 7) {
      m_oCanvas->DrawString("Highscore300<Month>", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 8) {
      m_oCanvas->DrawString("Highscore300<Today>", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }
    if (m_iWebRankShow == 9) {
      m_oCanvas->DrawString("Highscore300< All >", (m_oCanvas->m_iWidth / 2) - 60, 60, false, 1);
    }

    if (m_oRegisterHigh->m_cWebDataReady == 2) {
      m_oCanvas->DrawString("Connection failed...", (m_oCanvas->m_iWidth / 2) - 60, 82, false, 1);
      m_oCanvas->DrawString(m_oRegisterHigh->sLink, +4, 62, false, 1);
    }

    else if (m_oRegisterHigh->m_cWebDataReady == 1) {
      for (int i = 0; i < 5; i++) {
        if (i == 2) {
          g->setColor(255, 0, 0);
        }
        if (i == 3) {
          g->setColor(255, 255, 255);
        }

        if (m_iWebRankShow == 1) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHighMonPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHighMonPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighMonPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighMonPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighMonScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
        if (m_iWebRankShow == 2) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHighDayPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHighDayPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighDayPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighDayPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighDayScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
        if (m_iWebRankShow == 3) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHighAllPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHighAllPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighAllPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighAllPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHighAllScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }

        if (m_iWebRankShow == 4 && m_iScoreTurn100 != 0) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHigh100MonPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHigh100MonPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100MonPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100MonPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100MonScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
        if (m_iWebRankShow == 5 && m_iScoreTurn100 != 0) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHigh100DayPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHigh100DayPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100DayPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100DayPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100DayScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
        if (m_iWebRankShow == 6 && m_iScoreTurn100 != 0) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHigh100AllPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHigh100AllPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100AllPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100AllPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh100AllScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }

        if (m_iWebRankShow == 7 && m_iScoreTurn300 != 0) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHigh300MonPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHigh300MonPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300MonPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300MonPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300MonScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
        if (m_iWebRankShow == 8 && m_iScoreTurn300 != 0) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHigh300DayPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHigh300DayPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300DayPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300DayPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300DayScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
        if (m_iWebRankShow == 9 && m_iScoreTurn300 != 0) {
          if ((i == 0) && m_oRegisterHigh->m_sWebHigh300AllPlayers[0].compare("Empty              ") == 0) {
            continue;
          }
          if ((i == 1) && m_oRegisterHigh->m_sWebHigh300AllPlayers[1].compare("Empty              ") == 0) {
            continue;
          }

          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300AllPlace[i],
                                (m_oCanvas->m_iWidth / 2) - 100,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300AllPlayers[i],
                                (m_oCanvas->m_iWidth / 2) - 65,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
          m_oCanvas->DrawString(m_oRegisterHigh->m_sWebHigh300AllScore[i],
                                (m_oCanvas->m_iWidth / 2) + 15,
                                (m_oCanvas->m_iHeight / 2) - 10 + (i * 15),
                                false,
                                1);
        }
      }
    }
  }

  if (game_state == StateGame::state::board) {
    MapDraw(g);

    // Event Draw
    int iMapXOffset = (-m_iMapScreenX * 25) + m_iMapOffScreenX;
    int iMapYOffset = (-m_iMapScreenY * 25) + m_iMapOffScreenY;

    int i;

    for (i = 0; i < 15; i++) {
      if ((m_iEventX[i] + iMapXOffset) < -20 || (m_iEventX[i] + iMapXOffset) > m_oCanvas->m_iWidth) {
        continue;
      }
      if ((m_iEventY[i] + iMapYOffset) < -20 || (m_iEventY[i] + iMapYOffset) > m_oCanvas->m_iHeight - 20) {
        continue;
      }

      g->setClip(m_iEventX[i] + iMapXOffset, m_iEventY[i] + iMapYOffset, 19, 19);

      if (m_iEventAnim < 5) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iEventX[i] + iMapXOffset - 100,
                     m_iEventY[i] + iMapYOffset - 60,
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iEventAnim < 10) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iEventX[i] + iMapXOffset - 80,
                     m_iEventY[i] + iMapYOffset - 60,
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iEventAnim < 15) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iEventX[i] + iMapXOffset - 60,
                     m_iEventY[i] + iMapYOffset - 60,
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iEventAnim < 20) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iEventX[i] + iMapXOffset - 40,
                     m_iEventY[i] + iMapYOffset - 60,
                     Graphics::TOP | Graphics::LEFT);
      }
    }

    g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

    m_iEventAnim++;
    if (m_iEventAnim >= 30) {
      m_iEventAnim = 0;
    }

    // Event Draw-End

    for (i = 0; i < 10; i++) {
      if ((m_iEnemyShipX[i] + iMapXOffset) < -20 || (m_iEnemyShipX[i] + iMapXOffset) > m_oCanvas->m_iWidth) {
        continue;
      }
      if ((m_iEnemyShipY[i] + iMapYOffset) < -20 || (m_iEnemyShipY[i] + iMapYOffset) > m_oCanvas->m_iHeight - 20) {
        continue;
      }

      if (m_oCanvas->m_bSpeedOptimized == false) {
        int iGraphic = 0;

        for (int ii = 0; ii < 3; ii++) {
          if (m_iEnemyShipWaveA[i][0] == 18) {
            m_iEnemyShipWaveA[i][1] = 25;
          }
          if (m_iEnemyShipWaveA[i][0] == 10) {
            m_iEnemyShipWaveA[i][2] = 25;
          }

          if (m_iEnemyShipWaveA[i][ii] < 0) {
            continue;
          }
          if (m_iEnemyShipWaveA[i][ii] == 25) {
            m_iEnemyShipWaveX[i][ii] = m_iEnemyShipX[i];
            m_iEnemyShipWaveY[i][ii] = m_iEnemyShipY[i];

            if (m_iEnemyShipGraphicWay[i] == 1 || m_iEnemyShipGraphicWay[i] == 2) {
              m_iEnemyShipWaveY[i][ii] += 5;
            }
          }

          if (m_iEnemyShipWaveA[i][ii] < 6) {
            iGraphic = 3;
          } else if (m_iEnemyShipWaveA[i][ii] < 12) {
            iGraphic = 2;
          } else if (m_iEnemyShipWaveA[i][ii] < 18) {
            iGraphic = 1;
          } else if (m_iEnemyShipWaveA[i][ii] < 25) {
            iGraphic = 0;
          }

          m_iEnemyShipWaveA[i][ii]--;
          if (m_iEnemyShipWaveA[i][ii] >= 25) {
            continue;
          }

          g->setClip(m_iEnemyShipWaveX[i][ii] + iMapXOffset, m_iEnemyShipWaveY[i][ii] + iMapYOffset, 19, 19);

          if (iGraphic == 0) {
            g->drawImage(m_oCanvas->m_oImageArray[0],
                         m_iEnemyShipWaveX[i][ii] + iMapXOffset - 100,
                         m_iEnemyShipWaveY[i][ii] + iMapYOffset - 60,
                         Graphics::TOP | Graphics::LEFT);
          }
          if (iGraphic == 1) {
            g->drawImage(m_oCanvas->m_oImageArray[0],
                         m_iEnemyShipWaveX[i][ii] + iMapXOffset - 80,
                         m_iEnemyShipWaveY[i][ii] + iMapYOffset - 60,
                         Graphics::TOP | Graphics::LEFT);
          }
          if (iGraphic == 2) {
            g->drawImage(m_oCanvas->m_oImageArray[0],
                         m_iEnemyShipWaveX[i][ii] + iMapXOffset - 60,
                         m_iEnemyShipWaveY[i][ii] + iMapYOffset - 60,
                         Graphics::TOP | Graphics::LEFT);
          }
          if (iGraphic == 3) {
            g->drawImage(m_oCanvas->m_oImageArray[0],
                         m_iEnemyShipWaveX[i][ii] + iMapXOffset - 40,
                         m_iEnemyShipWaveY[i][ii] + iMapYOffset - 60,
                         Graphics::TOP | Graphics::LEFT);
          }
        }
      }
      g->setClip(m_iEnemyShipX[i] + iMapXOffset, m_iEnemyShipY[i] + iMapYOffset, 19, 19);

      if (m_iEnemyShipShipType[i] == 12 || m_iEnemyShipShipType[i] == 13) {
        if (m_iEnemyShipGraphicWay[i] == 1) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 140,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 2) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 8) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 60,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 4) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 120,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x20) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 80,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x80) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 100,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x10) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 20,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x40) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 40,
                       m_iEnemyShipY[i] + iMapYOffset - 40,
                       Graphics::TOP | Graphics::LEFT);
        }
      }

      else if (m_iEnemyShipShipType[i] == 14 || m_iEnemyShipShipType[i] == 15 || m_iEnemyShipShipType[i] == 18) {
        if (m_iEnemyShipGraphicWay[i] == 1) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 140,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 2) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 8) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 60,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 4) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 120,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x20) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 80,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x80) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 100,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x10) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 20,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x40) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 40,
                       m_iEnemyShipY[i] + iMapYOffset - 20,
                       Graphics::TOP | Graphics::LEFT);
        }
      }

      else if (m_iEnemyShipShipType[i] == 16 || m_iEnemyShipShipType[i] == 17) {
        if (m_iEnemyShipGraphicWay[i] == 1) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 140,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 2) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 8) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 60,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 4) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 120,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x20) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 80,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x80) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 100,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x10) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 20,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        } else if (m_iEnemyShipGraphicWay[i] == 0x40) {
          g->drawImage(m_oCanvas->m_oImageArray[4],
                       m_iEnemyShipX[i] + iMapXOffset - 40,
                       m_iEnemyShipY[i] + iMapYOffset - 60,
                       Graphics::TOP | Graphics::LEFT);
        }
      }
    }

    ShipDraw(g);

    if (m_oCanvas->m_oImageArray[5]) {
      for (i = 0; i < 19; i++) {
        if (m_iSkyType[i] == 0) {
          g->setClip(m_iSkyX[i] + iMapXOffset, m_iSkyY[i] + iMapYOffset, 53, 63);
          g->drawImage(m_oCanvas->m_oImageArray[5],
                       m_iSkyX[i] + iMapXOffset,
                       m_iSkyY[i] + iMapYOffset,
                       Graphics::TOP | Graphics::LEFT);
        }
        if (m_iSkyType[i] == 1) {
          g->setClip(m_iSkyX[i] + iMapXOffset, m_iSkyY[i] + iMapYOffset, 51, 63);
          g->drawImage(m_oCanvas->m_oImageArray[5],
                       m_iSkyX[i] + iMapXOffset - 56,
                       m_iSkyY[i] + iMapYOffset,
                       Graphics::TOP | Graphics::LEFT);
        }
        if (m_iSkyType[i] == 2) {
          g->setClip(m_iSkyX[i] + iMapXOffset, m_iSkyY[i] + iMapYOffset, 63, 63);
          g->drawImage(m_oCanvas->m_oImageArray[5],
                       m_iSkyX[i] + iMapXOffset - 108,
                       m_iSkyY[i] + iMapYOffset,
                       Graphics::TOP | Graphics::LEFT);
        }
      }
    }

    g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

    DrawBlackBackGround(g);

    if (m_bWait) {
      DrawFrame(g);
      DrawEvent(g);
    }

    InterfaceDraw(g, true);
  }

  if (game_state == StateGame::state::game_over) {
    MapDraw(g);
    ShipDraw(g);

    DrawFrame(g);

    g->setColor(0, 0, 0);
    m_oCanvas->DrawString("GAME OVER", (m_oCanvas->m_iWidth / 2) - 35, 6, true, 0);

    m_oCanvas->DrawString("Name: ", 8, 40, true, 0);
    m_oCanvas->DrawString(m_sPlayerName, 50, 40, true, 0);

    g->drawLine(50 + (m_iPlayerNamePos * 5), 55, 54 + (m_iPlayerNamePos * 5), 55);

    m_oCanvas->DrawString("[1] Save to local score", 8, 55, false, 0);
    m_oCanvas->DrawString("[3] Save to local/web (Wap)", 8, 65, false, 0);

    m_oCanvas->DrawString("Total", 10, 20, false, 0);
    m_oCanvas->DrawString("Turn100", 50, 20, false, 0);
    m_oCanvas->DrawString("Turn300", 100, 20, false, 0);

    m_oCanvas->DrawString(m_iScore, 15, 30, false, 0);
    m_oCanvas->DrawString(m_iScoreTurn100, 55, 30, false, 0);
    m_oCanvas->DrawString(m_iScoreTurn300, 105, 30, false, 0);
  }

  if (game_state == StateGame::state::sail_to_city) {
    m_oStateSailingToCity.Draw(g);
  }

  if (game_state == StateGame::state::city) {
    m_oStateCity.Draw(g);
    InterfaceDraw(g, false);
  }
  if (game_state == StateGame::state::close_combat) {
    m_oStateBoard.Draw(g);
  }
  if (game_state == StateGame::state::ranged_combat) {
    m_oStateAttack.Draw(g);
  }
}

void StateGame::LoadGame() {
  std::vector<char> recordData;
  int iLength;

  RecordStore* nameRecord = RecordStore::openRecordStore("KaperGame", true);

  if (nameRecord->getNumRecords() < 10) {
    nameRecord->closeRecordStore();
    RecordStore::deleteRecordStore("KaperGame");
    return;
  }

  m_iShipLvl = 5; // Prevent multi tasking, cutting resources...

  recordData = std::vector<char>(nameRecord->getRecordSize(1));
  iLength = nameRecord->getRecord(1, recordData, 0);
  m_iResourceCannon = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(2));
  iLength = nameRecord->getRecord(2, recordData, 0);
  m_iResourceCorn = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(3));
  iLength = nameRecord->getRecord(3, recordData, 0);
  m_iResourceGold = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(4));
  iLength = nameRecord->getRecord(4, recordData, 0);
  m_iResourcePirate = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(5));
  iLength = nameRecord->getRecord(5, recordData, 0);
  m_iScore = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(6));
  iLength = nameRecord->getRecord(6, recordData, 0);
  m_iScoreTurn100 = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(7));
  iLength = nameRecord->getRecord(7, recordData, 0);
  m_iScoreTurn300 = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(8));
  iLength = nameRecord->getRecord(8, recordData, 0);
  m_iTurn = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(9));
  iLength = nameRecord->getRecord(9, recordData, 0);
  m_iCapturedShip = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(10));
  iLength = nameRecord->getRecord(10, recordData, 0);
  m_iMapScreenX = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(11));
  iLength = nameRecord->getRecord(11, recordData, 0);
  m_iMapScreenY = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(12));
  iLength = nameRecord->getRecord(12, recordData, 0);
  m_iShipX = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(13));
  iLength = nameRecord->getRecord(13, recordData, 0);
  m_iShipY = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(14));
  iLength = nameRecord->getRecord(14, recordData, 0);
  m_iPiratesOnCapturedShip = std::stoi(std::string(recordData.data(), 0, iLength));

  recordData = std::vector<char>(nameRecord->getRecordSize(15));
  iLength = nameRecord->getRecord(15, recordData, 0);
  m_iShipLvl = std::stoi(std::string(recordData.data(), 0, iLength));

  for (int i = 0; i < 10; i++) {
    recordData = std::vector<char>(nameRecord->getRecordSize(16 + (i * 5)));
    iLength = nameRecord->getRecord(16 + (i * 5), recordData, 0);
    m_iEnemyShipShipType[i] = std::stoi(std::string(recordData.data(), 0, iLength));

    recordData = std::vector<char>(nameRecord->getRecordSize(17 + (i * 5)));
    iLength = nameRecord->getRecord(17 + (i * 5), recordData, 0);
    m_iEnemyShipX[i] = std::stoi(std::string(recordData.data(), 0, iLength));

    recordData = std::vector<char>(nameRecord->getRecordSize(18 + (i * 5)));
    iLength = nameRecord->getRecord(18 + (i * 5), recordData, 0);
    m_iEnemyShipY[i] = std::stoi(std::string(recordData.data(), 0, iLength));

    recordData = std::vector<char>(nameRecord->getRecordSize(19 + (i * 5)));
    iLength = nameRecord->getRecord(19 + (i * 5), recordData, 0);
    m_iEnemyShipCannons[i] = std::stoi(std::string(recordData.data(), 0, iLength));

    recordData = std::vector<char>(nameRecord->getRecordSize(20 + (i * 5)));
    iLength = nameRecord->getRecord(20 + (i * 5), recordData, 0);
    m_iEnemyShipSoldiers[i] = std::stoi(std::string(recordData.data(), 0, iLength));
  }

  nameRecord->closeRecordStore();

  m_bShotButtonGuide = false;

  /////////////////////////////////////////////////////
  // Check for Ghost and Remove them!  Stupid ghosts...
  /////////////////////////////////////////////////////
  CheckBoatsCollision(true);
}

void StateGame::SaveGame() {
  RecordStore* nameRecord = RecordStore::openRecordStore("KaperGame", true);

  nameRecord->setRecord(1, std::to_string(m_iResourceCannon));
  nameRecord->setRecord(2, std::to_string(m_iResourceCorn));
  nameRecord->setRecord(3, std::to_string(m_iResourceGold));
  nameRecord->setRecord(4, std::to_string(m_iResourcePirate));
  nameRecord->setRecord(5, std::to_string(m_iScore));
  nameRecord->setRecord(6, std::to_string(m_iScoreTurn100));
  nameRecord->setRecord(7, std::to_string(m_iScoreTurn300));
  nameRecord->setRecord(8, std::to_string(m_iTurn));
  nameRecord->setRecord(9, std::to_string(m_iCapturedShip));
  nameRecord->setRecord(10, std::to_string(m_iMapScreenX));
  nameRecord->setRecord(11, std::to_string(m_iMapScreenY));
  nameRecord->setRecord(12, std::to_string(m_iShipX));
  nameRecord->setRecord(13, std::to_string(m_iShipY));
  nameRecord->setRecord(14, std::to_string(m_iPiratesOnCapturedShip));
  nameRecord->setRecord(15, std::to_string(m_iShipLvl));

  for (int i = 0; i < 10; i++) {
    nameRecord->setRecord(16 + (i * 5), std::to_string(m_iEnemyShipShipType[i]));
    nameRecord->setRecord(17 + (i * 5), std::to_string(m_iEnemyShipX[i]));
    nameRecord->setRecord(18 + (i * 5), std::to_string(m_iEnemyShipY[i]));
    nameRecord->setRecord(19 + (i * 5), std::to_string(m_iEnemyShipCannons[i]));
    nameRecord->setRecord(20 + (i * 5), std::to_string(m_iEnemyShipSoldiers[i]));
  }

  nameRecord->closeRecordStore();
}

void StateGame::SaveHighscore(const std::string& sName, int iScore) {
  //------------------------------------------------------
  std::vector<std::string> sNames(5);
  std::vector<int> iScores(5);
  std::vector<char> recordData;
  int iLength;
  bool bNewScore = false;

  RecordStore* nameRecord = RecordStore::openRecordStore("KaperHigh", true);

  //----------------------------
  //---Check if Highscore list is empty
  //----------------------------
  if (nameRecord->getNumRecords() < 3) {
    // Add dummy data
    for (int i = 0; i < 5; i++) {
      if (i == 4) {
        nameRecord->addRecord("Pete");
      }
      if (i == 3) {
        nameRecord->addRecord("Kampfisk");
      }
      if (i == 2) {
        nameRecord->addRecord("Kizza");
      }
      if (i == 1) {
        nameRecord->addRecord("Jonas");
      }
      if (i == 0) {
        nameRecord->addRecord("Tommy");
      }
      nameRecord->addRecord(std::to_string(50 - (i * 10)));
    }
    }

  //----------------------------
  //---Get the list
  //----------------------------
  for (int i = 0; i < 5; i++) {
    recordData = std::vector<char>(nameRecord->getRecordSize((i * 2) + 1));
    iLength = nameRecord->getRecord((i * 2) + 1, recordData, 0);
    sNames[i] = std::string(recordData.data(), 0, iLength);

    recordData = std::vector<char>(nameRecord->getRecordSize((i * 2) + 2));
    iLength = nameRecord->getRecord((i * 2) + 2, recordData, 0);
    iScores[i] = std::stoi(std::string(recordData.data(), 0, iLength));
  }

  //----------------------------
  //--- Check if player score is high enought
  //----------------------------
  for (int i = 0; i < 5; i++) {
    if (iScores[i] < iScore) {
      for (int ii = 4; ii > i; ii--) {
        iScores[ii] = iScores[ii - 1];
        sNames[ii] = sNames[ii - 1];
      }

      iScores[i] = iScore;
      sNames[i] = sName;
      bNewScore = true;

      break;
    }
  }

  if (bNewScore) {
    for (int i = 0; i < 5; i++) {
      nameRecord->setRecord((i * 2) + 1, sNames[i]);
      nameRecord->setRecord((i * 2) + 2, std::to_string(iScores[i]));
    }
  }

  nameRecord->closeRecordStore();
}

void StateGame::CleanGame(bool CreateDummy) {
  std::string sTemp = "0";

  //---Create it, if first time run
  RecordStore* nameRecord = RecordStore::openRecordStore("KaperGame", true);
  nameRecord->closeRecordStore();

  RecordStore::deleteRecordStore("KaperGame");
  nameRecord = RecordStore::openRecordStore("KaperGame", true);

  if (CreateDummy) {
    for (int i = 0; i < 70; i++) {
      nameRecord->addRecord(sTemp);
    }
  }

  nameRecord->closeRecordStore();

  /////////////////////////////////////////////////////
  // Check for Ghost and Remove them!  Stupid ghosts...
  /////////////////////////////////////////////////////
  CheckBoatsCollision(true);
}

bool StateGame::Update() {
  if (m_bClose) {
    return true;
  }
  if (m_bLocked) {
    return false;
  }

  if (m_oCanvas->m_oStateMenu->m_bAnyContinueGame == true && m_iResourcePirate <= 0) {
    CleanGame(false);
    m_oCanvas->m_oStateMenu->m_bAnyContinueGame = false;
  }

  if (game_state == StateGame::state::board) {
    MapUpdate();
    ShipUpdate();

    m_iSkyCounterY--;

    for (int i = 0; i < 19; i++) {
      if (m_oCanvas->m_oImageArray[5]) {
        break;
      }

      if ((m_iSkyCounterY % 2) == 0) {
        m_iSkyX[i]++;
      }

      if (m_iSkyCounterY <= 0) {
        m_iSkyY[i]++;
      }

      // Respawn Skies

      if (m_iSkyX[i] > (25 * 29) + 100) {
        if (std::abs(m_oCanvas->m_oRand->nextInt() % 50) == 0) {
          m_iSkyType[i] = std::abs(m_oCanvas->m_oRand->nextInt() % 3);

          m_iSkyX[i] = -100 - std::abs(m_oCanvas->m_oRand->nextInt() % 600);
          m_iSkyY[i] = std::abs(m_oCanvas->m_oRand->nextInt() % (14 * 25));
        }
      }
    }

    if (m_iSkyCounterY <= 0) {
      m_iSkyCounterY = 16;
    }

    EnemyShipUpdate();

    // Turn500 - GameEnd
    if (m_iTurn >= 501) {
      GameOver();
    }

    // GameOver
    if (m_iResourcePirate <= 0) //----- DEMO CODE -----//
    {
      GameOver();
    }

    if (m_bWaitingOnMove == true && m_bShipMoving == false && m_bMapMoving == false) {
      if (m_bToCity) {
        m_bToCity = false;
        game_state = StateGame::state::sail_to_city;
        m_oStateSailingToCity.Init();

        return false;
      }

      if (true == InterfaceNewTurn()) { // Game Over
        GameOver();
      }
      if (CheckBoatsCollision(false) == false) {
        if (CheckEventCollision() == true) {
          m_iEvent = std::abs(m_oCanvas->m_oRand->nextInt() % (12));
          m_bWait = true;
        }
      }
      m_bWaitingOnMove = false;
    }

    KeyHandling();
  }

  if (game_state == StateGame::state::sail_to_city) {
    m_oStateSailingToCity.Update();
  }

  if (game_state == StateGame::state::city) {
    m_oStateCity.Update();
  }

  if (game_state == StateGame::state::close_combat) {
    m_oStateBoard.Update();
  }
  if (game_state == StateGame::state::ranged_combat) {
    m_oStateAttack.Update();
  }

  return false;
}

void StateGame::CloseCombatWon() {
  m_oCanvas->PlaySound("b.mid", false);

  bool bShip = true;
  ///////////////////////////
  // CLOSE_COMBAT WON
  ///////////////////////////
  if (12 == m_iEventOld) {
    m_iAddGold = 20;
    m_iAddFood = 5;
    m_iAddCannons = 1;
    m_iScore += 10;
  } else if (13 == m_iEventOld) {
    m_iAddGold = 30;
    m_iAddFood = 7;
    m_iAddCannons = 1;
    m_iScore += 20;
  } else if (14 == m_iEventOld) {
    m_iAddGold = 40;
    m_iAddFood = 10;
    m_iAddCannons = 1;
    m_iScore += 30;
  } else if (15 == m_iEventOld) {
    m_iAddGold = 50;
    m_iAddFood = 20;
    m_iAddCannons = 2;
    m_iScore += 40;
  } else if (16 == m_iEventOld) {
    m_iAddGold = 100;
    m_iAddFood = 50;
    m_iAddCannons = 3;
    m_iScore += 50;
  } else if (17 == m_iEventOld) {
    m_iAddGold = 300;
    m_iAddFood = 150;
    m_iAddCannons = 10;
    m_iScore += 60;
  } else if (18 == m_iEventOld) {
    m_iAddGold = 200;
    m_iAddFood = 50;
    m_iAddCannons = 2;
    m_iScore += 20;
  }

  else // Non-ship attack
  {
    m_iAddGold = 150;
    m_iAddFood = 50;
    bShip = false; // false; H�rde ordre fra Jonas

    m_iEventOld = 13;
    m_iEvent = 19;
    m_bWait = true;
    m_iScore += 20;
  }

  if (bShip) {
    CheckBoatsCollision(true);
    m_iEvent = 19;
    m_bWait = true;
  }
}

void StateGame::NormalButton(int iKey) {
  if (m_bLocked) {
    return;
  }

  KeyCheck(iKey); // Game keys

  // Normal Keys

  if (game_state == StateGame::state::high_score) {
    if (iKey == 128) {
      return;
    }

    int keyState = 0;
    keyState = m_oCanvas->getGameAction(iKey);

    if (keyState == Canvas::LEFT) {
      m_iWebRankShow--;
      if (m_iWebRankShow <= 0) {
        m_iWebRankShow = 9;
      }
      return;
    } else if (keyState == Canvas::RIGHT) {
      m_iWebRankShow++;
      if (m_iWebRankShow >= 10) {
        m_iWebRankShow = 1;
      }
      return;
    } else {
      m_oRegisterHigh->Deinit();
      m_bClose = true;
    }
  }

  if (game_state == StateGame::state::game_over) {
    if (Canvas::KEY_NUM1 == iKey) {
      m_oCanvas->StopSound();

      // Register Score
      SaveHighscore(m_sPlayerName, m_iScore);

      m_bClose = true;
      return;
    }

    if (Canvas::KEY_NUM3 == iKey) {
      m_oCanvas->StopSound();

      // Register Score
      SaveHighscore(m_sPlayerName, m_iScore);

      // Register Web Score
      Thread* commThread;

      int iKeyGen = m_iScore + m_iScoreTurn100 + m_iScoreTurn300;
      iKeyGen += m_sPlayerName[0];
      // iKey ^= 0xFF;

      std::string sLink = "http://www.mobilkaper.dk/AdLucem.php?Name=" + m_sPlayerName +
                          "&Scorea=" + std::to_string(m_iScore) + "&Scoreb=" + std::to_string(m_iScoreTurn100) +
                          "&Scorec=" + std::to_string(m_iScoreTurn300) + "&Key=" + std::to_string(iKeyGen);

      m_oRegisterHigh = new RegisterHighscore(sLink);
      commThread = new Thread(m_oRegisterHigh);
      commThread->start();

      game_state = StateGame::state::high_score;

      delete commThread;
      return;
    }

    if (iKey == 128) {
      return;
    }
    int keyState = m_oCanvas->getGameAction(iKey);

    //	m_sPlayerName

    if (keyState == Canvas::UP) {
      m_sPlayerName = m_sPlayerName.substr(0, m_iPlayerNamePos) +
        SwitchChar(m_sPlayerName[m_iPlayerNamePos], true) +
        m_sPlayerName.substr(m_iPlayerNamePos + 1);
    }

    if (keyState == Canvas::DOWN) {
      m_sPlayerName = m_sPlayerName.substr(0, m_iPlayerNamePos) +
        SwitchChar(m_sPlayerName[m_iPlayerNamePos], false) +
        m_sPlayerName.substr(m_iPlayerNamePos + 1);
    }

    if (keyState == Canvas::LEFT) {
      if (m_iPlayerNamePos > 0) {
        m_iPlayerNamePos--;
      }
    }

    if (keyState == Canvas::RIGHT) {
      if (m_iPlayerNamePos > 10) {
        return;
      }

      if (static_cast<int>(m_sPlayerName.length()) <= (m_iPlayerNamePos + 1)) {
        m_sPlayerName += "a";
      }
      m_iPlayerNamePos++;
    }

    return;
  }

  if (game_state == StateGame::state::board && m_bWait == false) {
    if (Canvas::KEY_NUM5 == iKey) {
      // Instant gold
      m_iResourceGold += m_iAddGold;
      m_iAddGold = 0;
      m_iResourceCorn += m_iAddFood;
      m_iAddFood = 0;
      m_iResourceCannon += m_iAddCannons;
      m_iAddCannons = 0;
      m_iResourcePirate += m_iAddPirates;
      m_iAddPirates = 0;
    }
  }

  if (game_state == StateGame::state::board && m_bWait == true) {
    if (Canvas::KEY_NUM7 == iKey) {
      KeyPressedEvent(7);
    }
    if (Canvas::KEY_NUM9 == iKey) {
      KeyPressedEvent(9);
    }
    if (Canvas::KEY_NUM5 == iKey) {
      KeyPressedEvent(5);
    }
    if (Canvas::KEY_NUM1 == iKey) {
      KeyPressedEvent(1);
    }
    if (Canvas::KEY_NUM3 == iKey) {
      KeyPressedEvent(3);
    }
  }

  else if (game_state == StateGame::state::sail_to_city) {
    m_oStateSailingToCity.SoftKey(iKey);

    if (Canvas::KEY_NUM5 != iKey) {
      return;
    }

    if (m_oStateSailingToCity.m_iStatus == 5) {
      m_oStateSailingToCity.m_bPaused = false;
      m_oStateSailingToCity.m_iStatus = 6;
    }

    if (m_oStateSailingToCity.m_iStatus == 1) // Docked
    {
      m_oStateCity.Init();
      game_state = StateGame::state::city;
    }

    else if (m_oStateSailingToCity.m_iStatus == 2) // Crashed
    {
      m_oStateCity.Init();
      game_state = StateGame::state::city;

      // Punish
      m_iAddCannons = -1;
      m_iAddPirates -= (m_iResourcePirate / 2);
    }
  }

  else if (game_state == StateGame::state::city) {
    if (-1 == m_oStateCity.SoftKey(iKey)) {
      m_oStateCity.DeInit();
      game_state = StateGame::state::board;
    }
  }

  else if (game_state == StateGame::state::close_combat) {
    int iStatus = 0;
    iStatus = m_oStateBoard.SoftKey(iKey);

    if (iStatus > 0) // Retreat
    {
      m_iResourcePirate = iStatus;
      m_oStateBoard.DeInit();
      game_state = StateGame::state::board;
      //				m_sEventMsg1 = "You escaped combat";
      CalculateFleePunish();
      m_iEvent = 20;
      m_iEventOld = -1;
    }

    if (iStatus == -1) // Game Over
    {
      m_oStateBoard.DeInit();
      game_state = StateGame::state::board;

      if (m_iResourcePirate <= 0) // GameOver
      {
        GameOver();
      } else // Won the battle
      {
        CloseCombatWon();
      }
    }
  } else if (game_state == StateGame::state::ranged_combat) {
    int iStatus = 0;
    iStatus = m_oStateAttack.SoftKey(iKey);

    if (iStatus > 0) // Board
    {
      if (m_iResourcePirate <= 0) // GameOver
      {
        GameOver();
      }

      m_iEnemySoldiers = iStatus;
      m_oStateBoard.Init(iStatus);
      game_state = StateGame::state::close_combat;
    }

    if (iStatus == -2) // Flee
    {
      if (m_iResourcePirate <= 0) // GameOver
      {
        GameOver();
      }

      //				m_oStateAttack.DeInit();

      game_state = StateGame::state::board;

      // m_sEventMsg1 = "You got away";
      CalculateFleePunish();
      m_iEvent = 20;
    }

    if (iStatus == -1) {
      //				m_oStateAttack.DeInit();

      game_state = StateGame::state::board;

      if (m_iResourcePirate <= 0) // GameOver
      {
        GameOver();
      } else // Won the battle
      {
        m_oCanvas->PlaySound("b.mid", false);

        ///////////////////////////
        // RANGE_COMBAT WON
        ///////////////////////////
        if (12 == m_iEventOld) {
          m_iAddFood = 5;
          m_iAddGold = 10;
          m_iAddPirates = 1;
          m_iScore += 10;
        }
        if (13 == m_iEventOld) {
          m_iAddFood = 7;
          m_iAddGold = 15;
          m_iAddPirates = 2;
          m_iScore += 20;
        }
        if (14 == m_iEventOld) {
          m_iAddFood = 10;
          m_iAddGold = 20;
          m_iAddPirates = 3;
          m_iScore += 30;
        }
        if (15 == m_iEventOld) {
          m_iAddFood = 15;
          m_iAddGold = 25;
          m_iAddPirates = 5;
          m_iScore += 40;
        }
        if (16 == m_iEventOld) {
          m_iAddFood = 20;
          m_iAddGold = 50;
          m_iAddPirates = 7;
          m_iScore += 50;
        }
        if (17 == m_iEventOld) {
          m_iAddFood = 30;
          m_iAddGold = 150;
          m_iAddPirates = 20;
          m_iScore += 100;
        }
        if (18 == m_iEventOld) {
          m_iAddFood = 20;
          m_iAddGold = 100;
          m_iAddPirates = 7;
          m_iScore += 35;
        }

        m_sEventMsg1 = "You sank the enemy.";
        m_sEventMsg2 = "And salvage resources";

        CheckBoatsCollision(true);
      }
    }
  }
}

bool StateGame::CheckBoatsCollision(bool bRemoveThem) {
  if (m_bLocked) {
    return false;
  }

  if (bRemoveThem) {
    for (int i = 0; i < 10; i++) {
      if (m_iEnemyShipSoldiers[i] <= 0 || ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX) &&
                                           (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY))) {
        int iRandom;

        // Placing random place
        do {
          m_iEnemyShipX[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (29));
          m_iEnemyShipY[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (14));
        } while (m_oCanvas->m_oStateGame->MapCheckCollision(
                     (m_iEnemyShipX[i] / 25) + 0, (m_iEnemyShipY[i] / 25) - 0, false) == 0 ||
                 ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX) &&
                  (m_iEnemyShipY[i] / 25) == (m_iShipY / 25) + m_iMapScreenY));

        m_iEnemyShipGraphicWay[i] = 1;

        iRandom = std::abs(m_oCanvas->m_oRand->nextInt() % (m_iShipLvl + 1));

        m_iEnemyShipShipType[i] = 12 + iRandom;

        if (iRandom == 0) {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 3) + 1 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 10) + 5 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        }

        else if (iRandom == 1) {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 5) + 2 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 21) + 15 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        } else if (iRandom == 2) {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 14) + 13 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 26) + 35 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        } else if (iRandom == 3) {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 26) + 25 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 71) + 40 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        } else if (iRandom == 4) {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 51) + 30 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 351) + 100 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        } else if (iRandom == 5) {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 101) + 100 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 501) + 500 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        } else //(iRandom == 6)
        {
          m_iEnemyShipCannons[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 101) + 50 +
                                    ((m_oCanvas->m_oStateGame->m_iResourceCannon * 30) / 100));
          m_iEnemyShipSoldiers[i] = (std::abs(m_oCanvas->m_oRand->nextInt() % 501) + 50 +
                                     ((m_oCanvas->m_oStateGame->m_iResourcePirate * 30) / 100));
        }
      }
    }

    return true;
  }

  for (int i = 0; i < 10; i++) {
    if ((m_iEnemyShipX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX) &&
        (m_iEnemyShipY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY)) {
      m_iEvent = m_iEnemyShipShipType[i];
      m_iShipNr = i;
      m_bWait = true;

      return true;
    }
  }

  return false; // Roll normal events
}

void StateGame::GameOver() {
  if (m_bLocked) {
    return;
  }

  CleanGame(false);

  game_state = StateGame::state::game_over;
  m_oCanvas->PlaySound("c.mid", false);
}

int StateGame::KeyCheck(int iKey) {
  if (m_bLocked) {
    return -1;
  }

  if (iKey != 128) {
    // Move ship phase
    if (m_bShipMoving == false && m_bMapMoving == false && m_bWait == false && m_iAddFood == 0 && m_iAddGold == 0 &&
        m_iAddPirates == 0 && m_iAddCannons == 0) {
      if (m_iKeyCountDown <= 0) {
        m_iKeyCountDown = 5;
      }

      int keyState = m_oCanvas->getGameAction(iKey);

      // Op
      if (keyState == Canvas::UP) {
        m_bKeyUp = true;
      }

      // Ned
      else if (keyState == Canvas::DOWN) {
        m_bKeyDown = true;
      }

      // H�jre
      else if (keyState == Canvas::RIGHT) {
        m_bKeyRight = true;
      }

      // Venstre
      else if (keyState == Canvas::LEFT) {
        m_bKeyLeft = true;
      }
    }
  }

  return -1;
}

char StateGame::SwitchChar(char cChar, bool bUp) {
  if (bUp) {
    if ('a' == cChar) {
      cChar = 'b';
    } else if ('b' == cChar) {
      cChar = 'c';
    } else if ('c' == cChar) {
      cChar = 'd';
    } else if ('d' == cChar) {
      cChar = 'e';
    } else if ('e' == cChar) {
      cChar = 'f';
    } else if ('f' == cChar) {
      cChar = 'g';
    } else if ('g' == cChar) {
      cChar = 'h';
    } else if ('h' == cChar) {
      cChar = 'i';
    } else if ('i' == cChar) {
      cChar = 'j';
    } else if ('j' == cChar) {
      cChar = 'k';
    } else if ('k' == cChar) {
      cChar = 'l';
    } else if ('l' == cChar) {
      cChar = 'm';
    } else if ('m' == cChar) {
      cChar = 'n';
    } else if ('n' == cChar) {
      cChar = 'o';
    } else if ('o' == cChar) {
      cChar = 'p';
    } else if ('p' == cChar) {
      cChar = 'q';
    } else if ('q' == cChar) {
      cChar = 'r';
    } else if ('r' == cChar) {
      cChar = 's';
    } else if ('s' == cChar) {
      cChar = 't';
    } else if ('t' == cChar) {
      cChar = 'v';
    } else if ('v' == cChar) {
      cChar = 'u';
    } else if ('u' == cChar) {
      cChar = 'w';
    } else if ('w' == cChar) {
      cChar = 'x';
    } else if ('x' == cChar) {
      cChar = 'y';
    } else if ('y' == cChar) {
      cChar = 'z';
    } else if ('z' == cChar) {
      cChar = ' ';
    } else if (' ' == cChar) {
      cChar = 'a';
    }
  } else {
    if ('a' == cChar) {
      cChar = ' ';
    } else if ('b' == cChar) {
      cChar = 'a';
    } else if ('c' == cChar) {
      cChar = 'b';
    } else if ('d' == cChar) {
      cChar = 'c';
    } else if ('e' == cChar) {
      cChar = 'd';
    } else if ('f' == cChar) {
      cChar = 'e';
    } else if ('g' == cChar) {
      cChar = 'f';
    } else if ('h' == cChar) {
      cChar = 'g';
    } else if ('i' == cChar) {
      cChar = 'h';
    } else if ('j' == cChar) {
      cChar = 'i';
    } else if ('k' == cChar) {
      cChar = 'j';
    } else if ('l' == cChar) {
      cChar = 'k';
    } else if ('m' == cChar) {
      cChar = 'l';
    } else if ('n' == cChar) {
      cChar = 'm';
    } else if ('o' == cChar) {
      cChar = 'n';
    } else if ('p' == cChar) {
      cChar = 'o';
    } else if ('q' == cChar) {
      cChar = 'p';
    } else if ('r' == cChar) {
      cChar = 'q';
    } else if ('s' == cChar) {
      cChar = 'r';
    } else if ('t' == cChar) {
      cChar = 's';
    } else if ('v' == cChar) {
      cChar = 't';
    } else if ('u' == cChar) {
      cChar = 'v';
    } else if ('w' == cChar) {
      cChar = 'u';
    } else if ('x' == cChar) {
      cChar = 'w';
    } else if ('y' == cChar) {
      cChar = 'x';
    } else if ('z' == cChar) {
      cChar = 'y';
    } else if (' ' == cChar) {
      cChar = 'z';
    }
  }

  if (m_iPlayerNamePos == 0 && cChar == ' ') {
    cChar = 'a';
  }

  return cChar;
}

void StateGame::KeyPressedEvent(int iKey) {
  m_oCanvas->StopSound();

  if (iKey == 5 && m_iEvent == 20) {
    m_bWait = false;

    m_iAddPirates -= m_iFleePunishKapers;
    m_iAddCannons -= m_iFleePunishCannons;

    return;
  }

  if (iKey == 5 && m_iEvent == 24) {
    m_bWait = false;
    return;
  }

  if (iKey == 5 && m_iEvent == 21) {
    m_bWait = false;
    return;
  }

  if (iKey == 5 && m_iEvent == 22) {
    m_bWait = false;
    //---------------------//
    //----- DEMO CODE -----//
    //---------------------//
    // GameOver();
    return;
  }

  if (iKey == 5 && m_iEvent == -1) // Continue pressed
  {
    m_sEventMsg2 = "";
    m_bWait = false;

    if (m_iEventOld == 8) {
      m_iEnemySoldiers = std::abs(m_oCanvas->m_oRand->nextInt() % (m_iResourcePirate + 1)) + 1;
      m_oStateBoard.Init(m_iEnemySoldiers);
      game_state = StateGame::state::close_combat; // GAME_CLOSECOMBAT;
    }
    if (m_iEventOld == 4) {
      m_iEnemySoldiers = std::abs(m_oCanvas->m_oRand->nextInt() % (m_iResourcePirate + 1)) + 1;
      m_oStateBoard.Init(m_iEnemySoldiers);
      game_state = StateGame::state::close_combat; // GAME_CLOSECOMBAT;
    }
  }

  if (iKey == 9 && m_iEvent != -1) {
    if (m_iEvent == 23) {
      m_iEvent = 20;
      CalculateFleePunish();
      return;
    }
    if (m_iEvent >= 12 && m_iEvent <= 18) {
      m_iEvent = 20;
      CalculateFleePunish();
      return;
    } else if (m_iEvent == 20) {
      return;
    }

    m_bWait = false;
  }

  if (iKey == 1) // Attack
  {
    if (m_iEvent >= 12 && m_iEvent <= 18) {
    } else {
      return;
    }

    m_oStateAttack.Init(m_iEnemyShipCannons[m_iShipNr], m_iEnemyShipSoldiers[m_iShipNr]);

    game_state = StateGame::state::ranged_combat; // GAME_RANGECOMBAT;
    m_iEventOld = m_iEvent;
    m_iEvent = -1;
  }

  if (iKey == 7) {
    if (m_iEvent == 12 || m_iEvent == 13 || m_iEvent == 14 || m_iEvent == 15 || m_iEvent == 16 || m_iEvent == 17 ||
        m_iEvent == 20 || m_iEvent == 18) {
      return;
    }

    if (m_iEvent == 23) {

      // Calculate Loss men
      // !!!!!
      while ((m_iResourcePirate + m_iAddPirates) > 0 && m_iEnemySoldiers > 0) {
        int iRandom = std::abs(m_oCanvas->m_oRand->nextInt() % 3);
        if (iRandom == 1) {
          m_iAddPirates--;
        }
        if (iRandom == 2) {
          m_iEnemySoldiers--;
        }
      }

      if ((m_iResourcePirate + m_iAddPirates) <= 0) {
        GameOver();
      } else {
        CloseCombatWon();
      }

      return;
    }

    if (m_iEvent == 19) {
      if (m_iCapturedShip != 0) {
        m_sEventMsg1 = "You already got a";
        m_sEventMsg2 = "prize ship";
      } else {
        m_sEventMsg1 = "Ship taken as prize";
        int iTemp = m_iAddPirates;
        m_iAddPirates -= ((m_iResourcePirate + iTemp) / 2);
        m_iPiratesOnCapturedShip = ((m_iResourcePirate + iTemp) / 2);
        m_iCapturedShip = m_iEventOld;
      }
    }

    if (m_iEvent == 0) {
      m_iAddFood = std::abs(m_oCanvas->m_oRand->nextInt() % 75) + 1 + ((m_iResourceCorn * 50) / 100);
      m_sEventMsg1 = "Grain found: " + std::to_string(m_iAddFood);
    }
    if (m_iEvent == 1) {
      m_iAddPirates = std::abs(m_oCanvas->m_oRand->nextInt() % 20) + 1 + ((m_iResourcePirate * 50) / 100);
      m_sEventMsg1 = "Survived sailors";
      m_sEventMsg2 = "found: " + std::to_string(m_iAddPirates);
    }
    if (m_iEvent == 2) {
      m_sEventMsg1 = "Nothing found...";
    }
    if (m_iEvent == 3) {
      m_iAddPirates = std::abs(m_oCanvas->m_oRand->nextInt() % 20) + 1 + ((m_iResourcePirate * 30) / 100); // %30
      if (m_iAddPirates > m_iResourcePirate) {
        m_iAddPirates = m_iResourcePirate;
      }
      m_sEventMsg1 = std::to_string(m_iAddPirates) + " Crewmen dies from";
      m_sEventMsg2 = "the plague.";
      m_iAddPirates = -m_iAddPirates;
    }
    if (m_iEvent == 4) {
      m_sEventMsg1 = "Pirates!,";
      m_sEventMsg2 = "prepare for battle";
    }
    if (m_iEvent == 5) {
      m_iAddCannons = std::abs(m_oCanvas->m_oRand->nextInt() % 9) + 1;
      m_sEventMsg1 = "Cannons found: " + std::to_string(m_iAddCannons);
    }
    if (m_iEvent == 6) {
      m_sEventMsg1 = "Nothing found...";
    }
    if (m_iEvent == 7) {
      m_iAddGold = std::abs(m_oCanvas->m_oRand->nextInt() % 250) + 50;
      m_sEventMsg1 = "Gold found: " + std::to_string(m_iAddGold);
    }
    if (m_iEvent == 8) {
      m_sEventMsg1 = "Pirates!,";
      m_sEventMsg2 = "prepare for battle";
    }
    if (m_iEvent == 9) {
      m_sEventMsg1 = "Nothing found...";
    }
    if (m_iEvent == 10) {
      m_sEventMsg1 = "Crashed into rock.";
      m_iAddFood = -1 - ((m_iResourceCorn * 60) / 100);      // 60%
      m_iAddGold = -1 - ((m_iResourceGold * 5) / 100);       //  5%
      m_iAddPirates = -1 - ((m_iResourcePirate * 20) / 100); // 20%
      m_iAddCannons = -1 - ((m_iResourceCannon * 10) / 100); // 10%
    }
    if (m_iEvent == 11) {
      m_iAddGold = std::abs(m_oCanvas->m_oRand->nextInt() % 250) + 50;
      m_sEventMsg1 = "Gold found: " + std::to_string(m_iAddGold);
    }

    m_iEventOld = m_iEvent;
    m_iEvent = -1;
  }
}

void StateGame::KeyHandling() {
  if (m_bLocked) {
    return;
  }

  if (!m_bKeyLeft && !m_bKeyRight && !m_bKeyUp && !m_bKeyDown) {
    return;
  }

  if (!(m_bWait == false && m_iAddFood == 0 && m_iAddGold == 0 && m_iAddPirates == 0 && m_iAddCannons == 0)) {
    m_bKeyLeft = false;
    m_bKeyRight = false;
    m_bKeyUp = false;
    m_bKeyDown = false;

    m_iKeyCountDown = 0;
  }

  int iMsg;

  if (m_iKeyCountDown <= 0) {
    if (m_bKeyUp && m_bKeyDown) {
      m_bKeyLeft = false;
      m_bKeyRight = false;
      m_bKeyUp = false;
      m_bKeyDown = false;

      m_iKeyCountDown = 0;
    }
    if (m_bKeyRight && m_bKeyLeft) {
      m_bKeyLeft = false;
      m_bKeyRight = false;
      m_bKeyUp = false;
      m_bKeyDown = false;

      m_iKeyCountDown = 0;
    }

    // 41
    if (m_bKeyUp && m_bKeyRight) {
      iMsg = ShipMove(0x80, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        MapIncSetXY(0x80);
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // 42
    else if (m_bKeyUp && m_bKeyLeft) {
      iMsg = ShipMove(0x20, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        MapIncSetXY(0x20);
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // 82
    else if (m_bKeyDown && m_bKeyLeft) {
      iMsg = ShipMove(0x10, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        MapIncSetXY(0x10);
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // 81
    else if (m_bKeyDown && m_bKeyRight) {
      iMsg = ShipMove(0x40, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        MapIncSetXY(0x40);
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // Op
    else if (m_bKeyUp) {
      iMsg = ShipMove(4, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        if (MapIncSetXY(4) == true) {
          ShipMove(4, true);
        }
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // Ned
    else if (m_bKeyDown) {
      iMsg = ShipMove(8, false);
      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        if (MapIncSetXY(8) == true) {
          ShipMove(8, true);
        }
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // H�jre
    else if (m_bKeyRight) {
      iMsg = ShipMove(1, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        if (MapIncSetXY(1) == true) {
          ShipMove(1, true);
        }
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    // Venstre
    else if (m_bKeyLeft) {
      iMsg = ShipMove(2, false);

      if ((iMsg & 0x04) == 0x04) // City collision
      {
        m_bToCity = true;
        iMsg -= 0x04;
      }

      if (0x02 == iMsg) {
        m_bKeyLeft = false;
        m_bKeyRight = false;
        m_bKeyUp = false;
        m_bKeyDown = false;

        m_iKeyCountDown = 0;
        return;
      }
      if (0x01 == iMsg) {
        if (MapIncSetXY(2) == true) {
          ShipMove(2, true);
        }
      }

      EnemyShipMove();

      m_bWaitingOnMove = true;
    }

    m_bKeyLeft = false;
    m_bKeyRight = false;
    m_bKeyUp = false;
    m_bKeyDown = false;
  }

  m_iKeyCountDown--;
}

void StateGame::CalculateFleePunish() {
  if (std::abs(m_oCanvas->m_oRand->nextInt() % 100) < 30) {
    m_iFleePunishKapers = std::abs(m_oCanvas->m_oRand->nextInt() % (1 + (((m_iResourcePirate + 1) * 30) / 100)));
    m_iFleePunishCannons = std::abs(m_oCanvas->m_oRand->nextInt() % (1 + (((m_iResourceCannon + 1) * 50) / 100)));
  } else {
    m_iFleePunishKapers = 0;
    m_iFleePunishCannons = 0;
  }
}

void StateGame::DrawEvent(Graphics* g) {
  int iX = (m_oCanvas->m_iWidth - 14) / 9;
  int iY = (m_oCanvas->m_iHeight - 34) / 9;
  int iXStart = 0;
  int iYStart = 0;

  if (iX < 15) {
    iXStart = -9;
  }
  if (iY < 9) {
    iYStart = -9;
  }

  g->setColor(0, 0, 0);

  if (m_iEvent == -1) // Press ok to continue
  {
    m_oCanvas->DrawString(m_sEventMsg1, iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString(m_sEventMsg2, iXStart + 21, iYStart + 21 + 10, false, 0);

    m_oCanvas->DrawString("Press [5] to continue", iXStart + 21, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 0 || m_iEvent == 1 || m_iEvent == 2 || m_iEvent == 3 || m_iEvent == 4 || m_iEvent == 5) {
    m_oCanvas->DrawString("An abandoned ship", iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString("appears.", iXStart + 21, iYStart + 21 + 10, false, 0);

    m_oCanvas->DrawString("Investigate ship?", iXStart + 21, iYStart + 21 + 30, false, 0);
    m_oCanvas->DrawString("Yes[7] No[9]", iXStart + 21 + 5, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 6 || m_iEvent == 7 || m_iEvent == 8) {
    m_oCanvas->DrawString("Strange object in", iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString("the mist.", iXStart + 21, iYStart + 21 + 10, false, 0);

    m_oCanvas->DrawString("Investigate mist?", iXStart + 21, iYStart + 21 + 30, false, 0);
    m_oCanvas->DrawString("Yes[7] No[9]", iXStart + 21 + 5, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 9 || m_iEvent == 10 || m_iEvent == 11) {
    m_oCanvas->DrawString("Floating debris.", iXStart + 21, iYStart + 21, false, 0);

    m_oCanvas->DrawString("Investigate?", iXStart + 21, iYStart + 21 + 30, false, 0);
    m_oCanvas->DrawString("Yes[7] No[9]", iXStart + 21 + 5, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 19) {
    m_oCanvas->DrawString("You won the battle!", iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString("[7]Take as prize ship", iXStart + 21, iYStart + 21 + 10, false, 0);
    m_oCanvas->DrawString("[9]Sink ship", iXStart + 21, iYStart + 21 + 20, false, 0);
  }

  if (m_iEvent == 21) {
    m_oCanvas->DrawString("Turn100 passed.", iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString("Score saved...", iXStart + 21, iYStart + 21 + 15, false, 0);
    m_oCanvas->DrawString("[5] Continue", iXStart + 21, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 22) {
    m_oCanvas->DrawString("Turn300 passed.", iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString("Score saved...", iXStart + 21, iYStart + 21 + 15, false, 0);
    m_oCanvas->DrawString("[5] Continue", iXStart + 21, iYStart + 21 + 40, false, 0);

  }

  if (m_iEvent == 24) {
    m_oCanvas->DrawString("Congratulations!", iXStart + 21, iYStart + 21, false, 0);

    if (1 == m_iShipLvl) {
      m_oCanvas->DrawString("Coward caper", iXStart + 21, iYStart + 21 + 15, false, 0);
    }
    if (2 == m_iShipLvl) {
      m_oCanvas->DrawString("Wussy caper", iXStart + 21, iYStart + 21 + 15, false, 0);
    }
    if (3 == m_iShipLvl) {
      m_oCanvas->DrawString("Average caper", iXStart + 21, iYStart + 21 + 15, false, 0);
    }
    if (4 == m_iShipLvl) {
      m_oCanvas->DrawString("Competent caper", iXStart + 21, iYStart + 21 + 15, false, 0);
    }
    if (5 == m_iShipLvl) {
      m_oCanvas->DrawString("Competent caper", iXStart + 21, iYStart + 21 + 15, false, 0);
    }

    m_oCanvas->DrawString("[5] Continue", iXStart + 21, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 23) {
    m_oCanvas->DrawString("Close combat. Enemy soldiers", iXStart + 21, iYStart + 21, false, 0);
    m_oCanvas->DrawString(
        "(" + std::to_string(m_iEnemySoldiers) + ") Board ship?", iXStart + 21, iYStart + 21 + 15, false, 0);
    m_oCanvas->DrawString("Yes[7] No[9]", iXStart + 21, iYStart + 21 + 40, false, 0);
  }

  if (m_iEvent == 20) {
    m_oCanvas->DrawString("You escaped combat", iXStart + 21, iYStart + 21, false, 0);

    if (m_iFleePunishKapers == 0 && m_iFleePunishCannons == 0) {
      m_oCanvas->DrawString("without damage [5]", iXStart + 21, iYStart + 21 + 10, false, 0);
    } else {
      m_oCanvas->DrawString("with some damage [5]", iXStart + 21, iYStart + 21 + 10, false, 0);
    }
  }

  if (m_iEvent == 12 || m_iEvent == 13 || m_iEvent == 14 || m_iEvent == 15 || m_iEvent == 16 || m_iEvent == 17 ||
      m_iEvent == 18) {
    if (m_iEvent == 12) {
      m_oCanvas->DrawString("English schooner", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[1-2] S[5-10]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    if (m_iEvent == 13) {
      m_oCanvas->DrawString("English merchant ship", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[1-3] S[5-15]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    if (m_iEvent == 14) {
      m_oCanvas->DrawString("English brig", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[5-8] S[25-40]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    if (m_iEvent == 15) {
      m_oCanvas->DrawString("English gunboat", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[7-10] S[35-50]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    if (m_iEvent == 16) {
      m_oCanvas->DrawString("English frigate", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[10-20] S[50-100]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    if (m_iEvent == 17) {
      m_oCanvas->DrawString("English Man-O-War", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[100-250] S[500-1250]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    if (m_iEvent == 18) {
      m_oCanvas->DrawString("Pirate ship!", iXStart + 21, iYStart + 21, false, 0);
      // g.drawString("C[x-xx] S[x-xxx]",21,21+10,Graphics.TOP | Graphics.LEFT);
    }

    m_oCanvas->DrawString("Choose order.", iXStart + 21, iYStart + 21 + 30, false, 0);
    m_oCanvas->DrawString("Attack[1] Flee[9]", iXStart + 21 + 5, 21 + 40, false, 0);
  }
}

void StateGame::DrawBlackBackGround(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  //------------------------------------------
  // X coordinate
  //------------------------------------------

  // 29 tiles height (29*25)
  // m_iMapScreenX

  int iTempX = m_iMapOffScreenX;
  if (iTempX < 0) {
    iTempX = -25 + iTempX;
  }
  if (iTempX > 0) {
    iTempX = -25 + iTempX;
  }
  if (iTempX == 0) {
    iTempX = -25;
  }

  if ((29 * 25) + 20 < -iTempX + (m_iMapScreenX * 25) + (m_oCanvas->m_iWidth)) {
    g->setColor(0, 0, 0);

    if (m_iMapOffScreenX < 0) {
      g->fillRect(
          (((m_oCanvas->m_iWidth + 24) / 25) * 25) + m_iMapOffScreenX, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
    } else if (m_iMapOffScreenX > 0) {
      g->fillRect((((m_oCanvas->m_iWidth + 24) / 25) * 25) - 25 + m_iMapOffScreenX,
                  0,
                  m_oCanvas->m_iWidth,
                  m_oCanvas->m_iHeight);
    } else {
      g->fillRect((((m_oCanvas->m_iWidth + 24) / 25) * 25) - 25, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
    }
  }

  //------------------------------------------
  // Y coordinate
  //------------------------------------------

  // 14 tiles height (14*25)
  // m_iMapScreenY

  int iTempY = m_iMapOffScreenY;
  if (iTempY < 0) {
    iTempY = -25 + iTempY;
  }
  if (iTempY > 0) {
    iTempY = -25 + iTempY;
  }
  if (iTempY == 0) {
    iTempY = -25;
  }

  if ((15 * 25) + 21 < -iTempY + (m_iMapScreenY * 25) + (m_oCanvas->m_iHeight)) {
    g->setColor(0, 0, 0);

    if (m_iMapOffScreenY < 0) {
      g->fillRect(
          0, (((m_oCanvas->m_iHeight) / 25) * 25) + m_iMapOffScreenY, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
    } else if (m_iMapOffScreenY > 0) {
      g->fillRect(
          0, (((m_oCanvas->m_iHeight) / 25) * 25) - 25 + m_iMapOffScreenY, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
    } else {
      g->fillRect(0, (((m_oCanvas->m_iHeight) / 25) * 25) - 25, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
    }
  }
}

void StateGame::DrawFrame(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  int iX = (m_oCanvas->m_iWidth - 10) / 9;
  int iY = (m_oCanvas->m_iHeight - 26) / 9;
  int iXStart = 9;
  int iYStart = 9;

  if (iX < 15) {
    iX++;
    iXStart = 0;
  }
  if (iY < 9) {
    iY++;
    if (iY < 9) {
      iY++;
    }
    iYStart = 0;
  }

  if (game_state == StateGame::state::game_over) {
    iY++;
    iX++;
    iXStart = 0;
    iYStart = 0;
  }

  for (int x = 0; x < iX; x++) {
    for (int y = 0; y < iY; y++) {
      g->setClip(iXStart + (x * 9), iYStart + (y * 9), 9, 9);

      if (x == 0 && y == 0) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     iXStart - 135 + (x * 9),
                     iYStart - 9 + (y * 9),
                     Graphics::TOP | Graphics::LEFT);
      } else if (x == (iX - 1) && y == 0) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     iXStart - 117 + (x * 9),
                     iYStart - 9 + (y * 9),
                     Graphics::TOP | Graphics::LEFT);
      } else if (x == (iX - 1) && y == (iY - 1)) {
        g->drawImage(
            m_oCanvas->m_oImageArray[3], iXStart - 135 + (x * 9), iYStart + (y * 9), Graphics::TOP | Graphics::LEFT);
      } else if (x == 0 && y == (iY - 1)) {
        g->drawImage(
            m_oCanvas->m_oImageArray[3], iXStart - 117 + (x * 9), iYStart + (y * 9), Graphics::TOP | Graphics::LEFT);
      }

      else if (x == 0 && y != 0) {
        g->drawImage(
            m_oCanvas->m_oImageArray[3], iXStart - 144 + (x * 9), iYStart + (y * 9), Graphics::TOP | Graphics::LEFT);
      } else if (x == (iX - 1) && y != 0) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     iXStart - 144 + (x * 9),
                     iYStart - 9 + (y * 9),
                     Graphics::TOP | Graphics::LEFT);
      } else if (x != 0 && y == 0) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     iXStart - 126 + (x * 9),
                     iYStart - 9 + (y * 9),
                     Graphics::TOP | Graphics::LEFT);
      } else if (x != 0 && y == (iY - 1)) {
        g->drawImage(
            m_oCanvas->m_oImageArray[3], iXStart - 126 + (x * 9), iYStart + (y * 9), Graphics::TOP | Graphics::LEFT);
      }
    }
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

  g->setColor(226, 208, 184);
  g->fillRect(iXStart + 9, iYStart + 9, (iX - 2) * 9, (iY - 2) * 9);
}

bool StateGame::CheckEventCollision() {
  for (int i = 0; i < 15; i++) {
    if ((m_iEventX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX) &&
        (m_iEventY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY)) {
      do {
        m_iEventX[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (29));
        m_iEventY[i] = 25 * std::abs(m_oCanvas->m_oRand->nextInt() % (14));
      } while (MapCheckCollision((m_iEventX[i] / 25) + 0, (m_iEventY[i] / 25) - 0, false) == 0 ||
               ((m_iEventX[i] / 25) == ((m_iShipX / 25) + m_iMapScreenX) &&
                (m_iEventY[i] / 25) == ((m_iShipY / 25) + m_iMapScreenY)));

      return true;
    }
  }

  return false;
}

RegisterHighscore::RegisterHighscore(const std::string& temp) {
  m_cWebDataReady = 0;
  sLink = temp;
}

void RegisterHighscore::Deinit() {
  sLink = "";

  m_sWebHighAllPlayers = std::vector<std::string>();
  m_sWebHighAllScore = std::vector<std::string>();
  m_sWebHighDayPlayers = std::vector<std::string>();
  m_sWebHighDayScore = std::vector<std::string>();
  m_sWebHighMonPlayers = std::vector<std::string>();
  m_sWebHighMonScore = std::vector<std::string>();

  m_sWebHigh100AllPlayers = std::vector<std::string>();
  m_sWebHigh100AllScore = std::vector<std::string>();
  m_sWebHigh100DayPlayers = std::vector<std::string>();
  m_sWebHigh100DayScore = std::vector<std::string>();
  m_sWebHigh100MonPlayers = std::vector<std::string>();
  m_sWebHigh100MonScore = std::vector<std::string>();

  m_sWebHigh300AllPlayers = std::vector<std::string>();
  m_sWebHigh300AllScore = std::vector<std::string>();
  m_sWebHigh300DayPlayers = std::vector<std::string>();
  m_sWebHigh300DayScore = std::vector<std::string>();
  m_sWebHigh300MonPlayers = std::vector<std::string>();
  m_sWebHigh300MonScore = std::vector<std::string>();

  m_sWebHighAllPlace = std::vector<std::string>();
  m_sWebHighDayPlace = std::vector<std::string>();
  m_sWebHighMonPlace = std::vector<std::string>();
  m_sWebHigh100AllPlace = std::vector<std::string>();
  m_sWebHigh100DayPlace = std::vector<std::string>();
  m_sWebHigh100MonPlace = std::vector<std::string>();
  m_sWebHigh300AllPlace = std::vector<std::string>();
  m_sWebHigh300DayPlace = std::vector<std::string>();
  m_sWebHigh300MonPlace = std::vector<std::string>();
}

void RegisterHighscore::run() {
  try {
    m_Http = Connector::open(sLink, Connector::READ, true);
    m_Http->setRequestMethod(HttpConnection::GET);
    m_Http->setRequestProperty("Content-Type", "text/plain");
    m_Http->setRequestProperty("Connection", "close");
    m_Stream = m_Http->openInputStream(); // Send request and recive data

    // Read data to string
    int ch;
    while ((ch = m_Stream->read()) != -1) {
      m_sBuf->append(static_cast<char>(ch));
    }

    // Close connection
    if (m_Stream != nullptr) {
      m_Stream->close();
    }

    if (m_Http != nullptr) {
      m_Http->close();
    }
    m_Stream = nullptr;
    m_Http = nullptr;

    // Convert Buf to String
    std::string sTemp = m_sBuf->toString();

    // Create Strings
    m_sWebHighAllPlayers = std::vector<std::string>(5);
    m_sWebHighAllScore = std::vector<std::string>(5);

    m_sWebHighMonPlayers = std::vector<std::string>(5);
    m_sWebHighMonScore = std::vector<std::string>(5);
    m_sWebHighDayPlayers = std::vector<std::string>(5);
    m_sWebHighDayScore = std::vector<std::string>(5);

    m_sWebHigh100AllPlayers = std::vector<std::string>(5);
    m_sWebHigh100AllScore = std::vector<std::string>(5);
    m_sWebHigh100MonPlayers = std::vector<std::string>(5);
    m_sWebHigh100MonScore = std::vector<std::string>(5);
    m_sWebHigh100DayPlayers = std::vector<std::string>(5);
    m_sWebHigh100DayScore = std::vector<std::string>(5);

    m_sWebHigh300AllPlayers = std::vector<std::string>(5);
    m_sWebHigh300AllScore = std::vector<std::string>(5);
    m_sWebHigh300MonPlayers = std::vector<std::string>(5);
    m_sWebHigh300MonScore = std::vector<std::string>(5);
    m_sWebHigh300DayPlayers = std::vector<std::string>(5);
    m_sWebHigh300DayScore = std::vector<std::string>(5);

    m_sWebHighAllPlace = std::vector<std::string>(5);
    m_sWebHighDayPlace = std::vector<std::string>(5);
    m_sWebHighMonPlace = std::vector<std::string>(5);
    m_sWebHigh100AllPlace = std::vector<std::string>(5);
    m_sWebHigh100DayPlace = std::vector<std::string>(5);
    m_sWebHigh100MonPlace = std::vector<std::string>(5);
    m_sWebHigh300AllPlace = std::vector<std::string>(5);
    m_sWebHigh300DayPlace = std::vector<std::string>(5);
    m_sWebHigh300MonPlace = std::vector<std::string>(5);

    // Fill data to Highscore List
    int i, iJump;
    for (i = 0; i < 5; i++) {
      iJump = 0;
      auto tempVar = 0 + (33 * i) + (iJump);
      m_sWebHighAllPlace[i] = sTemp.substr(tempVar, 4 + (33 * i) + (iJump) - (tempVar));
      auto tempVar2 = 4 + (33 * i) + (iJump);
      m_sWebHighAllPlayers[i] = sTemp.substr(tempVar2, 23 + (33 * i) + (iJump) - (tempVar2));
      auto tempVar3 = 24 + (33 * i) + (iJump);
      m_sWebHighAllScore[i] = sTemp.substr(tempVar3, 33 + (33 * i) + (iJump) - (tempVar3));
    }
    for (i = 0; i < 5; i++) {
      iJump = 1 * 165;
      auto tempVar4 = 0 + (33 * i) + (iJump);
      m_sWebHighMonPlace[i] = sTemp.substr(tempVar4, 4 + (33 * i) + (iJump) - (tempVar4));
      auto tempVar5 = 4 + (33 * i) + (iJump);
      m_sWebHighMonPlayers[i] = sTemp.substr(tempVar5, 23 + (33 * i) + (iJump) - (tempVar5));
      auto tempVar6 = 24 + (33 * i) + (iJump);
      m_sWebHighMonScore[i] = sTemp.substr(tempVar6, 33 + (33 * i) + (iJump) - (tempVar6));
    }

    for (i = 0; i < 5; i++) {
      iJump = 2 * 165;
      auto tempVar7 = 0 + (33 * i) + (iJump);
      m_sWebHighDayPlace[i] = sTemp.substr(tempVar7, 4 + (33 * i) + (iJump) - (tempVar7));
      auto tempVar8 = 4 + (33 * i) + (iJump);
      m_sWebHighDayPlayers[i] = sTemp.substr(tempVar8, 23 + (33 * i) + (iJump) - (tempVar8));
      auto tempVar9 = 24 + (33 * i) + (iJump);
      m_sWebHighDayScore[i] = sTemp.substr(tempVar9, 33 + (33 * i) + (iJump) - (tempVar9));
    }
    for (i = 0; i < 5; i++) {
      iJump = 3 * 165;
      auto tempVar10 = 0 + (33 * i) + (iJump);
      m_sWebHigh100AllPlace[i] = sTemp.substr(tempVar10, 4 + (33 * i) + (iJump) - (tempVar10));
      auto tempVar11 = 4 + (33 * i) + (iJump);
      m_sWebHigh100AllPlayers[i] = sTemp.substr(tempVar11, 23 + (33 * i) + (iJump) - (tempVar11));
      auto tempVar12 = 24 + (33 * i) + (iJump);
      m_sWebHigh100AllScore[i] = sTemp.substr(tempVar12, 33 + (33 * i) + (iJump) - (tempVar12));
    }

    for (i = 0; i < 5; i++) {
      iJump = 4 * 165;
      auto tempVar13 = 0 + (33 * i) + (iJump);
      m_sWebHigh100MonPlace[i] = sTemp.substr(tempVar13, 4 + (33 * i) + (iJump) - (tempVar13));
      auto tempVar14 = 4 + (33 * i) + (iJump);
      m_sWebHigh100MonPlayers[i] = sTemp.substr(tempVar14, 23 + (33 * i) + (iJump) - (tempVar14));
      auto tempVar15 = 24 + (33 * i) + (iJump);
      m_sWebHigh100MonScore[i] = sTemp.substr(tempVar15, 33 + (33 * i) + (iJump) - (tempVar15));
    }
    for (i = 0; i < 5; i++) {
      iJump = 5 * 165;
      auto tempVar16 = 0 + (33 * i) + (iJump);
      m_sWebHigh100DayPlace[i] = sTemp.substr(tempVar16, 4 + (33 * i) + (iJump) - (tempVar16));
      auto tempVar17 = 4 + (33 * i) + (iJump);
      m_sWebHigh100DayPlayers[i] = sTemp.substr(tempVar17, 23 + (33 * i) + (iJump) - (tempVar17));
      auto tempVar18 = 24 + (33 * i) + (iJump);
      m_sWebHigh100DayScore[i] = sTemp.substr(tempVar18, 33 + (33 * i) + (iJump) - (tempVar18));
    }

    for (i = 0; i < 5; i++) {
      iJump = 6 * 165;
      auto tempVar19 = 0 + (33 * i) + (iJump);
      m_sWebHigh300AllPlace[i] = sTemp.substr(tempVar19, 4 + (33 * i) + (iJump) - (tempVar19));
      auto tempVar20 = 4 + (33 * i) + (iJump);
      m_sWebHigh300AllPlayers[i] = sTemp.substr(tempVar20, 23 + (33 * i) + (iJump) - (tempVar20));
      auto tempVar21 = 24 + (33 * i) + (iJump);
      m_sWebHigh300AllScore[i] = sTemp.substr(tempVar21, 33 + (33 * i) + (iJump) - (tempVar21));
    }
    for (i = 0; i < 5; i++) {
      iJump = 7 * 165;
      auto tempVar22 = 0 + (33 * i) + (iJump);
      m_sWebHigh300MonPlace[i] = sTemp.substr(tempVar22, 4 + (33 * i) + (iJump) - (tempVar22));
      auto tempVar23 = 4 + (33 * i) + (iJump);
      m_sWebHigh300MonPlayers[i] = sTemp.substr(tempVar23, 23 + (33 * i) + (iJump) - (tempVar23));
      auto tempVar24 = 24 + (33 * i) + (iJump);
      m_sWebHigh300MonScore[i] = sTemp.substr(tempVar24, 33 + (33 * i) + (iJump) - (tempVar24));
    }
    for (i = 0; i < 5; i++) {
      iJump = 8 * 165;
      auto tempVar25 = 0 + (33 * i) + (iJump);
      m_sWebHigh300DayPlace[i] = sTemp.substr(tempVar25, 4 + (33 * i) + (iJump) - (tempVar25));
      auto tempVar26 = 4 + (33 * i) + (iJump);
      m_sWebHigh300DayPlayers[i] = sTemp.substr(tempVar26, 23 + (33 * i) + (iJump) - (tempVar26));
      auto tempVar27 = 24 + (33 * i) + (iJump);
      m_sWebHigh300DayScore[i] = sTemp.substr(tempVar27, 33 + (33 * i) + (iJump) - (tempVar27));
    }

    m_cWebDataReady = 1;
  } catch (const std::runtime_error& e) {
    m_cWebDataReady = 2;
    sLink = e.what();
  }
}

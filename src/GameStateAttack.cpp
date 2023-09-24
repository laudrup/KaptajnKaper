#include "GameStateAttack.h"
#include "Canvas.h"
#include "Graphics.h"
#include "Image.h"
#include "Random.h"

CGameStateAttack::CGameStateAttack(KaperCanvas* oKaperCanvas) {
  m_bLocked = true;
  m_oCanvas = oKaperCanvas;

  m_iShipEnemyWaveA = std::vector<int>(3);
  m_iShipEnemyWaveX = std::vector<int>(3);
  m_iShipEnemyWaveY = std::vector<int>(3);
  m_iShipFriendlyWaveA = std::vector<int>(3);
  m_iShipFriendlyWaveX = std::vector<int>(3);
  m_iShipFriendlyWaveY = std::vector<int>(3);

  m_iEnemyCannonBallsX = std::vector<int>(50);
  m_iEnemyCannonBallsY = std::vector<int>(50);
  m_iEnemyCannonBallsXSpeed = std::vector<int>(50);
  m_iEnemyCannonBallsYSpeed = std::vector<int>(50);
  m_iEnemyCannonBallsBoost = std::vector<int>(50);
  m_iFriendlyCannonBallsX = std::vector<int>(50);
  m_iFriendlyCannonBallsY = std::vector<int>(50);
  m_iFriendlyCannonBallsXSpeed = std::vector<int>(50);
  m_iFriendlyCannonBallsYSpeed = std::vector<int>(50);
  m_iFriendlyCannonBallsBoost = std::vector<int>(50);

  m_iWaterSplashX = std::vector<int>(50);
  m_iWaterSplashY = std::vector<int>(50);
  m_iWaterSplashLife = std::vector<int>(50);
  m_iExplosionX = std::vector<int>(20);
  m_iExplosionY = std::vector<int>(20);
  m_iExplosionLife = std::vector<int>(20);
}

void CGameStateAttack::Init(int iEnemiesCannons, int iEnemiesSoldier) {
  m_bLocked = true;

  m_iKeyPause = 20;

  m_iEnemyCannons = iEnemiesCannons;
  m_iEnemySoldiers = iEnemiesSoldier;
  m_iFriendlyCannons = m_oCanvas->m_oStateGame->m_iResourceCannon;
  m_iFriendlyPirates = m_oCanvas->m_oStateGame->m_iResourcePirate;

  m_iShipEnemyWaveA[0] = 0;
  m_iShipEnemyWaveA[1] = 10;
  m_iShipEnemyWaveA[2] = 20;
  m_iShipFriendlyWaveA[0] = 0;
  m_iShipFriendlyWaveA[1] = 10;
  m_iShipFriendlyWaveA[2] = 20;

  m_iShipEnemyWaveX[0] = -20;
  m_iShipEnemyWaveX[1] = -20;
  m_iShipEnemyWaveX[2] = -20;
  m_iShipFriendlyWaveX[0] = -20;
  m_iShipFriendlyWaveX[1] = -20;
  m_iShipFriendlyWaveX[2] = -20;

  for (int i = 0; i < 50; i++) {
    m_iEnemyCannonBallsBoost[i] = 0;
    m_iFriendlyCannonBallsBoost[i] = 0;
    m_iWaterSplashLife[i] = 0;

    if (i < 20) {
      m_iExplosionLife[i] = 0;
    }
  }

  m_iBrainFire = std::abs(m_oCanvas->m_oRand->nextInt() % 200) + 10 + 20;
  m_iBrainX = 0;
  m_iBrainY = 0;
  m_iBrainNew = 0;

  m_iFriendlyCoolDown = 20;
  m_iEnemyCoolDown = 20;

  m_iEnemyBoatX = m_oCanvas->m_iWidth / 2;
  m_iEnemyBoatY = 5;
  m_iEnemyBoatBoost = 0;
  m_iFriendlyBoatX = m_oCanvas->m_iWidth / 2;
  m_iFriendlyBoatY = m_oCanvas->m_iHeight - 25;
  m_iFriendlyBoatBoost = 0;

  m_iWaterMoveOffset = 0;

  m_bGameOver = false;
  m_bRetreat = false;
  m_bBoard = false;

  m_bLeft = false;
  m_bRight = false;
  m_bUp = false;
  m_bDown = false;
  m_bFire = false;

  m_iFriendlyShipType = m_oCanvas->m_oStateGame->m_iShipLvl;
  m_bLocked = false;
}

void CGameStateAttack::Draw(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

  if (m_oCanvas->m_bSpeedOptimized == true) {
    g->setColor(2, 142, 239);
    g->fillRect(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
  } else {
    Image oImgWater = Image::createImage(25, 25);
    oImgWater.getGraphics()->drawImage(m_oCanvas->m_oImageArray[1], -165, -20, Graphics::TOP | Graphics::LEFT);

    for (int x = 0; x <= (m_oCanvas->m_iWidth / 25); x++) {
      for (int y = -1; y <= (m_oCanvas->m_iHeight / 25); y++) {
        g->drawImage(oImgWater, (x * 25), (y * 25) + m_iWaterMoveOffset, Graphics::TOP | Graphics::LEFT);
      }
    }

    m_iWaterMoveOffset++;
    if (m_iWaterMoveOffset >= 25) {
      m_iWaterMoveOffset = 0;
    }

    DrawWakes(g);
    DrawSplash(g);
  }

  DrawBoost(g);
  DrawBoats(g);
  DrawBalls(g);

  if (m_oCanvas->m_bSpeedOptimized == false) {
    DrawFlash(g);
  }

  DrawInterface(g);

  if (m_bGameOver == true) {
    g->setColor(0, 0, 0);
    m_oCanvas->DrawString("Battle over", (m_oCanvas->m_iWidth / 2) - 38, (m_oCanvas->m_iHeight / 2), true, 1);
  } else if (m_bRetreat == true) {
    g->setColor(0, 0, 0);
    m_oCanvas->DrawString("You ran like a dog", (m_oCanvas->m_iWidth / 2) - 64, (m_oCanvas->m_iHeight / 2), true, 1);
  } else if (m_bBoard == true) {
    g->setColor(0, 0, 0);
    m_oCanvas->DrawString("ATTACK!", (m_oCanvas->m_iWidth / 2) - 23, (m_oCanvas->m_iHeight / 2), true, 1);
  } else if (m_oCanvas->m_oStateGame->m_bShotButtonGuide == true) {
    g->setColor(0, 0, 0);
    m_oCanvas->DrawString(
        "Press [0] or [5] to fire", (m_oCanvas->m_iWidth / 2) - 64, (m_oCanvas->m_iHeight / 2), false, 1);
  }
}

int CGameStateAttack::Update() {
  if (m_bLocked) {
    return 0;
  }

  if (m_oCanvas->m_oStateMenu->m_bAnyContinueGame == true && m_iFriendlyPirates <= 0) {
    m_oCanvas->m_oStateGame->CleanGame(false);
    m_oCanvas->m_oStateMenu->m_bAnyContinueGame = false;
  }

  if (m_bGameOver || m_bRetreat || m_bBoard) {
    if (m_iKeyPause > 0) {
      m_iKeyPause--;
    }
    return 0;
  }

  if (m_iFriendlyBoatY > m_oCanvas->m_iHeight) {
    m_bRetreat = true;
    return 0;
  }

  if (m_iFriendlyCoolDown > 0) {
    m_iFriendlyCoolDown--;
  }
  if (m_iEnemyCoolDown > 0) {
    m_iEnemyCoolDown--;
  }

  if (m_iEnemySoldiers <= 0 || m_iFriendlyPirates <= 0) {
    m_bGameOver = true;
  }

  Move();

  if (m_iFriendlyBoatY < m_iEnemyBoatY + 15 && m_iFriendlyBoatY > m_iEnemyBoatY - 15) {
    if (m_iFriendlyBoatX < m_iEnemyBoatX + 15 && m_iFriendlyBoatX > m_iEnemyBoatX - 15) {
      m_bBoard = true;
      return 0;
    }
  }

  for (int i = 0; i < 50; i++) {
    if (m_iWaterSplashLife[i] > 0) {
      m_iWaterSplashLife[i] -= 2;
    }
  }
  for (int i = 0; i < 20; i++) {
    if (m_iExplosionLife[i] > 0) {
      m_iExplosionLife[i] -= 2;
    }
  }

  for (int i = 0; i < 49; i++) {
    if (m_iFriendlyCannonBallsBoost[i] > 1) {
      m_iFriendlyCannonBallsBoost[i]--;
      m_iFriendlyCannonBallsX[i] += m_iFriendlyCannonBallsXSpeed[i];
      m_iFriendlyCannonBallsY[i] += m_iFriendlyCannonBallsYSpeed[i];
    } else if (m_iFriendlyCannonBallsBoost[i] == 1) {
      bool bHit = false;

      m_iFriendlyCannonBallsBoost[i]--;

      if ((m_iFriendlyCannonBallsY[i] / 10) < m_iEnemyBoatY + 20 && (m_iFriendlyCannonBallsY[i] / 10) > m_iEnemyBoatY) {
        if ((m_iFriendlyCannonBallsX[i] / 10) < m_iEnemyBoatX + 20 &&
            (m_iFriendlyCannonBallsX[i] / 10) > m_iEnemyBoatX) {
          m_iEnemyCannons -= std::abs(m_oCanvas->m_oRand->nextInt() % 3);
          m_iEnemySoldiers -= std::abs(m_oCanvas->m_oRand->nextInt() % (15 + 1)) + 2;

          m_oCanvas->m_oStateGame->m_iScore++;

          if (m_iEnemyCannons < 0) {
            m_iEnemyCannons = 0;
          }
          if (m_iEnemySoldiers < 0) {
            m_iEnemySoldiers = 0;
          }

          bHit = true;
        }
      }

      if (bHit == false) // Water splash
      {
        for (int iSplash = 0; iSplash < 50; iSplash++) {
          if (m_iWaterSplashLife[iSplash] <= 0) {
            m_iWaterSplashLife[iSplash] = 30;
            m_iWaterSplashX[iSplash] = (m_iFriendlyCannonBallsX[i] / 10);
            m_iWaterSplashY[iSplash] = (m_iFriendlyCannonBallsY[i] / 10);
            break;
          }
        }

      } else // Explosion
      {
        for (int iSplash = 0; iSplash < 20; iSplash++) {
          if (m_iExplosionLife[iSplash] <= 0) {
            m_iExplosionLife[iSplash] = 30;
            m_iExplosionX[iSplash] = (m_iFriendlyCannonBallsX[i] / 10);
            m_iExplosionY[iSplash] = (m_iFriendlyCannonBallsY[i] / 10);
            break;
          }
        }
      }
    }

    if (m_iEnemyCannonBallsBoost[i] > 1) {
      m_iEnemyCannonBallsBoost[i]--;

      m_iEnemyCannonBallsY[i] += m_iEnemyCannonBallsYSpeed[i];
      m_iEnemyCannonBallsX[i] += m_iEnemyCannonBallsXSpeed[i];
    } else if (m_iEnemyCannonBallsBoost[i] == 1) {
      bool bHit = false;

      m_iEnemyCannonBallsBoost[i]--;

      if ((m_iEnemyCannonBallsY[i] / 10) < m_iFriendlyBoatY + 20 && (m_iEnemyCannonBallsY[i] / 10) > m_iFriendlyBoatY) {
        if (m_iEnemyCannonBallsX[i] / 10 < m_iFriendlyBoatX + 20 && m_iEnemyCannonBallsX[i] / 10 > m_iFriendlyBoatX) {
          m_iFriendlyCannons -= std::abs(m_oCanvas->m_oRand->nextInt() % 3);
          m_iFriendlyPirates -= std::abs(m_oCanvas->m_oRand->nextInt() % (15 + 1)) + 2;

          if (m_iFriendlyCannons < 0) {
            m_iFriendlyCannons = 0;
          }
          if (m_iFriendlyPirates < 0) {
            m_iFriendlyPirates = 0;
          }

          bHit = true;
        }
      }

      if (bHit == false) // Water splash
      {
        for (int iSplash = 0; iSplash < 50; iSplash++) {
          if (m_iWaterSplashLife[iSplash] <= 0) {
            m_iWaterSplashLife[iSplash] = 30;
            m_iWaterSplashX[iSplash] = (m_iEnemyCannonBallsX[i] / 10);
            m_iWaterSplashY[iSplash] = (m_iEnemyCannonBallsY[i] / 10);
            break;
          }
        }

      } else // Explosion
      {
        for (int iSplash = 0; iSplash < 20; iSplash++) {
          if (m_iExplosionLife[iSplash] <= 0) {
            m_iExplosionLife[iSplash] = 30;
            m_iExplosionX[iSplash] = (m_iEnemyCannonBallsX[i] / 10);
            m_iExplosionY[iSplash] = (m_iEnemyCannonBallsY[i] / 10);
            break;
          }
        }
      }
    }
  }

  // AI Handle
  TheBrain();

  return 0;
}

int CGameStateAttack::SoftKey(int iKey) {
  if (m_bLocked) {
    return 0;
  }

  KeyCheck(iKey);

  if (m_bGameOver && m_iKeyPause <= 0) {
    m_oCanvas->m_oStateGame->m_iResourceCannon = m_iFriendlyCannons;
    m_oCanvas->m_oStateGame->m_iResourcePirate = m_iFriendlyPirates;

    m_oCanvas->m_oStateGame->m_iEnemyShipCannons[m_oCanvas->m_oStateGame->m_iShipNr] = m_iEnemyCannons;
    m_oCanvas->m_oStateGame->m_iEnemyShipSoldiers[m_oCanvas->m_oStateGame->m_iShipNr] = m_iEnemySoldiers;

    return -1;
  }

  if (m_bRetreat && m_iKeyPause <= 0) // Flee
  {
    m_oCanvas->m_oStateGame->m_iResourceCannon = m_iFriendlyCannons;
    m_oCanvas->m_oStateGame->m_iResourcePirate = m_iFriendlyPirates;

    m_oCanvas->m_oStateGame->m_iEnemyShipCannons[m_oCanvas->m_oStateGame->m_iShipNr] = m_iEnemyCannons;
    m_oCanvas->m_oStateGame->m_iEnemyShipSoldiers[m_oCanvas->m_oStateGame->m_iShipNr] = m_iEnemySoldiers;

    return -2;
  }

  if (m_bBoard && m_iKeyPause <= 0) // Board
  {
    m_oCanvas->m_oStateGame->m_iResourceCannon = m_iFriendlyCannons;
    m_oCanvas->m_oStateGame->m_iResourcePirate = m_iFriendlyPirates;

    m_oCanvas->m_oStateGame->m_iEnemyShipCannons[m_oCanvas->m_oStateGame->m_iShipNr] = m_iEnemyCannons;
    m_oCanvas->m_oStateGame->m_iEnemyShipSoldiers[m_oCanvas->m_oStateGame->m_iShipNr] = m_iEnemySoldiers;

    return m_iEnemySoldiers;
  }

  return 0;
}

void CGameStateAttack::SoftKeyRelease(int iKey) {
  if (m_bLocked) {
    return;
  }

  if (iKey == Canvas::KEY_NUM5 || iKey == Canvas::KEY_NUM0) {
    m_bFire = false;

    if (m_iFriendlyBoatBoost != 0) {
      CalulateSpeedFriend();

      for (int iCan = 0; iCan <= (m_iFriendlyCannons / 3); iCan++) {
        if (m_iFriendlyCannons == 0) {
          break;
        }
        for (int i = 0; i < 49; i++) {
          if (m_iFriendlyCannonBallsBoost[i] <= 0) {
            m_iFriendlyCannonBallsX[i] = 10 * ((m_iFriendlyBoatX + 10) + (m_oCanvas->m_oRand->nextInt() % 7));
            m_iFriendlyCannonBallsY[i] = 10 * (m_iFriendlyBoatY + 10 + (m_oCanvas->m_oRand->nextInt() % 7));
            m_iFriendlyCannonBallsBoost[i] = m_iFriendlyBoatBoost + (m_oCanvas->m_oRand->nextInt() % 7);

            m_iFriendlyCannonBallsYSpeed[i] = m_iFriendYSpeed;
            m_iFriendlyCannonBallsXSpeed[i] = m_iFriendXSpeed;

            break;
          }
        }
      }
      m_iFriendlyBoatBoost = 0;
      m_iFriendlyCoolDown = 30;

      m_iFriendlyFireFlash = 10;
      m_iFriendlyFireFlashX = m_iFriendlyBoatX;
      m_iFriendlyFireFlashY = m_iFriendlyBoatY;
    }
  }

  int keyState = m_oCanvas->getGameAction(iKey);

  if ((keyState == Canvas::LEFT)) {
    m_bLeft = false;
  }
  if ((keyState == Canvas::RIGHT)) {
    m_bRight = false;
  }
  if ((keyState == Canvas::UP)) {
    m_bUp = false;
  }
  if ((keyState == Canvas::DOWN)) {
    m_bDown = false;
  }
}

void CGameStateAttack::DrawBalls(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  for (int i = 0; i < 49; i++) {
    if (m_iFriendlyCannonBallsBoost[i] > 0) {
      g->setClip((m_iFriendlyCannonBallsX[i] / 10), (m_iFriendlyCannonBallsY[i] / 10), 3, 3);
      g->drawImage(m_oCanvas->m_oImageArray[3],
                   (m_iFriendlyCannonBallsX[i] / 10) - 171,
                   (m_iFriendlyCannonBallsY[i] / 10),
                   Graphics::TOP | Graphics::LEFT);
    }
    if (m_iEnemyCannonBallsBoost[i] > 0) {
      g->setClip((m_iEnemyCannonBallsX[i] / 10), (m_iEnemyCannonBallsY[i] / 10), 3, 3);
      g->drawImage(m_oCanvas->m_oImageArray[3],
                   (m_iEnemyCannonBallsX[i] / 10) - 171,
                   (m_iEnemyCannonBallsY[i] / 10),
                   Graphics::TOP | Graphics::LEFT);
    }
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
}

void CGameStateAttack::DrawBoost(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  // Enemy boost strengt
  g->setColor(200, 20, 20);
  g->fillRect(m_oCanvas->m_iWidth - 9, 0, m_oCanvas->m_iWidth - 3, m_iEnemyBoatBoost);

  // Friendly boost strengt
  g->setColor(20, 200, 20);
  g->fillRect(m_oCanvas->m_iWidth - 9, m_oCanvas->m_iHeight - m_iFriendlyBoatBoost, 9, m_iFriendlyBoatBoost);

  // Enemy Cooldown
  g->setColor(200, 200, 20);
  g->fillRect(m_oCanvas->m_iWidth - 9, 0, m_oCanvas->m_iWidth - 3, m_iEnemyCoolDown);

  // Friendly Cooldown
  g->setColor(200, 200, 20);
  g->fillRect(m_oCanvas->m_iWidth - 9, m_oCanvas->m_iHeight - m_iFriendlyCoolDown, 9, m_iFriendlyCoolDown);

  if (m_iFriendlyBoatBoost > 0) {
    if ((m_iFriendlyBoatBoost % 5) == 1) {
      CalulateSpeedFriend();
    }

    // Bullet Target
    g->setClip(m_iFriendlyBoatX + ((m_iFriendlyBoatBoost * m_iFriendXSpeed) / 10) + 5,
               m_iFriendlyBoatY + ((m_iFriendYSpeed * m_iFriendlyBoatBoost) / 10),
               16,
               16);
    g->drawImage(m_oCanvas->m_oImageArray[3],
                 (m_iFriendlyBoatX + ((m_iFriendlyBoatBoost * m_iFriendXSpeed) / 10) + 5) - 162,
                 (m_iFriendlyBoatY + ((m_iFriendYSpeed * m_iFriendlyBoatBoost) / 10)) - 62,
                 Graphics::TOP | Graphics::LEFT);
  }
}

void CGameStateAttack::DrawBoats(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  // Draw Kaper Boat
  g->setClip(m_iFriendlyBoatX, m_iFriendlyBoatY, 19, 19);

  if (m_oCanvas->m_bSpeedOptimized == false) {
    if (2 >= m_iFriendlyShipType) {
      g->drawImage(
          m_oCanvas->m_oImageArray[0], m_iFriendlyBoatX - 140, m_iFriendlyBoatY - 80, Graphics::TOP | Graphics::LEFT);
    }
    if (2 < m_iFriendlyShipType && 4 >= m_iFriendlyShipType) {
      g->drawImage(
          m_oCanvas->m_oImageArray[3], m_iFriendlyBoatX - 140, m_iFriendlyBoatY - 60, Graphics::TOP | Graphics::LEFT);
    }
    if (4 < m_iFriendlyShipType) {
      g->drawImage(
          m_oCanvas->m_oImageArray[4], m_iFriendlyBoatX - 140, m_iFriendlyBoatY, Graphics::TOP | Graphics::LEFT);
    }
  } else {
    if (4 < m_iFriendlyShipType) {
      g->drawImage(
          m_oCanvas->m_oImageArray[4], m_iFriendlyBoatX - 140, m_iFriendlyBoatY, Graphics::TOP | Graphics::LEFT);
    } else {
      g->drawImage(
          m_oCanvas->m_oImageArray[3], m_iFriendlyBoatX - 140, m_iFriendlyBoatY - 60, Graphics::TOP | Graphics::LEFT);
    }
  }

  // Draw Enemy Boat
  g->setClip(m_iEnemyBoatX, m_iEnemyBoatY, 19, 19);
  if (m_oCanvas->m_oStateGame->m_iEventOld == 12 || m_oCanvas->m_oStateGame->m_iEventOld == 13) {
    g->drawImage(m_oCanvas->m_oImageArray[4], m_iEnemyBoatX - 140, m_iEnemyBoatY - 40, Graphics::TOP | Graphics::LEFT);
  }
  if (m_oCanvas->m_oStateGame->m_iEventOld == 14 || m_oCanvas->m_oStateGame->m_iEventOld == 15 ||
      m_oCanvas->m_oStateGame->m_iEventOld == 18) {
    g->drawImage(m_oCanvas->m_oImageArray[4], m_iEnemyBoatX - 140, m_iEnemyBoatY - 20, Graphics::TOP | Graphics::LEFT);
  }
  if (m_oCanvas->m_oStateGame->m_iEventOld == 16 || m_oCanvas->m_oStateGame->m_iEventOld == 17) {
    g->drawImage(m_oCanvas->m_oImageArray[4], m_iEnemyBoatX - 140, m_iEnemyBoatY - 60, Graphics::TOP | Graphics::LEFT);
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
}

void CGameStateAttack::DrawInterface(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  ///////////////////
  //--- PIRATES ---//
  ///////////////////

  // Draw interface-Icon
  if (m_oCanvas->m_bSpeedOptimized == false) {
    g->setClip(0, m_oCanvas->m_iHeight - 17, 14, 14);
    g->drawImage(m_oCanvas->m_oImageArray[3], (0) - 190, (m_oCanvas->m_iHeight - 17), Graphics::TOP | Graphics::LEFT);
    g->setClip(40, m_oCanvas->m_iHeight - 17, 14, 14);
    g->drawImage(m_oCanvas->m_oImageArray[3], (40) - 175, (m_oCanvas->m_iHeight - 17), Graphics::TOP | Graphics::LEFT);
    g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
  } else {
    g->setColor(0, 0, 255);
    g->drawString("M", 5, (m_oCanvas->m_iHeight - 15), Graphics::TOP | Graphics::LEFT);
    g->setColor(100, 100, 100);
    g->drawString("C", 45, (m_oCanvas->m_iHeight - 15), Graphics::TOP | Graphics::LEFT);
  }
  // Pirates
  g->setColor(255, 255, 255);
  m_oCanvas->DrawString(m_iFriendlyPirates, 15, m_oCanvas->m_iHeight - 15, false, 1);

  // Cannons
  g->setColor(255, 255, 255);
  m_oCanvas->DrawString(m_iFriendlyCannons, 56, m_oCanvas->m_iHeight - 15, false, 1);

  ///////////////////
  //--- SOLDIER ---//
  ///////////////////

  if (m_oCanvas->m_bSpeedOptimized == false) {
    // Draw interface-Icon
    g->setClip(0, 2, 14, 14);
    g->drawImage(m_oCanvas->m_oImageArray[3], (0) - 190, 2, Graphics::TOP | Graphics::LEFT);
    g->setClip(40, 2, 14, 14);
    g->drawImage(m_oCanvas->m_oImageArray[3], (40) - 175, 2, Graphics::TOP | Graphics::LEFT);
    g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
  } else {
    g->setColor(0, 0, 255);
    g->drawString("M", 5, 7, Graphics::TOP | Graphics::LEFT);
    g->setColor(100, 100, 100);
    g->drawString("C", 45, 7, Graphics::TOP | Graphics::LEFT);
  }
  // Soldiers
  g->setColor(255, 255, 255);
  m_oCanvas->DrawString(m_iEnemySoldiers, 15, 4, false, 1);

  // Cannons
  g->setColor(255, 255, 255);
  m_oCanvas->DrawString(m_iEnemyCannons, 56, 4, false, 1);
}

void CGameStateAttack::Move() {
  if (m_bLocked) {
    return;
  }

  if (m_bLeft) {
    if (m_iFriendlyBoatX > 1) {
      m_iFriendlyBoatX--;
    }
  }
  if (m_bRight) {
    if (m_iFriendlyBoatX + 20 < m_oCanvas->m_iWidth) {
      m_iFriendlyBoatX++;
    }
  }
  if (m_bDown) {
    m_iFriendlyBoatY++;
  }
  if (m_bUp) {
    if (m_iFriendlyBoatY > 1) {
      m_iFriendlyBoatY--;
    }
  }
  if (m_bFire && m_iFriendlyCoolDown <= 0 && m_iFriendlyCannons != 0) {
    m_iFriendlyBoatBoost += 2;
  }
}

void CGameStateAttack::KeyCheck(int iKey) {
  //------------------------------------------------------
  if (m_bLocked) {
    return;
  }

  if (iKey == Canvas::KEY_NUM5 || iKey == Canvas::KEY_NUM0) {
    m_bFire = true;
    m_oCanvas->m_oStateGame->m_bShotButtonGuide = false;
  }

  int keyState = m_oCanvas->getGameAction(iKey);

  if ((keyState == Canvas::LEFT)) {
    m_bLeft = true;
  }
  if ((keyState == Canvas::RIGHT)) {
    m_bRight = true;
  }
  if ((keyState == Canvas::UP)) {
    m_bUp = true;
  }
  if ((keyState == Canvas::DOWN)) {
    m_bDown = true;
  }
}

void CGameStateAttack::TheBrain() {
  if (m_bLocked) {
    return;
  }

  if (m_iBrainX == m_iEnemyBoatX && m_iBrainY == m_iEnemyBoatY) {
    if (std::abs(m_oCanvas->m_oRand->nextInt() % (30)) == 0) {
      m_iBrainNew = 0;
    }
  }

  if (m_iBrainNew <= 0 && m_iEnemyCannons <= 0) {
    m_iBrainX = m_iFriendlyBoatX + (m_oCanvas->m_oRand->nextInt() % 5);
    m_iBrainY = m_iFriendlyBoatY + (m_oCanvas->m_oRand->nextInt() % 5);
    m_iBrainNew = std::abs(m_oCanvas->m_oRand->nextInt() % 100);
  }

  else if (m_iBrainNew <= 0) {
    if (std::abs(m_oCanvas->m_oRand->nextInt() % (2)) == 0) {
      m_iBrainX = std::abs(m_oCanvas->m_oRand->nextInt() % (m_oCanvas->m_iWidth - 20));
      m_iBrainY = 20 + std::abs((m_oCanvas->m_oRand->nextInt() % 50));
    } else {
      m_iBrainX = m_iFriendlyBoatX + (m_oCanvas->m_oRand->nextInt() % 8);
      m_iBrainY = 20 + std::abs((m_oCanvas->m_oRand->nextInt() % (1 + m_iFriendlyBoatY)));
    }
    m_iBrainNew = std::abs(m_oCanvas->m_oRand->nextInt() % 150) + 50;
  }

  if (m_iBrainFire <= 0 && m_iEnemyCoolDown <= 0 && m_iEnemyCannons != 0) // Fire shot
  {
    m_iBrainFire = std::abs(m_oCanvas->m_oRand->nextInt() % 200) + 10;

    CalulateSpeedEnemy();

    for (int iCan = 0; iCan <= (m_iEnemyCannons / 3); iCan++) {
      for (int i = 0; i < 49; i++) {
        if (m_iEnemyCannonBallsBoost[i] <= 0) {
          m_iEnemyCannonBallsX[i] = 10 * ((m_iEnemyBoatX + 10) + (m_oCanvas->m_oRand->nextInt() % 7));
          m_iEnemyCannonBallsY[i] = 10 * (m_iEnemyBoatY + 10 + (m_oCanvas->m_oRand->nextInt() % 7));
          m_iEnemyCannonBallsBoost[i] = m_iEnemyBoatBoost + (m_oCanvas->m_oRand->nextInt() % 7);

          m_iEnemyCannonBallsYSpeed[i] = m_iEnemyYSpeed;
          m_iEnemyCannonBallsXSpeed[i] = m_iEnemyXSpeed;

          break;
        }
      }
    }
    m_iEnemyBoatBoost = 0;
    m_iEnemyCoolDown = 30;

    m_iEnemyFireFlash = 10;
    m_iEnemyFireFlashX = m_iEnemyBoatX;
    m_iEnemyFireFlashY = m_iEnemyBoatY;
  }

  int iDistance, iYDistance = 0, iXDistance = 0;
  if (m_iEnemyBoatY < m_iFriendlyBoatY) {
    iYDistance = m_iFriendlyBoatY - m_iEnemyBoatY;
  }
  if (m_iEnemyBoatY > m_iFriendlyBoatY) {
    iYDistance = m_iEnemyBoatY - m_iFriendlyBoatY;
  }
  if (m_iEnemyBoatX < m_iFriendlyBoatX) {
    iXDistance = m_iFriendlyBoatX - m_iEnemyBoatX;
  }
  if (m_iEnemyBoatX > m_iFriendlyBoatX) {
    iXDistance = m_iEnemyBoatX - m_iFriendlyBoatX;
  }

  if (iYDistance < iXDistance) {
    iDistance = (iXDistance + (5 * (iYDistance / 16)));
  } else {
    iDistance = (iYDistance + (5 * (iXDistance / 16)));
  }

  if (((m_iEnemyBoatBoost * 3)) > (iDistance - 10)) {
    if (std::abs(m_oCanvas->m_oRand->nextInt() % (2)) == 0) {
      m_iBrainFire = 0;
    }
  }

  m_iBrainNew--;
  m_iBrainFire--;
  if (m_iEnemyCoolDown <= 0 && m_iEnemyCannons != 0) {
    m_iEnemyBoatBoost += 2;
  }

  if (m_iBrainX < m_iEnemyBoatX) {
    m_iEnemyBoatX--;
  }
  if (m_iBrainX > m_iEnemyBoatX) {
    m_iEnemyBoatX++;
  }
  if (m_iBrainY < m_iEnemyBoatY) {
    m_iEnemyBoatY--;
  }
  if (m_iBrainY > m_iEnemyBoatY) {
    m_iEnemyBoatY++;
  }
}

void CGameStateAttack::DrawSplash(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  for (int iSplash = 0; iSplash < 50; iSplash++) {
    if (m_iWaterSplashLife[iSplash] > 0) {
      g->setClip(m_iWaterSplashX[iSplash], m_iWaterSplashY[iSplash], 10, 10);

      if (m_iWaterSplashLife[iSplash] > 20) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iWaterSplashX[iSplash] - 7,
                     m_iWaterSplashY[iSplash] - 62,
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iWaterSplashLife[iSplash] > 10) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iWaterSplashX[iSplash] - 18,
                     m_iWaterSplashY[iSplash] - 62,
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iWaterSplashLife[iSplash] > 0) {
        g->drawImage(m_oCanvas->m_oImageArray[0],
                     m_iWaterSplashX[iSplash] - 29,
                     m_iWaterSplashY[iSplash] - 62,
                     Graphics::TOP | Graphics::LEFT);
      }
    }
  }

  for (int iSplash = 0; iSplash < 20; iSplash++) {
    if (m_iExplosionLife[iSplash] > 0) {
      g->setClip(m_iExplosionX[iSplash], m_iExplosionY[iSplash], 10, 10);

      if (m_iExplosionLife[iSplash] > 20) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     m_iExplosionX[iSplash] - 106,
                     m_iExplosionY[iSplash],
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iExplosionLife[iSplash] > 10) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     m_iExplosionX[iSplash] - 95,
                     m_iExplosionY[iSplash],
                     Graphics::TOP | Graphics::LEFT);
      } else if (m_iExplosionLife[iSplash] > 0) {
        g->drawImage(m_oCanvas->m_oImageArray[3],
                     m_iExplosionX[iSplash] - 84,
                     m_iExplosionY[iSplash],
                     Graphics::TOP | Graphics::LEFT);
      }
    }
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
}

void CGameStateAttack::DrawWakes(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  int iGraphic = 0;

  // Enemy wakes
  for (int i = 0; i < 3; i++) {
    m_iShipEnemyWaveA[i]--;
    if (m_iShipEnemyWaveA[i] < 0) {
      m_iShipEnemyWaveA[i] = 30;
      m_iShipEnemyWaveX[i] = m_iEnemyBoatX;
      m_iShipEnemyWaveY[i] = m_iEnemyBoatY;
    }

    if (m_iShipEnemyWaveA[i] > 25) {
      continue;
    }

    if (m_iShipEnemyWaveA[i] < 6) {
      iGraphic = 3;
    } else if (m_iShipEnemyWaveA[i] < 12) {
      iGraphic = 2;
    } else if (m_iShipEnemyWaveA[i] < 18) {
      iGraphic = 1;
    } else if (m_iShipEnemyWaveA[i] < 25) {
      iGraphic = 0;
    }

    g->setClip(m_iShipEnemyWaveX[i], m_iShipEnemyWaveY[i], 19, 19);

    if (iGraphic == 0) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipEnemyWaveX[i] - 100,
                   m_iShipEnemyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (iGraphic == 1) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipEnemyWaveX[i] - 80,
                   m_iShipEnemyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (iGraphic == 2) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipEnemyWaveX[i] - 60,
                   m_iShipEnemyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (iGraphic == 3) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipEnemyWaveX[i] - 40,
                   m_iShipEnemyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
  }

  // Friendly wakes
  for (int i = 0; i < 3; i++) {
    m_iShipFriendlyWaveA[i]--;
    if (m_iShipFriendlyWaveA[i] < 0) {
      m_iShipFriendlyWaveA[i] = 30;
      m_iShipFriendlyWaveX[i] = m_iFriendlyBoatX;
      m_iShipFriendlyWaveY[i] = m_iFriendlyBoatY;
    }

    if (m_iShipFriendlyWaveA[i] > 25) {
      continue;
    }

    if (m_iShipFriendlyWaveA[i] < 6) {
      iGraphic = 3;
    } else if (m_iShipFriendlyWaveA[i] < 12) {
      iGraphic = 2;
    } else if (m_iShipFriendlyWaveA[i] < 18) {
      iGraphic = 1;
    } else if (m_iShipFriendlyWaveA[i] < 25) {
      iGraphic = 0;
    }

    g->setClip(m_iShipFriendlyWaveX[i], m_iShipFriendlyWaveY[i], 19, 19);

    if (iGraphic == 0) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipFriendlyWaveX[i] - 100,
                   m_iShipFriendlyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (iGraphic == 1) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipFriendlyWaveX[i] - 80,
                   m_iShipFriendlyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (iGraphic == 2) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipFriendlyWaveX[i] - 60,
                   m_iShipFriendlyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (iGraphic == 3) {
      g->drawImage(m_oCanvas->m_oImageArray[0],
                   m_iShipFriendlyWaveX[i] - 40,
                   m_iShipFriendlyWaveY[i] - 60,
                   Graphics::TOP | Graphics::LEFT);
    }
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
}

void CGameStateAttack::DrawFlash(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  // Friendly
  if (m_iFriendlyFireFlash > 0) {
    g->setClip(m_iFriendlyFireFlashX, m_iFriendlyFireFlashY - 5, 20, 20);

    if (m_iFriendlyFireFlash > 9 && m_iFriendlyFireFlash <= 10) {
      g->drawImage(m_oCanvas->m_oImageArray[3],
                   m_iFriendlyFireFlashX - 63,
                   m_iFriendlyFireFlashY - 5,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (m_iFriendlyFireFlash > 6 && m_iFriendlyFireFlash <= 9) {
      g->drawImage(m_oCanvas->m_oImageArray[3],
                   m_iFriendlyFireFlashX - 42,
                   m_iFriendlyFireFlashY - 5,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (m_iFriendlyFireFlash > 3 && m_iFriendlyFireFlash <= 6) {
      g->drawImage(m_oCanvas->m_oImageArray[3],
                   m_iFriendlyFireFlashX - 21,
                   m_iFriendlyFireFlashY - 5,
                   Graphics::TOP | Graphics::LEFT);
    }
    if (m_iFriendlyFireFlash > 0 && m_iFriendlyFireFlash <= 3) {
      g->drawImage(m_oCanvas->m_oImageArray[3],
                   m_iFriendlyFireFlashX,
                   m_iFriendlyFireFlashY - 5,
                   Graphics::TOP | Graphics::LEFT);
    }

    m_iFriendlyFireFlash--;
  }

  // Enemy
  if (m_iEnemyFireFlash > 0) {
    g->setClip(m_iEnemyFireFlashX, m_iEnemyFireFlashY + 5, 20, 20);

    if (m_iEnemyFireFlash > 9 && m_iEnemyFireFlash <= 10) {
      g->drawImage(
          m_oCanvas->m_oImageArray[3], m_iEnemyFireFlashX - 63, m_iEnemyFireFlashY + 5, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iEnemyFireFlash > 6 && m_iEnemyFireFlash <= 9) {
      g->drawImage(
          m_oCanvas->m_oImageArray[3], m_iEnemyFireFlashX - 42, m_iEnemyFireFlashY + 5, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iEnemyFireFlash > 3 && m_iEnemyFireFlash <= 6) {
      g->drawImage(
          m_oCanvas->m_oImageArray[3], m_iEnemyFireFlashX - 21, m_iEnemyFireFlashY + 5, Graphics::TOP | Graphics::LEFT);
    }
    if (m_iEnemyFireFlash > 0 && m_iEnemyFireFlash <= 3) {
      g->drawImage(
          m_oCanvas->m_oImageArray[3], m_iEnemyFireFlashX, m_iEnemyFireFlashY + 5, Graphics::TOP | Graphics::LEFT);
    }

    m_iEnemyFireFlash--;
  }

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);
}

void CGameStateAttack::CalulateSpeedEnemy() {
  int iXDistance = 0;
  int iYDistance = 0;
  int iDistance = 0;

  if (m_iEnemyBoatY < m_iFriendlyBoatY) {
    iYDistance = m_iFriendlyBoatY - m_iEnemyBoatY;
  }
  if (m_iEnemyBoatY > m_iFriendlyBoatY) {
    iYDistance = m_iEnemyBoatY - m_iFriendlyBoatY;
  }
  if (m_iEnemyBoatX < m_iFriendlyBoatX) {
    iXDistance = m_iFriendlyBoatX - m_iEnemyBoatX;
  }
  if (m_iEnemyBoatX > m_iFriendlyBoatX) {
    iXDistance = m_iEnemyBoatX - m_iFriendlyBoatX;
  }

  if (iYDistance < iXDistance) {
    iDistance = (iXDistance + (5 * (iYDistance / 16)));
  } else {
    iDistance = (iYDistance + (5 * (iXDistance / 16)));
  }

  m_iEnemyXSpeed = (((m_iFriendlyBoatX - m_iEnemyBoatX) * 6) / iDistance);
  m_iEnemyYSpeed = (((m_iFriendlyBoatY - m_iEnemyBoatY) * 30) / iDistance);

  if (m_iEnemyYSpeed > 0 && m_iEnemyYSpeed < 15) {
    m_iEnemyYSpeed = 15;
  }
  if (m_iEnemyYSpeed <= 0 && m_iEnemyYSpeed > -15) {
    m_iEnemyYSpeed = -15;
  }
}

void CGameStateAttack::CalulateSpeedFriend() {
  int iXDistance = 0;
  int iYDistance = 0;
  int iDistance = 0;

  if (m_iEnemyBoatY < m_iFriendlyBoatY) {
    iYDistance = m_iFriendlyBoatY - m_iEnemyBoatY;
  }
  if (m_iEnemyBoatY > m_iFriendlyBoatY) {
    iYDistance = m_iEnemyBoatY - m_iFriendlyBoatY;
  }
  if (m_iEnemyBoatX < m_iFriendlyBoatX) {
    iXDistance = m_iFriendlyBoatX - m_iEnemyBoatX;
  }
  if (m_iEnemyBoatX > m_iFriendlyBoatX) {
    iXDistance = m_iEnemyBoatX - m_iFriendlyBoatX;
  }

  if (iYDistance < iXDistance) {
    iDistance = (iXDistance + (5 * (iYDistance / 16)));
  } else {
    iDistance = (iYDistance + (5 * (iXDistance / 16)));
  }

  m_iFriendXSpeed = (((m_iEnemyBoatX - m_iFriendlyBoatX) * 6) / iDistance);
  m_iFriendYSpeed = (((m_iEnemyBoatY - m_iFriendlyBoatY) * 30) / iDistance);

  if (m_iFriendYSpeed > 0 && m_iFriendYSpeed < 15) {
    m_iFriendYSpeed = 15;
  }
  if (m_iFriendYSpeed <= 0 && m_iFriendYSpeed > -15) {
    m_iFriendYSpeed = -15;
  }
}

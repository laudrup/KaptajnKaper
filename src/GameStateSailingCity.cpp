#include "GameStateSailingCity.h"
#include "Canvas.h"
#include "Graphics.h"
#include "Image.h"
#include "Random.h"
#include "KaperCanvas.h"

CGameStateSailingCity::CGameStateSailingCity(KaperCanvas* oKaperCanvas) {
  m_bLocked = true;
  m_oCanvas = oKaperCanvas;

  // Calculate how many barricate to generate
  m_iNumberOfObliteries = ((m_oCanvas->m_iWidth / 9) * (m_oCanvas->m_iHeight / 9)) / 20;

  if (m_iNumberOfObliteries > 20) {
    m_iNumberOfObliteries = 20;
  }

  // Place them random at map
  m_iObjectsX = std::vector<int>(m_iNumberOfObliteries);
  m_iObjectsY = std::vector<int>(m_iNumberOfObliteries);
}

void CGameStateSailingCity::Init() {
  m_bLocked = true;

  // Set ship start position
  m_iShipY = 0;
  m_iShipX = m_oCanvas->m_iWidth / 2;

  m_iWind = (m_oCanvas->m_oRand->nextInt() % 3);

  for (int i = 0; i < m_iNumberOfObliteries; i++) {
    m_iObjectsX[i] = 0 + (std::abs(m_oCanvas->m_oRand->nextInt() % ((m_oCanvas->m_iWidth / 9) - 0)));
    m_iObjectsY[i] = 4 + (std::abs(m_oCanvas->m_oRand->nextInt() % ((m_oCanvas->m_iHeight / 9) - 7)));

    m_iObjectsX[i] *= 9;
    m_iObjectsY[i] *= 9;
  }

  m_bRight = false;
  m_bLeft = false;

  m_bPaused = true;
  m_iStatus = 5;

  m_bLocked = false;
}

void CGameStateSailingCity::Draw(Graphics* g) {
  if (m_bLocked) {
    return;
  }

  int i;

  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

  Image oImgWater = Image::createImage(9, 9);
  oImgWater.getGraphics()->drawImage(m_oCanvas->m_oImageArray[1], -90, -40, Graphics::TOP | Graphics::LEFT);

  if (m_bPaused || m_iStatus == 6) {
    if (m_iStatus == 6) {
      m_iStatus = 0;
    }
    // Draw Water

    for (int x = 0; x < (m_oCanvas->m_iWidth / 9) + 1; x++) {
      for (int y = 0; y < (m_oCanvas->m_iHeight / 9) + 1; y++) {
        g->drawImage(oImgWater, x * 9, y * 9, Graphics::TOP | Graphics::LEFT);
      }
    }
    // Draw Dock

    // Water splash
    for (i = 0; i <= ((m_oCanvas->m_iWidth / 9) / 2) - 2; i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 18, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 126, m_oCanvas->m_iHeight - 18 - 40, Graphics::TOP | Graphics::LEFT);
    }

    g->setClip((i + 0) * 9, m_oCanvas->m_iHeight - 18, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 0) * 9) - 135,
                 m_oCanvas->m_iHeight - 18 - 40,
                 Graphics::TOP | Graphics::LEFT);

    g->setClip((i + 2) * 9, m_oCanvas->m_iHeight - 18, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 2) * 9) - 153,
                 m_oCanvas->m_iHeight - 18 - 49,
                 Graphics::TOP | Graphics::LEFT);

    for (i += 3; i <= (m_oCanvas->m_iWidth / 9); i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 18, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 126, m_oCanvas->m_iHeight - 18 - 40, Graphics::TOP | Graphics::LEFT);
    }

    // Dock with Water splash
    for (i = 0; i < ((m_oCanvas->m_iWidth / 9) / 2) - 2; i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 9, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 108, m_oCanvas->m_iHeight - 9 - 40, Graphics::TOP | Graphics::LEFT);
    }
    g->setClip(i * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(
        m_oCanvas->m_oImageArray[1], (i * 9) - 99, m_oCanvas->m_iHeight - 9 - 40, Graphics::TOP | Graphics::LEFT);
    g->setClip((i + 1) * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 1) * 9) - 144,
                 m_oCanvas->m_iHeight - 9 - 40,
                 Graphics::TOP | Graphics::LEFT);

    g->setClip((i + 3) * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 3) * 9) - 153,
                 m_oCanvas->m_iHeight - 9 - 40,
                 Graphics::TOP | Graphics::LEFT);
    g->setClip((i + 4) * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 4) * 9) - 117,
                 m_oCanvas->m_iHeight - 9 - 40,
                 Graphics::TOP | Graphics::LEFT);

    for (i += 5; i <= (m_oCanvas->m_iWidth / 9); i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 9, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 108, m_oCanvas->m_iHeight - 9 - 40, Graphics::TOP | Graphics::LEFT);
    }
  }

  // Ship

  m_iShipX += 5;
  m_iShipY += 5;

  g->setClip(((m_iShipX / 9) * 9) - 0, ((m_iShipY / 9) * 9) - 9, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[1],
               ((m_iShipX / 9) * 9) - 90,
               ((m_iShipY / 9) * 9) - 49,
               Graphics::TOP | Graphics::LEFT);
  g->setClip(((m_iShipX / 9) * 9) - 9, ((m_iShipY / 9) * 9) - 9, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[1],
               ((m_iShipX / 9) * 9) - 99,
               ((m_iShipY / 9) * 9) - 49,
               Graphics::TOP | Graphics::LEFT);
  g->setClip(((m_iShipX / 9) * 9) + 9, ((m_iShipY / 9) * 9) - 9, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[1],
               ((m_iShipX / 9) * 9) - 81,
               ((m_iShipY / 9) * 9) - 49,
               Graphics::TOP | Graphics::LEFT);

  g->setClip(((m_iShipX / 9) * 9) + 9, ((m_iShipY / 9) * 9) - 0, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[1],
               ((m_iShipX / 9) * 9) - 81,
               ((m_iShipY / 9) * 9) - 40,
               Graphics::TOP | Graphics::LEFT);
  g->setClip(((m_iShipX / 9) * 9) - 0, ((m_iShipY / 9) * 9) - 0, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[1],
               ((m_iShipX / 9) * 9) - 90,
               ((m_iShipY / 9) * 9) - 40,
               Graphics::TOP | Graphics::LEFT);
  g->setClip(((m_iShipX / 9) * 9) - 9, ((m_iShipY / 9) * 9) - 0, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[1],
               ((m_iShipX / 9) * 9) - 99,
               ((m_iShipY / 9) * 9) - 40,
               Graphics::TOP | Graphics::LEFT);
  m_iShipX -= 5;
  m_iShipY -= 5;

  if (m_iShipY > (m_oCanvas->m_iHeight - 35)) {
    // Draw Dock

    // Water splash
    for (i = 0; i <= ((m_oCanvas->m_iWidth / 9) / 2) - 2; i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 18, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 126, m_oCanvas->m_iHeight - 18 - 40, Graphics::TOP | Graphics::LEFT);
    }

    g->setClip((i + 0) * 9, m_oCanvas->m_iHeight - 18, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 0) * 9) - 135,
                 m_oCanvas->m_iHeight - 18 - 40,
                 Graphics::TOP | Graphics::LEFT);

    g->setClip((i + 2) * 9, m_oCanvas->m_iHeight - 18, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 2) * 9) - 153,
                 m_oCanvas->m_iHeight - 18 - 49,
                 Graphics::TOP | Graphics::LEFT);

    for (i += 3; i <= (m_oCanvas->m_iWidth / 9); i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 18, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 126, m_oCanvas->m_iHeight - 18 - 40, Graphics::TOP | Graphics::LEFT);
    }

    // Dock with Water splash
    for (i = 0; i < ((m_oCanvas->m_iWidth / 9) / 2) - 2; i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 9, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 108, m_oCanvas->m_iHeight - 9 - 40, Graphics::TOP | Graphics::LEFT);
    }
    g->setClip(i * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(
        m_oCanvas->m_oImageArray[1], (i * 9) - 99, m_oCanvas->m_iHeight - 9 - 40, Graphics::TOP | Graphics::LEFT);
    g->setClip((i + 1) * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 1) * 9) - 144,
                 m_oCanvas->m_iHeight - 9 - 40,
                 Graphics::TOP | Graphics::LEFT);

    g->setClip((i + 3) * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 3) * 9) - 153,
                 m_oCanvas->m_iHeight - 9 - 40,
                 Graphics::TOP | Graphics::LEFT);
    g->setClip((i + 4) * 9, m_oCanvas->m_iHeight - 9, 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[1],
                 ((i + 4) * 9) - 117,
                 m_oCanvas->m_iHeight - 9 - 40,
                 Graphics::TOP | Graphics::LEFT);

    for (i += 5; i <= (m_oCanvas->m_iWidth / 9); i++) {
      g->setClip(i * 9, m_oCanvas->m_iHeight - 9, 9, 9);
      g->drawImage(
          m_oCanvas->m_oImageArray[1], (i * 9) - 108, m_oCanvas->m_iHeight - 9 - 40, Graphics::TOP | Graphics::LEFT);
    }
  }

  // Objects
  g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

  for (i = 0; i < m_iNumberOfObliteries; i++) {
    g->drawImage(oImgWater, ((m_iObjectsX[i] / 9) * 9), ((m_iObjectsY[i] / 9) * 9), Graphics::TOP | Graphics::LEFT);
    g->drawImage(oImgWater, ((m_iObjectsX[i] / 9) * 9) - 9, ((m_iObjectsY[i] / 9) * 9), Graphics::TOP | Graphics::LEFT);
    g->drawImage(oImgWater, ((m_iObjectsX[i] / 9) * 9) + 9, ((m_iObjectsY[i] / 9) * 9), Graphics::TOP | Graphics::LEFT);
  }

  for (i = 0; i < m_iNumberOfObliteries; i++) {

    g->setClip(m_iObjectsX[i], m_iObjectsY[i], 9, 9);
    g->drawImage(m_oCanvas->m_oImageArray[3], m_iObjectsX[i] - 153, m_iObjectsY[i], Graphics::TOP | Graphics::LEFT);

    if (m_bPaused == false) {
      if ((m_oCanvas->m_iHeight / 2) < m_iObjectsY[i]) {
        m_iObjectsX[i]++;
        if (m_iObjectsX[i] > (m_oCanvas->m_iWidth)) {
          m_iObjectsX[i] = -9;
        }
      } else {
        m_iObjectsX[i]--;
        if (m_iObjectsX[i] < -9) {
          m_iObjectsX[i] = m_oCanvas->m_iWidth;
        }
      }

      if (m_iShipY + 7 > (m_iObjectsY[i]) && m_iShipY < (7 + (m_iObjectsY[i]))) {
        if (m_iShipX + 7 > (m_iObjectsX[i]) && m_iShipX < (7 + (m_iObjectsX[i]))) {
          m_iStatus = 2;
          m_bPaused = true;
        }
      }
    }
  }

  g->setClip(m_iShipX, m_iShipY, 9, 9);
  g->drawImage(m_oCanvas->m_oImageArray[3], m_iShipX - 162, m_iShipY, Graphics::TOP | Graphics::LEFT);

  if (m_bPaused) {
    g->setClip(0, 0, m_oCanvas->m_iWidth, m_oCanvas->m_iHeight);

    // Draw text
    g->setColor(255, 255, 255);
    if (m_iStatus == 5) {
      if (m_iWind != 0) {
        if (m_iWind < 0) {
          m_oCanvas->DrawString(
              "Watch out for wind (<<<)", (m_oCanvas->m_iWidth / 2) - 58, (m_oCanvas->m_iHeight / 2), false, 1);
        }
        if (m_iWind > 0) {
          m_oCanvas->DrawString(
              "Watch out for wind (>>>)", (m_oCanvas->m_iWidth / 2) - 58, (m_oCanvas->m_iHeight / 2), false, 1);
        }
      }
    }

    if (m_iStatus == 1) {
      m_oCanvas->DrawString("You docked.", (m_oCanvas->m_iWidth / 2) - 58, (m_oCanvas->m_iHeight / 2), true, 1);
    }
    if (m_iStatus == 2) {
      m_oCanvas->DrawString("You crashed.", (m_oCanvas->m_iWidth / 2) - 58, (m_oCanvas->m_iHeight / 2), true, 1);
    }

    m_oCanvas->DrawString(
        "Press [5] to continue", (m_oCanvas->m_iWidth / 2) - 58, (m_oCanvas->m_iHeight / 2 + 10), false, 1);
  }
}

int CGameStateSailingCity::Update() {
  if (m_bLocked) {
    return 0;
  }

  if (m_bPaused == true) {
    return 0;
  }

  m_iShipY++;
  m_iShipX += m_iWind;

  // Side collision
  if (m_iShipX > (m_oCanvas->m_iWidth) || m_iShipX < -6) {
    m_iStatus = 2;
    m_bPaused = true;
  }

  // Dock collision
  if (m_iShipY > (m_oCanvas->m_iHeight - 14)) {
    if ((((m_oCanvas->m_iWidth) / 2) - 18) < m_iShipX && (((m_oCanvas->m_iWidth) / 2) + 9) > m_iShipX) {
      m_iStatus = 1;
      m_bPaused = true;
    } else {
      m_iStatus = 2;
      m_bPaused = true;
    }
  }

  if (m_bRight) {
    m_iShipX += 3;
  }
  if (m_bLeft) {
    m_iShipX -= 3;
  }
  if (m_bDown) {
    m_iShipY += 2;
  }

  return 0;
}

void CGameStateSailingCity::SoftKey(int iKey) {
  if (m_bLocked) {
    return;
  }

  if (iKey != 128) {
    int keyState = m_oCanvas->getGameAction(iKey);

    // H�jre
    if (keyState == Canvas::RIGHT) {
      m_bRight = true;
      return;
    }

    // Venstre
    if (keyState == Canvas::LEFT) {
      m_bLeft = true;
      return;
    }

    // Ned
    if (keyState == Canvas::DOWN) {
      m_bDown = true;
      return;
    }
  }

  m_bRight = false;
  m_bLeft = false;
  m_bDown = false;
}

#include "menu.h"

#include "game/mission/menu.h"

s16 menuButtonAtCoords(u16 x, u16 y, u16 buttonCount) {
  // Out of bounds horizontally?
  if ((x < menuX) || (x >= (menuX + menuButtonWidth))) {
    return -1;
  }

  // Above top button?
  if (y < menuY) {
    return -1;
  }

  // Above bottom of each button?
  u16 buttonBottomY = menuY + menuButtonHeight;
  for (u16 buttonIndex = 0; buttonIndex < buttonCount; ++buttonIndex) {
    if (y < buttonBottomY) {
      return buttonIndex;
    }

    buttonBottomY += menuButtonHeight;
  }

  return -1;
}

#ifndef GUARD_FULLSCREEN_POPUP_BG_H
#define GUARD_FULLSCREEN_POPUP_BG_H

#include "main.h"
#include "constants/fullscreen_popup_bg.h"

// Single image. Expects VAR_0x8000 = FULLSCREEN_POPUP_BG_*.
// Existing scripts keep using this.
void OpenFullscreenPopupBgFromScript(void);

// Multiple images, A advances, B closes. Expects VAR_0x8000 = FULLSCREEN_POPUP_SEQ_*.
void OpenFullscreenPopupSequenceFromScript(void);

// Image with a dialogue box. A/B advances text. Expects VAR_0x8000 = FULLSCREEN_POPUP_STORY_*.
void OpenFullscreenPopupStoryFromScript(void);

void FullscreenPopupBg_Init(MainCallback callback);

#endif // GUARD_FULLSCREEN_POPUP_BG_H

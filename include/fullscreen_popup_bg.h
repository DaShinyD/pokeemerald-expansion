#ifndef GUARD_FULLSCREEN_POPUP_BG_H
#define GUARD_FULLSCREEN_POPUP_BG_H

#include "main.h"

// Callnative entrypoint (expects VAR_0x8000 / gSpecialVar_0x8000 = popup id)
void OpenFullscreenPopupBgFromScript(void);

// If you want to open from other places (menus, etc)
void FullscreenPopupBg_Init(MainCallback callback);

// Popup IDs
enum FullscreenPopupBg
{
    FULLSCREEN_POPUP_BG_TEST,
    FULLSCREEN_POPUP_BG_KEMO,
};

#endif // GUARD_FULLSCREEN_POPUP_BG_H

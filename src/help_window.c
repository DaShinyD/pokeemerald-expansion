#include "global.h"
#include "menu.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "text.h"
#include "window.h"
#include "help_window.h"
#include "constants/songs.h"
#include "data/help_window.h"

EWRAM_DATA static u8 sHelpWindowId;
EWRAM_DATA static bool8 sHelpWindowActive;

static const struct WindowTemplate sHelpWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 3,
    .width = 26,
    .height = 14,
    .paletteNum = 15,
    .baseBlock = 8
};

static const u8 sHelpHeaderColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE, TEXT_COLOR_LIGHT_GRAY};

void ShowHelpInfoWindow(struct ScriptContext *ctx)
{
    u16 helpTutorialId = ScriptReadHalfword(ctx);
    u32 xOffset = 0;
    u32 yOffset = 1;
    u8 headerFont = FONT_NORMAL;
    u8 descFont = FONT_SMALL;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (helpTutorialId >= HELP_COUNT)
        helpTutorialId = 0;

    PlaySE(SE_RG_HELP_OPEN);

    if (sHelpWindowActive)
        HideHelpInfoWindow(ctx);

    sHelpWindowId = AddWindow(&sHelpWindowTemplate);
    sHelpWindowActive = TRUE;
    DrawStdWindowFrame(sHelpWindowId, FALSE);

    if (gHelpWindowInfo[helpTutorialId].headerFont)
        headerFont = gHelpWindowInfo[helpTutorialId].headerFont;
    if (gHelpWindowInfo[helpTutorialId].descFont)
        descFont = gHelpWindowInfo[helpTutorialId].descFont;

    StringCopy(gStringVar4, gHelpWindowInfo[helpTutorialId].header);
    AddTextPrinterParameterized4(sHelpWindowId, headerFont, xOffset, yOffset, 0, 0, sHelpHeaderColor, 0, gStringVar4);
    yOffset += 16;

    StringCopy(gStringVar4, gHelpWindowInfo[helpTutorialId].desc);
    AddTextPrinterParameterized(sHelpWindowId, descFont, gStringVar4, 0, yOffset, TEXT_SKIP_DRAW, NULL);

    CopyWindowToVram(sHelpWindowId, COPYWIN_FULL);
}

void HideHelpInfoWindow(struct ScriptContext *ctx)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (!sHelpWindowActive)
        return;

    PlaySE(SE_RG_HELP_CLOSE);
    ClearStdWindowAndFrameToTransparent(sHelpWindowId, FALSE);
    CopyWindowToVram(sHelpWindowId, COPYWIN_FULL);
    RemoveWindow(sHelpWindowId);
    sHelpWindowActive = FALSE;
}

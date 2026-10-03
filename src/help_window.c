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
    .height = 11,
    .paletteNum = 15,
    // BG0 owns tiles 0x000-0x2FF (char base 2 up to the map tilemaps at
    // VRAM 0xE000); a window running past that corrupts the map. 0x001-0x11E
    // also stays clear of the overworld's own text box (0x194), its message
    // frame (0x200) and the standard border this window's frame points at
    // (0x214), all of which are loaded once per map and never rewritten.
    .baseBlock = 0x001
};

static const u8 sHelpHeaderColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE, TEXT_COLOR_LIGHT_GRAY};

void HideHelpInfoWindowImmediate(void)
{
    if (!sHelpWindowActive)
        return;

    PlaySE(SE_RG_HELP_CLOSE);
    ClearStdWindowAndFrameToTransparent(sHelpWindowId, FALSE);
    CopyWindowToVram(sHelpWindowId, COPYWIN_FULL);
    RemoveWindow(sHelpWindowId);
    sHelpWindowActive = FALSE;
}

void ShowHelpInfoWindowId(u16 helpTutorialId)
{
    u32 xOffset = 0;
    u32 yOffset = 1;
    u8 headerFont = FONT_NORMAL;
    u8 descFont = FONT_SMALL;

    if (helpTutorialId >= HELP_COUNT)
        helpTutorialId = 0;

    PlaySE(SE_RG_HELP_OPEN);

    if (sHelpWindowActive)
        HideHelpInfoWindowImmediate();

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

void ShowHelpInfoWindow(struct ScriptContext *ctx)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    ShowHelpInfoWindowId(ScriptReadHalfword(ctx));
}

void HideHelpInfoWindow(struct ScriptContext *ctx)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    HideHelpInfoWindowImmediate();
}

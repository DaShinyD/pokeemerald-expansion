#include "global.h"
#include "fullscreen_popup_bg.h"
#include "strings.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "item_icon.h"
#include "item_use.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "overworld.h"
#include "event_data.h"
#include "window.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "constants/event_objects.h"
#include "random.h"

enum
{
    FULLSCREEN_POPUP_MODE_SINGLE,
    FULLSCREEN_POPUP_MODE_SEQUENCE,
    FULLSCREEN_POPUP_MODE_STORY,
};

#define STORY_MSG_TILE  1
#define STORY_MSG_PAL   DLG_WINDOW_PALETTE_NUM

static EWRAM_DATA u8 *sBg0TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;

struct FullscreenPopupBgResources
{
    u8 gfxLoadState;
    u8 mode;
    u8 listId;
    u8 pageIndex;
    u8 pageCount;
    u8 windowId;
    u8 currentImageId;
    MainCallback savedCallback;
};

struct FullscreenPopupBgAsset
{
    const u32 *tiles;
    const u32 *tilemap;
    const u16 *palette;
};

struct FullscreenPopupStoryPage
{
    u8 imageId;
    const u8 *text;
};

static EWRAM_DATA struct FullscreenPopupBgResources *sFullscreenPopupBgDataPtr = NULL;

static void FullscreenPopupBg_InitEx(MainCallback callback, u8 mode);
static void FullscreenPopupBg_RunSetup(void);
static bool8 FullscreenPopupBg_DoGfxSetup(void);
static bool8 FullscreenPopupBg_InitBgs(void);
static void FullscreenPopupBg_FadeAndBail(void);
static bool8 FullscreenPopupBg_LoadGraphics(u16 popupId);
static void FullscreenPopupBg_InitStoryWindow(void);
static void FullscreenPopupBg_PrintStoryPage(void);
static u8 FullscreenPopupBg_GetImageId(u8 page);
static void FullscreenPopupBg_BeginClose(u8 taskId);
static void FullscreenPopupBg_Advance(u8 taskId);

static void FullscreenPopupBg_MainCB(void);
static void FullscreenPopupBg_VBlankCB(void);

static void Task_FullscreenPopupBgWaitFadeIn(u8 taskId);
static void Task_FullscreenPopupBgMain(u8 taskId);
static void Task_FullscreenPopupBgTurnOff(u8 taskId);
static void Task_FullscreenPopupBgWaitFadeOutToSwap(u8 taskId);
static void Task_FullscreenPopupBgWaitFadeInAfterSwap(u8 taskId);

static const struct BgTemplate sFullscreenPopupBgTemplates[] =
{
    {
        .bg = 0,    // dialogue window (story mode)
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,    // fullscreen popup image
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 1, // 8bpp
        .priority = 1,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sStoryWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = STORY_MSG_PAL,
        .baseBlock = 0x20,
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sTestPopupTiles[]   = INCBIN_U32("graphics/fullscreen_popups/meme2.8bpp.lz");
static const u32 sTestPopupTilemap[] = INCBIN_U32("graphics/fullscreen_popups/meme2.bin.lz");
static const u16 sTestPopupPalette[] = INCBIN_U16("graphics/fullscreen_popups/meme2.gbapal");
static const u32 sKemoPopupTiles[]   = INCBIN_U32("graphics/fullscreen_popups/kemo2.8bpp.lz");
static const u32 sKemoPopupTilemap[] = INCBIN_U32("graphics/fullscreen_popups/kemo2.bin.lz");
static const u16 sKemoPopupPalette[] = INCBIN_U16("graphics/fullscreen_popups/kemo2.gbapal");

static const struct FullscreenPopupBgAsset sFullscreenPopupBgData[] =
{
    [FULLSCREEN_POPUP_BG_TEST]  =
    {
        .tiles   = sTestPopupTiles,
        .tilemap = sTestPopupTilemap,
        .palette = sTestPopupPalette,
    },
    [FULLSCREEN_POPUP_BG_KEMO]  =
    {
        .tiles = sKemoPopupTiles,
        .tilemap = sKemoPopupTilemap,
        .palette = sKemoPopupPalette,
    }
};

// Add image IDs here, end with FULLSCREEN_POPUP_END.
static const u8 sSeqTest[] =
{
    FULLSCREEN_POPUP_BG_TEST,
    FULLSCREEN_POPUP_BG_KEMO,
    FULLSCREEN_POPUP_END,
};

static const u8 *const sFullscreenPopupSequences[] =
{
    [FULLSCREEN_POPUP_SEQ_TEST] = sSeqTest,
};

static const u8 sStoryTestText0[] = _("This is a fullscreen story page.\p"
                                      "Press A to keep reading.");
static const u8 sStoryTestText1[] = _("The picture can change between\n"
                                      "pages, or stay the same.");

static const struct FullscreenPopupStoryPage sStoryTest[] =
{
    { FULLSCREEN_POPUP_BG_TEST, sStoryTestText0 },
    { FULLSCREEN_POPUP_BG_KEMO, sStoryTestText1 },
    { FULLSCREEN_POPUP_END, NULL },
};

static const struct FullscreenPopupStoryPage *const sFullscreenPopupStories[] =
{
    [FULLSCREEN_POPUP_STORY_TEST] = sStoryTest,
};

void OpenFullscreenPopupBgFromScript(void)
{
    CleanupOverworldWindowsAndTilemaps();
    FullscreenPopupBg_InitEx(CB2_ReturnToFieldContinueScript, FULLSCREEN_POPUP_MODE_SINGLE);
}

void OpenFullscreenPopupSequenceFromScript(void)
{
    CleanupOverworldWindowsAndTilemaps();
    FullscreenPopupBg_InitEx(CB2_ReturnToFieldContinueScript, FULLSCREEN_POPUP_MODE_SEQUENCE);
}

void OpenFullscreenPopupStoryFromScript(void)
{
    CleanupOverworldWindowsAndTilemaps();
    FullscreenPopupBg_InitEx(CB2_ReturnToFieldContinueScript, FULLSCREEN_POPUP_MODE_STORY);
}

void FullscreenPopupBg_Init(MainCallback callback)
{
    FullscreenPopupBg_InitEx(callback, FULLSCREEN_POPUP_MODE_SINGLE);
}

static u8 FullscreenPopupBg_CountSequencePages(const u8 *seq)
{
    u8 i;

    if (seq == NULL)
        return 0;

    for (i = 0; seq[i] != FULLSCREEN_POPUP_END; i++)
        ;

    return i;
}

static u8 FullscreenPopupBg_CountStoryPages(const struct FullscreenPopupStoryPage *story)
{
    u8 i;

    if (story == NULL)
        return 0;

    for (i = 0; story[i].imageId != FULLSCREEN_POPUP_END; i++)
        ;

    return i;
}

static void FullscreenPopupBg_InitEx(MainCallback callback, u8 mode)
{
    if ((sFullscreenPopupBgDataPtr = AllocZeroed(sizeof(struct FullscreenPopupBgResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sFullscreenPopupBgDataPtr->gfxLoadState = 0;
    sFullscreenPopupBgDataPtr->savedCallback = callback;
    sFullscreenPopupBgDataPtr->mode = mode;
    sFullscreenPopupBgDataPtr->listId = gSpecialVar_0x8000;
    sFullscreenPopupBgDataPtr->pageIndex = 0;
    sFullscreenPopupBgDataPtr->windowId = WINDOW_NONE;

    switch (mode)
    {
    case FULLSCREEN_POPUP_MODE_SEQUENCE:
        if (sFullscreenPopupBgDataPtr->listId >= NELEMS(sFullscreenPopupSequences))
            sFullscreenPopupBgDataPtr->listId = 0;
        sFullscreenPopupBgDataPtr->pageCount = FullscreenPopupBg_CountSequencePages(sFullscreenPopupSequences[sFullscreenPopupBgDataPtr->listId]);
        break;
    case FULLSCREEN_POPUP_MODE_STORY:
        if (sFullscreenPopupBgDataPtr->listId >= NELEMS(sFullscreenPopupStories))
            sFullscreenPopupBgDataPtr->listId = 0;
        sFullscreenPopupBgDataPtr->pageCount = FullscreenPopupBg_CountStoryPages(sFullscreenPopupStories[sFullscreenPopupBgDataPtr->listId]);
        break;
    default:
        sFullscreenPopupBgDataPtr->pageCount = 1;
        break;
    }

    if (sFullscreenPopupBgDataPtr->pageCount == 0)
        sFullscreenPopupBgDataPtr->pageCount = 1;

    SetMainCallback2(FullscreenPopupBg_RunSetup);
}

static void FullscreenPopupBg_RunSetup(void)
{
    while (1)
    {
        if (FullscreenPopupBg_DoGfxSetup() == TRUE)
            break;
    }
}

static void FullscreenPopupBg_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void FullscreenPopupBg_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

#define try_free(ptr) ({                  \
    void ** ptr__ = (void **)&(ptr);      \
    if (*ptr__ != NULL)                  \
        Free(*ptr__);                    \
})

static void FullscreenPopupBg_FreeResources(void)
{
    try_free(sFullscreenPopupBgDataPtr);
    try_free(sBg0TilemapBuffer);
    try_free(sBg1TilemapBuffer);
    FreeAllWindowBuffers();
}

static void Task_FullscreenPopupBgWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sFullscreenPopupBgDataPtr->savedCallback);
        FullscreenPopupBg_FreeResources();
        DestroyTask(taskId);
    }
}

static void FullscreenPopupBg_FadeAndBail(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_FullscreenPopupBgWaitFadeAndBail, 0);
    SetVBlankCallback(FullscreenPopupBg_VBlankCB);
    SetMainCallback2(FullscreenPopupBg_MainCB);
}

static bool8 FullscreenPopupBg_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg0TilemapBuffer = AllocZeroed(0x800);
    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg0TilemapBuffer == NULL || sBg1TilemapBuffer == NULL)
        return FALSE;
    memset(sBg1TilemapBuffer, 0, 0x800);

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sFullscreenPopupBgTemplates, NELEMS(sFullscreenPopupBgTemplates));
    SetBgTilemapBuffer(0, sBg0TilemapBuffer);
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);

    ScheduleBgCopyTilemapToVram(1);
    ShowBg(1);

    return TRUE;
}

static bool8 FullscreenPopupBg_LoadGraphics(u16 popupId)
{
    if (popupId >= NELEMS(sFullscreenPopupBgData))
        popupId = 0;

    sFullscreenPopupBgDataPtr->currentImageId = popupId;

    LZDecompressVram(sFullscreenPopupBgData[popupId].tiles, (void *)BG_CHAR_ADDR(0));

    {
        const int srcW = 30, srcH = 20;
        const int dstW = 32, dstH = 32;

        u16 *tmp16 = Alloc(srcW * srcH * sizeof(u16));
        u16 *dst16 = (u16 *)sBg1TilemapBuffer;

        if (tmp16 != NULL)
        {
            LZDecompressWram(sFullscreenPopupBgData[popupId].tilemap, tmp16);

            memset(dst16, 0, dstW * dstH * sizeof(u16));

            for (int y = 0; y < srcH; y++)
            {
                for (int x = 0; x < srcW; x++)
                {
                    dst16[y * dstW + x] = tmp16[y * srcW + x] & 0x03FF;
                }
            }

            Free(tmp16);
        }
        else
        {
            LZDecompressWram(sFullscreenPopupBgData[popupId].tilemap, sBg1TilemapBuffer);
        }
    }

    ScheduleBgCopyTilemapToVram(1);
    LoadPalette(sFullscreenPopupBgData[popupId].palette, 0, PLTT_SIZE_8BPP);
    return TRUE;
}

static u8 FullscreenPopupBg_GetImageId(u8 page)
{
    switch (sFullscreenPopupBgDataPtr->mode)
    {
    case FULLSCREEN_POPUP_MODE_SEQUENCE:
        return sFullscreenPopupSequences[sFullscreenPopupBgDataPtr->listId][page];
    case FULLSCREEN_POPUP_MODE_STORY:
        return sFullscreenPopupStories[sFullscreenPopupBgDataPtr->listId][page].imageId;
    default:
        return sFullscreenPopupBgDataPtr->listId;
    }
}

static void FullscreenPopupBg_InitStoryWindow(void)
{
    InitWindows(sStoryWindowTemplates);
    DeactivateAllTextPrinters();
    sFullscreenPopupBgDataPtr->windowId = 0;
    LoadMessageBoxGfx(sFullscreenPopupBgDataPtr->windowId, STORY_MSG_TILE, BG_PLTT_ID(STORY_MSG_PAL));
    DrawDialogFrameWithCustomTileAndPalette(sFullscreenPopupBgDataPtr->windowId, TRUE, STORY_MSG_TILE, STORY_MSG_PAL);
    ShowBg(0);
}

static void FullscreenPopupBg_PrintStoryPage(void)
{
    const u8 *text = sFullscreenPopupStories[sFullscreenPopupBgDataPtr->listId][sFullscreenPopupBgDataPtr->pageIndex].text;

    if (sFullscreenPopupBgDataPtr->windowId == WINDOW_NONE)
        return;

    if (text == NULL)
        text = gText_EmptyString2;

    FillWindowPixelBuffer(sFullscreenPopupBgDataPtr->windowId, PIXEL_FILL(1));
    StringExpandPlaceholders(gStringVar4, text);
    gTextFlags.canABSpeedUpPrint = TRUE;
    AddTextPrinterParameterized2(sFullscreenPopupBgDataPtr->windowId, FONT_NORMAL, gStringVar4, GetPlayerTextSpeedDelay(), NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    PutWindowTilemap(sFullscreenPopupBgDataPtr->windowId);
    CopyWindowToVram(sFullscreenPopupBgDataPtr->windowId, COPYWIN_FULL);
}

static bool8 FullscreenPopupBg_DoGfxSetup(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
        gMain.state++;
        break;

    case 1:
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        gMain.state++;
        break;

    case 2:
        if (FullscreenPopupBg_InitBgs())
        {
            sFullscreenPopupBgDataPtr->gfxLoadState = 0;
            gMain.state++;
        }
        else
        {
            FullscreenPopupBg_FadeAndBail();
            return TRUE;
        }
        break;

    case 3:
        if (FullscreenPopupBg_LoadGraphics(FullscreenPopupBg_GetImageId(0)) == TRUE)
            gMain.state++;
        break;

    case 4:
        if (sFullscreenPopupBgDataPtr->mode == FULLSCREEN_POPUP_MODE_STORY)
            FullscreenPopupBg_InitStoryWindow();
        taskId = CreateTask(Task_FullscreenPopupBgWaitFadeIn, 0);
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        gMain.state++;
        break;

    case 5:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;

    default:
        SetVBlankCallback(FullscreenPopupBg_VBlankCB);
        SetMainCallback2(FullscreenPopupBg_MainCB);
        return TRUE;
    }

    return FALSE;
}

static void Task_FullscreenPopupBgWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (sFullscreenPopupBgDataPtr->mode == FULLSCREEN_POPUP_MODE_STORY)
            FullscreenPopupBg_PrintStoryPage();
        gTasks[taskId].func = Task_FullscreenPopupBgMain;
    }
}

static void Task_FullscreenPopupBgTurnOff(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sFullscreenPopupBgDataPtr->savedCallback);
        FullscreenPopupBg_FreeResources();
        DestroyTask(taskId);
    }
}

static void FullscreenPopupBg_BeginClose(u8 taskId)
{
    PlaySE(SE_PC_OFF);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_FullscreenPopupBgTurnOff;
}

static void Task_FullscreenPopupBgWaitFadeOutToSwap(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        sFullscreenPopupBgDataPtr->pageIndex++;
        FullscreenPopupBg_LoadGraphics(FullscreenPopupBg_GetImageId(sFullscreenPopupBgDataPtr->pageIndex));
        if (sFullscreenPopupBgDataPtr->mode == FULLSCREEN_POPUP_MODE_STORY)
        {
            LoadMessageBoxGfx(sFullscreenPopupBgDataPtr->windowId, STORY_MSG_TILE, BG_PLTT_ID(STORY_MSG_PAL));
            DrawDialogFrameWithCustomTileAndPalette(sFullscreenPopupBgDataPtr->windowId, TRUE, STORY_MSG_TILE, STORY_MSG_PAL);
        }
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gTasks[taskId].func = Task_FullscreenPopupBgWaitFadeInAfterSwap;
    }
}

static void Task_FullscreenPopupBgWaitFadeInAfterSwap(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (sFullscreenPopupBgDataPtr->mode == FULLSCREEN_POPUP_MODE_STORY)
            FullscreenPopupBg_PrintStoryPage();
        gTasks[taskId].func = Task_FullscreenPopupBgMain;
    }
}

static void FullscreenPopupBg_Advance(u8 taskId)
{
    u8 nextPage = sFullscreenPopupBgDataPtr->pageIndex + 1;

    if (nextPage >= sFullscreenPopupBgDataPtr->pageCount)
    {
        FullscreenPopupBg_BeginClose(taskId);
        return;
    }

    PlaySE(SE_SELECT);

    if (sFullscreenPopupBgDataPtr->mode == FULLSCREEN_POPUP_MODE_STORY
     && FullscreenPopupBg_GetImageId(sFullscreenPopupBgDataPtr->pageIndex) == FullscreenPopupBg_GetImageId(nextPage))
    {
        sFullscreenPopupBgDataPtr->pageIndex = nextPage;
        FullscreenPopupBg_PrintStoryPage();
        return;
    }

    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_FullscreenPopupBgWaitFadeOutToSwap;
}

static void Task_FullscreenPopupBgMain(u8 taskId)
{
    if (gPaletteFade.active)
        return;

    switch (sFullscreenPopupBgDataPtr->mode)
    {
    case FULLSCREEN_POPUP_MODE_STORY:
        if (IsTextPrinterActive(sFullscreenPopupBgDataPtr->windowId))
            return;
        if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
            FullscreenPopupBg_Advance(taskId);
        break;
    case FULLSCREEN_POPUP_MODE_SEQUENCE:
        if (JOY_NEW(A_BUTTON))
            FullscreenPopupBg_Advance(taskId);
        else if (JOY_NEW(B_BUTTON))
            FullscreenPopupBg_BeginClose(taskId);
        break;
    default:
        if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
            FullscreenPopupBg_BeginClose(taskId);
        break;
    }
}

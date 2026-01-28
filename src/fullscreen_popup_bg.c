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
#include "text_window.h"
#include "overworld.h"
#include "event_data.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "constants/event_objects.h"
#include "random.h"

// Match your ui_screenshots style: one/two tilemap buffers in EWRAM
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
// static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL; // only needed if you want a second layer later

//==========STATIC=DEFINES==========//
struct FullscreenPopupBgResources
{
    u8 gfxLoadState;
    MainCallback savedCallback;
};

static EWRAM_DATA struct FullscreenPopupBgResources *sFullscreenPopupBgDataPtr = NULL;

static void FullscreenPopupBg_RunSetup(void);
static bool8 FullscreenPopupBg_DoGfxSetup(void);
static bool8 FullscreenPopupBg_InitBgs(void);
static void FullscreenPopupBg_FadeAndBail(void);
static bool8 FullscreenPopupBg_LoadGraphics(void);

static void FullscreenPopupBg_MainCB(void);
static void FullscreenPopupBg_VBlankCB(void);

static void Task_FullscreenPopupBgWaitFadeIn(u8 taskId);
static void Task_FullscreenPopupBgMain(u8 taskId);
static void Task_FullscreenPopupBgTurnOff(u8 taskId);

//==========CONST=DATA==========//
// Mirrors your screenshot bg templates: BG0 reserved, BG1 is the 8bpp image layer.
static const struct BgTemplate sFullscreenPopupBgTemplates[] =
{
    {
        .bg = 0,    // windows, etc (unused for now)
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 1,
    },
    {
        .bg = 1,    // fullscreen popup image
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 0,
        .paletteMode = 1, // 8bpp (same as ui_screenshots)
    },
    
};

// ---------- Assets (same conventions as ui_screenshots) ----------
// Put your compiled assets here:
//  graphics/fullscreen_popups/<name>_tiles.8bpp
//  graphics/fullscreen_popups/<name>_tiles.bin
//  graphics/fullscreen_popups/<name>_tiles.gbapal

static const u32 sTestPopupTiles[]   = INCBIN_U32("graphics/fullscreen_popups/meme2.8bpp.lz");
static const u32 sTestPopupTilemap[] = INCBIN_U32("graphics/fullscreen_popups/meme2.bin.lz");
static const u16 sTestPopupPalette[] = INCBIN_U16("graphics/fullscreen_popups/meme2.gbapal");
static const u32 sKemoPopupTiles[]   = INCBIN_U32("graphics/fullscreen_popups/kemo2.8bpp.lz");
static const u32 sKemoPopupTilemap[] = INCBIN_U32("graphics/fullscreen_popups/kemo2.bin.lz");
static const u16 sKemoPopupPalette[] = INCBIN_U16("graphics/fullscreen_popups/kemo2.gbapal");

struct FullscreenPopupBgAsset
{
    const u32 *tiles;
    const u32 *tilemap;
    const u16 *palette;
};

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

//==========FUNCTIONS==========//

void OpenFullscreenPopupBgFromScript(void)
{
    // Same pattern as OpenScreenshotsFromScript 
    CleanupOverworldWindowsAndTilemaps();
    FullscreenPopupBg_Init(CB2_ReturnToFieldContinueScript);
}

void FullscreenPopupBg_Init(MainCallback callback)
{
    if ((sFullscreenPopupBgDataPtr = AllocZeroed(sizeof(struct FullscreenPopupBgResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sFullscreenPopupBgDataPtr->gfxLoadState = 0;
    sFullscreenPopupBgDataPtr->savedCallback = callback;

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
    try_free(sBg1TilemapBuffer);
    // try_free(sBg2TilemapBuffer);
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

    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
        return FALSE;
    memset(sBg1TilemapBuffer, 0, 0x800);

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sFullscreenPopupBgTemplates, NELEMS(sFullscreenPopupBgTemplates));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);

    ScheduleBgCopyTilemapToVram(1);
    ShowBg(1);

    return TRUE;
}

static bool8 FullscreenPopupBg_LoadGraphics(void)
{
    u16 popupId = gSpecialVar_0x8000;
    if (popupId >= NELEMS(sFullscreenPopupBgData))
        popupId = 0;

    // 1) Tiles -> VRAM (charblock 0)
    LZDecompressVram(sFullscreenPopupBgData[popupId].tiles, (void *)BG_CHAR_ADDR(0));

    // 2) Tilemap -> Tilemap Studio output is 30x20 *u16* entries.
    //    Hardware expects a 32x32 u16 screenblock, so pad/copy into 32-wide rows.
    {
        const int srcW = 30, srcH = 20;
        const int dstW = 32, dstH = 32;

        u16 *tmp16 = Alloc(srcW * srcH * sizeof(u16));  // 1200 bytes
        u16 *dst16 = (u16 *)sBg1TilemapBuffer;          // 2048 bytes

        if (tmp16 != NULL)
        {
            LZDecompressWram(sFullscreenPopupBgData[popupId].tilemap, tmp16);

            memset(dst16, 0, dstW * dstH * sizeof(u16));

            for (int y = 0; y < srcH; y++)
            {
                for (int x = 0; x < srcW; x++)
                {
                    // Keep only tile index (0..1023). Tilemap Studio sets 0x1000.
                    dst16[y * dstW + x] = tmp16[y * srcW + x] & 0x03FF;
                }
            }

            Free(tmp16);
        }
        else
        {
            // Fallback: at least try direct decompress (won't pad correctly).
            LZDecompressWram(sFullscreenPopupBgData[popupId].tilemap, sBg1TilemapBuffer);
        }
    }

    ScheduleBgCopyTilemapToVram(1);

    // 3) Palette -> 8bpp full palette
    LoadPalette(sFullscreenPopupBgData[popupId].palette, 0, PLTT_SIZE_8BPP);

    return TRUE;
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
        // If you use scanline effects / sprite pals elsewhere, you can mirror screenshots more closely:
        // ScanlineEffect_Stop();
        // FreeAllSpritePalettes();
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
        if (FullscreenPopupBg_LoadGraphics() == TRUE)
            gMain.state++;
        break;

    case 4:
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
        gTasks[taskId].func = Task_FullscreenPopupBgMain;
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

static void Task_FullscreenPopupBgMain(u8 taskId)
{
    // Screenshots used B only; this supports A or B (easy QoL).
    if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_FullscreenPopupBgTurnOff;
    }
}

#include "global.h"
#include "quests.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "list_menu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/characters.h"
#include "constants/rgb.h"
#include "constants/songs.h"

struct QuestMenuResources
{
    MainCallback savedCallback;
    u8 gfxLoadState;
    u8 maxShowed;
    u8 nItems;
    u8 scrollIndicatorArrowPairId;
    u8 submenuWindowId;
    u8 oakSpriteId;
};

struct QuestMenuStaticResources
{
    MainCallback savedCallback;
    u16 scroll;
    u16 row;
};

EWRAM_DATA static struct QuestMenuResources *sQuestMenuData = NULL;
EWRAM_DATA static u8 *sBg1TilemapBuffer = NULL;
EWRAM_DATA static struct ListMenuItem *sListMenuItems = NULL;
EWRAM_DATA static struct QuestMenuStaticResources sListMenuState = {0};

static void QuestMenu_MainCB(void);
static void QuestMenu_VBlankCB(void);
static void QuestMenu_RunSetup(void);
static bool8 QuestMenu_DoGfxSetup(void);
static bool8 QuestMenu_LoadGraphics(void);
static void QuestMenu_FadeAndBail(void);
static void Task_QuestMenuWaitFadeAndBail(u8 taskId);
static bool8 QuestMenu_InitBgs(void);
static bool8 QuestMenu_AllocateResources(void);
static void QuestMenu_BuildListMenuTemplate(void);
static void QuestMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void QuestMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void QuestMenu_PrintHeader(void);
static void QuestMenu_PlaceScrollIndicatorArrows(void);
static void QuestMenu_RemoveScrollIndicatorArrows(void);
static void QuestMenu_SetCursorPosition(void);
static void QuestMenu_FreeResources(void);
static void Task_QuestMenuTurnOff(u8 taskId);
static void Task_QuestMenuMain(u8 taskId);
static void Task_QuestMenuSubmenuInit(u8 taskId);
static void Task_QuestMenuSubmenuRun(u8 taskId);
static void Task_QuestMenuDetails(u8 taskId);
static void Task_QuestMenuReward(u8 taskId);
static void Task_QuestMenuBeginQuest(u8 taskId);
static void Task_QuestMenuEndQuest(u8 taskId);
static void Task_QuestMenuDisplayMessage(u8 taskId);
static void Task_QuestMenuRefreshAfterAcknowledgement(u8 taskId);
static void Task_QuestMenuCleanUp(u8 taskId);
static void Task_QuestMenuCancel(u8 taskId);
static void QuestMenu_InitWindows(void);
static void QuestMenu_ReturnFromSubmenu(u8 taskId);
static void QuestMenu_SubmenuSelectionMessage(u8 taskId);
static u8 QuestMenu_GetCursorPosition(void);
static void QuestMenu_InitItems(void);
static u8 QuestMenu_CalcMaxShowed(void);
static bool8 IsActiveQuest(u8 questId);
static void ResetActiveQuest(void);
static void QuestMenu_PrintDescription(const u8 *desc);
static void QuestMenu_CreateOakSprite(void);
static void QuestMenu_DestroyOakSprite(void);

#define QUEST_MENU_OAK_SPRITE_X 20
#define QUEST_MENU_OAK_SPRITE_Y 130
#define QUEST_MENU_DESC_TEXT_X  8

// graphics/quest_menu/ — see graphics/quest_menu/README.md
static const u32 sQuestMenuTiles[] = INCBIN_U32("graphics/quest_menu/menu.4bpp.lz");
static const u32 sQuestMenuBgPals[] = INCBIN_U32("graphics/quest_menu/menu_pal.gbapal.lz");
static const u32 sQuestMenuTilemap[] = INCBIN_U32("graphics/quest_menu/tilemap.bin.lz");
static const u16 sQuestMenuMainWindowPal[] = INCBIN_U16("graphics/quest_menu/main_window.gbapal");

// FRLG-style window tiles (graphics/text_window/fr_*.png)
static const u16 sQuestMenuFrStdFrame0[] = INCBIN_U16("graphics/text_window/fr_std0.4bpp");
static const u16 sQuestMenuFrMessageBox[] = INCBIN_U16("graphics/text_window/fr_message_box.4bpp");

#define QUEST_MENU_FRAME_TILE 0x3A3
#define QUEST_MENU_FRAME_PAL  12
#define QUEST_MENU_MSG_TILE   0x3AC
#define QUEST_MENU_MSG_PAL    11

static void QuestMenu_LoadFrWindowTiles(u8 windowId)
{
    u8 bg = GetWindowAttribute(windowId, WINDOW_BG);

    LoadBgTiles(bg, sQuestMenuFrStdFrame0, 0x120, QUEST_MENU_FRAME_TILE);
    LoadBgTiles(bg, sQuestMenuFrMessageBox, 0x280, QUEST_MENU_MSG_TILE);
    LoadPalette(GetTextWindowPalette(3), PLTT_ID(QUEST_MENU_FRAME_PAL), PLTT_SIZE_4BPP);
    LoadPalette(GetTextWindowPalette(0), PLTT_ID(QUEST_MENU_MSG_PAL), PLTT_SIZE_4BPP);
}

static const u8 sText_Empty[] = _("");
static const u8 sText_QuestsHeader[] = _(" Side\n Quests");
static const u8 sText_QuestMenu_Begin[] = _("Begin");
static const u8 sText_QuestMenu_End[] = _("End");
static const u8 sText_QuestMenu_Details[] = _("Details");
static const u8 sText_QuestMenu_Reward[] = _("Reward");
static const u8 sText_QuestMenu_Unknown[] = _("?????????");
static const u8 sText_QuestMenu_UnknownDesc[] = _("?????????");
static const u8 sText_QuestMenu_Active[] = _("{COLOR}{07}Active");
static const u8 sText_QuestMenu_Complete[] = _("{COLOR}{09}Done");
static const u8 sText_QuestMenu_SelectedQuest[] = _("Do what with\nthis quest?");
static const u8 sText_QuestMenu_DisplayDetails[] = _("POC: {STR_VAR_1}\nMap: {STR_VAR_2}");
static const u8 sText_QuestMenu_DisplayReward[] = _("Reward:\n{STR_VAR_1}");
static const u8 sText_QuestMenu_BeginQuest[] = _("Initiating Quest:\n{STR_VAR_1}");
static const u8 sText_QuestMenu_EndQuest[] = _("Cancelling Quest:\n{STR_VAR_1}");

#define SIDE_QUEST_TEXT(num, title) \
    static const u8 sQuestName_##num[] = _(title); \
    static const u8 sQuestDesc_##num[] = _("Complete this side quest."); \
    static const u8 sQuestPoc_##num[] = _("???"); \
    static const u8 sQuestMap_##num[] = _("???"); \
    static const u8 sQuestReward_##num[] = _("???")

SIDE_QUEST_TEXT(1,  "Side Quest 1");
SIDE_QUEST_TEXT(2,  "Side Quest 2");
SIDE_QUEST_TEXT(3,  "Side Quest 3");
SIDE_QUEST_TEXT(4,  "Side Quest 4");
SIDE_QUEST_TEXT(5,  "Side Quest 5");
SIDE_QUEST_TEXT(6,  "Side Quest 6");
SIDE_QUEST_TEXT(7,  "Side Quest 7");
SIDE_QUEST_TEXT(8,  "Side Quest 8");
SIDE_QUEST_TEXT(9,  "Side Quest 9");
SIDE_QUEST_TEXT(10, "Side Quest 10");
SIDE_QUEST_TEXT(11, "Side Quest 11");
SIDE_QUEST_TEXT(12, "Side Quest 12");
SIDE_QUEST_TEXT(13, "Side Quest 13");
SIDE_QUEST_TEXT(14, "Side Quest 14");
SIDE_QUEST_TEXT(15, "Side Quest 15");
SIDE_QUEST_TEXT(16, "Side Quest 16");
SIDE_QUEST_TEXT(17, "Side Quest 17");
SIDE_QUEST_TEXT(18, "Side Quest 18");
SIDE_QUEST_TEXT(19, "Side Quest 19");
SIDE_QUEST_TEXT(20, "Side Quest 20");
SIDE_QUEST_TEXT(21, "Side Quest 21");
SIDE_QUEST_TEXT(22, "Side Quest 22");
SIDE_QUEST_TEXT(23, "Side Quest 23");
SIDE_QUEST_TEXT(24, "Side Quest 24");
SIDE_QUEST_TEXT(25, "Side Quest 25");
SIDE_QUEST_TEXT(26, "Side Quest 26");
SIDE_QUEST_TEXT(27, "Side Quest 27");
SIDE_QUEST_TEXT(28, "Side Quest 28");
SIDE_QUEST_TEXT(29, "Side Quest 29");
SIDE_QUEST_TEXT(30, "Side Quest 30");

#undef SIDE_QUEST_TEXT

#define side_quest_entry(num) \
    { \
        .name = sQuestName_##num, \
        .desc = sQuestDesc_##num, \
        .poc = sQuestPoc_##num, \
        .map = sQuestMap_##num, \
        .reward = sQuestReward_##num, \
    }

const struct SideQuest gSideQuests[SIDE_QUEST_COUNT] =
{
    side_quest_entry(1),
    side_quest_entry(2),
    side_quest_entry(3),
    side_quest_entry(4),
    side_quest_entry(5),
    side_quest_entry(6),
    side_quest_entry(7),
    side_quest_entry(8),
    side_quest_entry(9),
    side_quest_entry(10),
    side_quest_entry(11),
    side_quest_entry(12),
    side_quest_entry(13),
    side_quest_entry(14),
    side_quest_entry(15),
    side_quest_entry(16),
    side_quest_entry(17),
    side_quest_entry(18),
    side_quest_entry(19),
    side_quest_entry(20),
    side_quest_entry(21),
    side_quest_entry(22),
    side_quest_entry(23),
    side_quest_entry(24),
    side_quest_entry(25),
    side_quest_entry(26),
    side_quest_entry(27),
    side_quest_entry(28),
    side_quest_entry(29),
    side_quest_entry(30),
};

#undef side_quest_entry

static const struct MenuAction sQuestSubmenuOptions[] =
{
    {sText_QuestMenu_Begin,   {.void_u8 = Task_QuestMenuBeginQuest}},
    {sText_QuestMenu_Details, {.void_u8 = Task_QuestMenuDetails}},
    {gText_Cancel,            {.void_u8 = Task_QuestMenuCancel}},
};

static const struct MenuAction sActiveQuestSubmenuOptions[] =
{
    {sText_QuestMenu_End,     {.void_u8 = Task_QuestMenuEndQuest}},
    {sText_QuestMenu_Details, {.void_u8 = Task_QuestMenuDetails}},
    {gText_Cancel,            {.void_u8 = Task_QuestMenuCancel}},
};

static const struct MenuAction sCompletedQuestSubmenuOptions[] =
{
    {sText_QuestMenu_Reward,  {.void_u8 = Task_QuestMenuReward}},
    {sText_QuestMenu_Details, {.void_u8 = Task_QuestMenuDetails}},
    {gText_Cancel,            {.void_u8 = Task_QuestMenuCancel}},
};

static const struct BgTemplate sQuestMenuBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sQuestMenuWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 7,
        .tilemapTop = 1,
        .width = 19,
        .height = 12,
        .paletteNum = 15,
        .baseBlock = 0x020,
    },
    {
        .bg = 0,
        .tilemapLeft = 10,
        .tilemapTop = 14,
        .width = 20,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 0x104,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 5,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x17C,
    },
    {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop = 9,
        .width = 7,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 0x190,
    },
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 11,
        .baseBlock = 0x1C8,
    },
    DUMMY_WIN_TEMPLATE,
};

static const u8 sQuestMenuFontColors[][3] =
{
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY},
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
};

static const u8 sQuestMenuDescFontColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_DARK_GRAY};

void QuestMenu_Init(MainCallback callback)
{
    if (!FlagGet(FLAG_QUESTS_ACTIVATE))
    {
        SetMainCallback2(callback);
        return;
    }

    if ((sQuestMenuData = Alloc(sizeof(struct QuestMenuResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sListMenuState.savedCallback = callback;
    sListMenuState.scroll = 0;
    sListMenuState.row = 0;
    sQuestMenuData->scrollIndicatorArrowPairId = 0xFF;
    sQuestMenuData->submenuWindowId = 0xFF;
    sQuestMenuData->oakSpriteId = SPRITE_NONE;
    sQuestMenuData->gfxLoadState = 0;
    sQuestMenuData->savedCallback = callback;
    SetMainCallback2(QuestMenu_RunSetup);
}

static void QuestMenu_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void QuestMenu_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void QuestMenu_RunSetup(void)
{
    while (QuestMenu_DoGfxSetup() == FALSE)
        ;
}

static bool8 QuestMenu_DoGfxSetup(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 1:
        if (!QuestMenu_InitBgs())
        {
            QuestMenu_FadeAndBail();
            return TRUE;
        }
        sQuestMenuData->gfxLoadState = 0;
        gMain.state++;
        break;
    case 2:
        if (QuestMenu_LoadGraphics() == TRUE)
            gMain.state++;
        break;
    case 3:
        QuestMenu_InitWindows();
        QuestMenu_InitItems();
        QuestMenu_SetCursorPosition();
        gMain.state++;
        break;
    case 4:
        if (!QuestMenu_AllocateResources())
        {
            QuestMenu_FadeAndBail();
            return TRUE;
        }
        QuestMenu_BuildListMenuTemplate();
        QuestMenu_PrintHeader();
        QuestMenu_CreateOakSprite();
        gMain.state++;
        break;
    case 5:
        taskId = CreateTask(Task_QuestMenuMain, 0);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sListMenuState.scroll, sListMenuState.row);
        QuestMenu_PlaceScrollIndicatorArrows();
        LoadListMenuSwapLineGfx();
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(QuestMenu_VBlankCB);
        SetMainCallback2(QuestMenu_MainCB);
        return TRUE;
    }

    return FALSE;
}

static void QuestMenu_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_QuestMenuWaitFadeAndBail, 0);
    SetVBlankCallback(QuestMenu_VBlankCB);
    SetMainCallback2(QuestMenu_MainCB);
}

static void Task_QuestMenuWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sListMenuState.savedCallback);
        QuestMenu_FreeResources();
        FlagClear(FLAG_QUEST_MENU_ACTIVE);
        DestroyTask(taskId);
    }
}

static bool8 QuestMenu_InitBgs(void)
{
    ResetAllBgsCoordinates();
    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
        return FALSE;

    CpuFill16(0, sBg1TilemapBuffer, 0x800);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sQuestMenuBgTemplates, ARRAY_COUNT(sQuestMenuBgTemplates));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    ShowBg(1);
    return TRUE;
}

static bool8 QuestMenu_LoadGraphics(void)
{
    switch (sQuestMenuData->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sQuestMenuTiles, 0, 0, 0);
        sQuestMenuData->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sQuestMenuTilemap, sBg1TilemapBuffer);
            sQuestMenuData->gfxLoadState++;
        }
        break;
    case 2:
        LoadCompressedPalette(sQuestMenuBgPals, 0, 0x60);
        sQuestMenuData->gfxLoadState++;
        break;
    default:
        sQuestMenuData->gfxLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

static bool8 QuestMenu_AllocateResources(void)
{
    sListMenuItems = Alloc(sizeof(struct ListMenuItem) * (SIDE_QUEST_COUNT + 1));
    if (sListMenuItems == NULL)
        return FALSE;

    return TRUE;
}

static void QuestMenu_BuildListMenuTemplate(void)
{
    u16 i;

    for (i = 0; i < sQuestMenuData->nItems; i++)
    {
        if (GetSetQuestFlag(i, QUEST_FLAG_GET_UNLOCKED))
            sListMenuItems[i].name = gSideQuests[i].name;
        else
            sListMenuItems[i].name = sText_QuestMenu_Unknown;

        sListMenuItems[i].id = i;
    }

    sListMenuItems[i].name = gText_Cancel;
    sListMenuItems[i].id = LIST_CANCEL;

    gMultiuseListMenuTemplate.items = sListMenuItems;
    gMultiuseListMenuTemplate.totalItems = sQuestMenuData->nItems + 1;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 1;
    gMultiuseListMenuTemplate.itemVerticalPadding = 1;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sQuestMenuData->maxShowed;
    gMultiuseListMenuTemplate.fontId = FONT_NORMAL;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = QuestMenu_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = QuestMenu_ItemPrintFunc;
    gMultiuseListMenuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    gMultiuseListMenuTemplate.cursorKind = 0;
}

static void QuestMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    const u8 *desc;

    if (!onInit)
        PlaySE(SE_SELECT);

    if (itemIndex == LIST_CANCEL)
        desc = gText_Cancel2;
    else if (GetSetQuestFlag(itemIndex, QUEST_FLAG_GET_UNLOCKED))
        desc = gSideQuests[itemIndex].desc;
    else
        desc = sText_QuestMenu_UnknownDesc;

    QuestMenu_PrintDescription(desc);
}

static void QuestMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    if (itemId != LIST_CANCEL)
    {
        if (GetSetQuestFlag(itemId, QUEST_FLAG_GET_COMPLETED))
            StringCopy(gStringVar4, sText_QuestMenu_Complete);
        else if (IsActiveQuest(itemId))
            StringCopy(gStringVar4, sText_QuestMenu_Active);
        else
            StringCopy(gStringVar4, sText_Empty);

        AddTextPrinterParameterized4(windowId, FONT_NORMAL, 88, y, 0, 0, sQuestMenuFontColors[1], TEXT_SKIP_DRAW, gStringVar4);
    }
}

static void QuestMenu_PrintHeader(void)
{
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    AddTextPrinterParameterized4(2, FONT_NORMAL, 0, 1, 0, 1, sQuestMenuFontColors[0], TEXT_SKIP_DRAW, sText_QuestsHeader);
    PutWindowTilemap(2);
    CopyWindowToVram(2, COPYWIN_GFX);
}

static void QuestMenu_PrintDescription(const u8 *desc)
{
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    AddTextPrinterParameterized4(1, FONT_NORMAL, QUEST_MENU_DESC_TEXT_X, 3, 2, 0, sQuestMenuDescFontColors, TEXT_SKIP_DRAW, desc);
    CopyWindowToVram(1, COPYWIN_GFX);
}

static void QuestMenu_CreateOakSprite(void)
{
    u8 spriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_OAK, SpriteCallbackDummy, QUEST_MENU_OAK_SPRITE_X, QUEST_MENU_OAK_SPRITE_Y, 0);

    if (spriteId != MAX_SPRITES)
    {
        sQuestMenuData->oakSpriteId = spriteId;
        gSprites[spriteId].oam.priority = 0;
        StartSpriteAnim(&gSprites[spriteId], ANIM_STD_FACE_SOUTH);
    }
    else
    {
        sQuestMenuData->oakSpriteId = SPRITE_NONE;
    }
}

static void QuestMenu_DestroyOakSprite(void)
{
    if (sQuestMenuData != NULL && sQuestMenuData->oakSpriteId != SPRITE_NONE)
    {
        DestroySprite(&gSprites[sQuestMenuData->oakSpriteId]);
        sQuestMenuData->oakSpriteId = SPRITE_NONE;
    }
}

static void QuestMenu_PlaceScrollIndicatorArrows(void)
{
    if (sQuestMenuData->nItems + 1 > sQuestMenuData->maxShowed)
    {
        sQuestMenuData->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(
            SCROLL_ARROW_UP,
            120, 8, 104,
            (sQuestMenuData->nItems + 1) - sQuestMenuData->maxShowed,
            110, 110,
            &sListMenuState.scroll);
    }
}

static u8 QuestMenu_CalcMaxShowed(void)
{
    u8 windowHeightPx = 12 * 8;
    u8 yMultiplier = GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT) + 1;

    return (windowHeightPx - 2) / yMultiplier;
}

static void QuestMenu_RemoveScrollIndicatorArrows(void)
{
    if (sQuestMenuData->scrollIndicatorArrowPairId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sQuestMenuData->scrollIndicatorArrowPairId);
        sQuestMenuData->scrollIndicatorArrowPairId = 0xFF;
    }
}

static void QuestMenu_SetCursorPosition(void)
{
    if (sListMenuState.scroll != 0 && sListMenuState.scroll + sQuestMenuData->maxShowed > sQuestMenuData->nItems + 1)
        sListMenuState.scroll = (sQuestMenuData->nItems + 1) - sQuestMenuData->maxShowed;

    if (sListMenuState.scroll + sListMenuState.row >= sQuestMenuData->nItems + 1)
    {
        if (sQuestMenuData->nItems + 1 < 2)
            sListMenuState.row = 0;
        else
            sListMenuState.row = sQuestMenuData->nItems;
    }
}

static void QuestMenu_FreeResources(void)
{
    if (sQuestMenuData != NULL)
    {
        Free(sQuestMenuData);
        sQuestMenuData = NULL;
    }

    if (sBg1TilemapBuffer != NULL)
    {
        Free(sBg1TilemapBuffer);
        sBg1TilemapBuffer = NULL;
    }

    if (sListMenuItems != NULL)
    {
        Free(sListMenuItems);
        sListMenuItems = NULL;
    }

    QuestMenu_DestroyOakSprite();
    FreeAllWindowBuffers();
}

static void Task_QuestMenuTurnOff(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_QuestMenuWaitFadeAndBail;
}

static u8 QuestMenu_GetCursorPosition(void)
{
    return sListMenuState.scroll + sListMenuState.row;
}

static void QuestMenu_InitItems(void)
{
    u8 maxVisible = QuestMenu_CalcMaxShowed();

    sQuestMenuData->nItems = SIDE_QUEST_COUNT;
    if (sQuestMenuData->nItems + 1 <= maxVisible)
        sQuestMenuData->maxShowed = sQuestMenuData->nItems + 1;
    else
        sQuestMenuData->maxShowed = maxVisible;
}

static void Task_QuestMenuMain(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 input;

    if (gPaletteFade.active)
        return;

    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
        QuestMenu_RemoveScrollIndicatorArrows();
        gTasks[taskId].func = Task_QuestMenuTurnOff;
        return;
    }

    input = ListMenu_ProcessInput(data[0]);
    ListMenuGetScrollAndRow(data[0], &sListMenuState.scroll, &sListMenuState.row);

    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
        QuestMenu_RemoveScrollIndicatorArrows();
        gTasks[taskId].func = Task_QuestMenuTurnOff;
        break;
    default:
        if (GetSetQuestFlag(input, QUEST_FLAG_GET_UNLOCKED))
        {
            PlaySE(SE_SELECT);
            QuestMenu_RemoveScrollIndicatorArrows();
            data[1] = input;
            gTasks[taskId].func = Task_QuestMenuSubmenuInit;
        }
        else
        {
            PlaySE(SE_FAILURE);
        }
        break;
    }
}

static void QuestMenu_ReturnFromSubmenu(u8 taskId)
{
    QuestMenu_PlaceScrollIndicatorArrows();
    gTasks[taskId].func = Task_QuestMenuMain;
}

static void Task_QuestMenuSubmenuInit(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 questIndex = data[1];

    if (sQuestMenuData->submenuWindowId == 0xFF)
    {
        sQuestMenuData->submenuWindowId = AddWindow(&sQuestMenuWindowTemplates[3]);
        DrawStdFrameWithCustomTileAndPalette(sQuestMenuData->submenuWindowId, TRUE, QUEST_MENU_FRAME_TILE, QUEST_MENU_FRAME_PAL);
    }

    FillWindowPixelBuffer(sQuestMenuData->submenuWindowId, PIXEL_FILL(1));
    ClearWindowTilemap(sQuestMenuData->submenuWindowId);

    DrawStdFrameWithCustomTileAndPalette(3, FALSE, QUEST_MENU_FRAME_TILE, QUEST_MENU_FRAME_PAL);

    if (GetSetQuestFlag(questIndex, QUEST_FLAG_GET_COMPLETED))
    {
        PrintMenuTable(3, ARRAY_COUNT(sCompletedQuestSubmenuOptions), sCompletedQuestSubmenuOptions);
        InitMenuNormal(3, FONT_NORMAL, 8, 2, GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT) + 2,
                       ARRAY_COUNT(sCompletedQuestSubmenuOptions), 0);
    }
    else if (IsActiveQuest(questIndex))
    {
        PrintMenuTable(3, ARRAY_COUNT(sActiveQuestSubmenuOptions), sActiveQuestSubmenuOptions);
        InitMenuNormal(3, FONT_NORMAL, 8, 2, GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT) + 2,
                       ARRAY_COUNT(sActiveQuestSubmenuOptions), 0);
    }
    else
    {
        PrintMenuTable(3, ARRAY_COUNT(sQuestSubmenuOptions), sQuestSubmenuOptions);
        InitMenuNormal(3, FONT_NORMAL, 8, 2, GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT) + 2,
                       ARRAY_COUNT(sQuestSubmenuOptions), 0);
    }

    data[2] = 0;
    AddTextPrinterParameterized(sQuestMenuData->submenuWindowId, FONT_NORMAL, sText_QuestMenu_SelectedQuest, 0, 2, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sQuestMenuData->submenuWindowId, COPYWIN_FULL);
    PutWindowTilemap(3);
    CopyWindowToVram(3, COPYWIN_FULL);
    gTasks[taskId].func = Task_QuestMenuSubmenuRun;
}

static void Task_QuestMenuSubmenuRun(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 questIndex = data[1];
    s8 input = Menu_ProcessInputNoWrapAround_other();

    switch (input)
    {
    case MENU_NOTHING_CHOSEN:
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        Task_QuestMenuCancel(taskId);
        break;
    default:
        PlaySE(SE_SELECT);
        if (GetSetQuestFlag(questIndex, QUEST_FLAG_GET_COMPLETED))
            sCompletedQuestSubmenuOptions[input].func.void_u8(taskId);
        else if (IsActiveQuest(questIndex))
            sActiveQuestSubmenuOptions[input].func.void_u8(taskId);
        else
            sQuestSubmenuOptions[input].func.void_u8(taskId);
        break;
    }
}

static void QuestMenu_SubmenuSelectionMessage(u8 taskId)
{
    ClearStdWindowAndFrameToTransparent(3, FALSE);
    ClearWindowTilemap(3);

    if (sQuestMenuData->submenuWindowId != 0xFF)
    {
        ClearStdWindowAndFrameToTransparent(sQuestMenuData->submenuWindowId, FALSE);
        RemoveWindow(sQuestMenuData->submenuWindowId);
        sQuestMenuData->submenuWindowId = 0xFF;
    }
}

static void Task_QuestMenuDetails(u8 taskId)
{
    u8 questIndex = gTasks[taskId].data[1];

    QuestMenu_SubmenuSelectionMessage(taskId);
    StringCopy(gStringVar1, gSideQuests[questIndex].poc);
    StringCopy(gStringVar2, gSideQuests[questIndex].map);
    StringExpandPlaceholders(gStringVar4, sText_QuestMenu_DisplayDetails);
    Task_QuestMenuDisplayMessage(taskId);
}

static void Task_QuestMenuReward(u8 taskId)
{
    u8 questIndex = gTasks[taskId].data[1];

    QuestMenu_SubmenuSelectionMessage(taskId);
    StringCopy(gStringVar1, gSideQuests[questIndex].reward);
    StringExpandPlaceholders(gStringVar4, sText_QuestMenu_DisplayReward);
    Task_QuestMenuDisplayMessage(taskId);
}

static void Task_QuestMenuBeginQuest(u8 taskId)
{
    u8 questIndex = gTasks[taskId].data[1];

    SetActiveQuest(questIndex);
    QuestMenu_SubmenuSelectionMessage(taskId);
    StringCopy(gStringVar1, gSideQuests[questIndex].name);
    StringExpandPlaceholders(gStringVar4, sText_QuestMenu_BeginQuest);
    Task_QuestMenuDisplayMessage(taskId);
}

static void Task_QuestMenuEndQuest(u8 taskId)
{
    u8 questIndex = gTasks[taskId].data[1];

    ResetActiveQuest();
    QuestMenu_SubmenuSelectionMessage(taskId);
    StringCopy(gStringVar1, gSideQuests[questIndex].name);
    StringExpandPlaceholders(gStringVar4, sText_QuestMenu_EndQuest);
    Task_QuestMenuDisplayMessage(taskId);
}

static void Task_QuestMenuDisplayMessage(u8 taskId)
{
    FillWindowPixelBuffer(4, PIXEL_FILL(1));
    DrawStdFrameWithCustomTileAndPalette(4, FALSE, QUEST_MENU_MSG_TILE, QUEST_MENU_MSG_PAL);
    AddTextPrinterParameterized(4, FONT_NORMAL, gStringVar4, 0, 2, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(4);
    CopyWindowToVram(4, COPYWIN_FULL);
    gTasks[taskId].func = Task_QuestMenuRefreshAfterAcknowledgement;
}

static void Task_QuestMenuRefreshAfterAcknowledgement(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Task_QuestMenuCleanUp(taskId);
    }
}

static void Task_QuestMenuCleanUp(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    ClearStdWindowAndFrameToTransparent(4, FALSE);
    ClearWindowTilemap(4);

    DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
    QuestMenu_InitItems();
    QuestMenu_SetCursorPosition();
    QuestMenu_BuildListMenuTemplate();
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sListMenuState.scroll, sListMenuState.row);
    QuestMenu_ReturnFromSubmenu(taskId);
}

static void Task_QuestMenuCancel(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    ClearStdWindowAndFrameToTransparent(3, FALSE);
    ClearWindowTilemap(3);

    if (sQuestMenuData->submenuWindowId != 0xFF)
    {
        ClearStdWindowAndFrameToTransparent(sQuestMenuData->submenuWindowId, FALSE);
        RemoveWindow(sQuestMenuData->submenuWindowId);
        sQuestMenuData->submenuWindowId = 0xFF;
    }

    QuestMenu_ReturnFromSubmenu(taskId);
}

static void QuestMenu_InitWindows(void)
{
    u8 i;

    InitWindows(sQuestMenuWindowTemplates);
    DeactivateAllTextPrinters();
    QuestMenu_LoadFrWindowTiles(0);
    LoadPalette(sQuestMenuMainWindowPal, PLTT_ID(15), PLTT_SIZE_4BPP);

    for (i = 0; i <= 2; i++)
    {
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
        PutWindowTilemap(i);
        CopyWindowToVram(i, COPYWIN_GFX);
    }

    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    DrawStdFrameWithCustomTileAndPalette(0, FALSE, QUEST_MENU_FRAME_TILE, QUEST_MENU_FRAME_PAL);
    ScheduleBgCopyTilemapToVram(0);
    CopyWindowToVram(0, COPYWIN_FULL);
}

void Task_OpenQuestMenuFromStartMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetQuestMenuActive();
        QuestMenu_Init(CB2_ReturnToFullScreenStartMenu);
        DestroyTask(taskId);
    }
}

bool8 StartMenuQuestMenuCallback(void)
{
    if (!FlagGet(FLAG_QUESTS_ACTIVATE))
        return FALSE;

    CreateTask(Task_OpenQuestMenuFromStartMenu, 0);
    return TRUE;
}

void SetQuestMenuActive(void)
{
    FlagSet(FLAG_QUEST_MENU_ACTIVE);
}

s8 GetSetQuestFlag(u8 quest, u8 caseId)
{
    u8 index;
    u8 bit;
    u8 mask;

    if (quest >= SIDE_QUEST_COUNT)
        return -1;

    index = quest / 8;
    bit = quest % 8;
    mask = 1 << bit;

    switch (caseId)
    {
    case QUEST_FLAG_GET_UNLOCKED:
        return gSaveBlock2Ptr->unlockedQuests[index] & mask;
    case QUEST_FLAG_SET_UNLOCKED:
        gSaveBlock2Ptr->unlockedQuests[index] |= mask;
        return 1;
    case QUEST_FLAG_GET_COMPLETED:
        return gSaveBlock2Ptr->completedQuests[index] & mask;
    case QUEST_FLAG_SET_COMPLETED:
        gSaveBlock2Ptr->completedQuests[index] |= mask;
        return 1;
    default:
        return -1;
    }
}

s8 GetActiveQuestIndex(void)
{
    if (gSaveBlock2Ptr->activeQuest > 0)
        return gSaveBlock2Ptr->activeQuest - 1;

    return NO_ACTIVE_QUEST;
}

static bool8 IsActiveQuest(u8 questId)
{
    return (u8)GetActiveQuestIndex() == questId;
}

void SetActiveQuest(u8 questId)
{
    if (questId < SIDE_QUEST_COUNT)
        gSaveBlock2Ptr->activeQuest = questId + 1;
}

static void ResetActiveQuest(void)
{
    gSaveBlock2Ptr->activeQuest = 0;
}

void GetSetQuestFlagSpecial(void)
{
    gSpecialVar_Result = GetSetQuestFlag(gSpecialVar_0x8004, gSpecialVar_0x8005);
}

void SetActiveQuestSpecial(void)
{
    SetActiveQuest(gSpecialVar_0x8004);
}

void OpenQuestMenuSpecial(void)
{
    if (!FlagGet(FLAG_QUESTS_ACTIVATE))
        return;

    SetQuestMenuActive();
    QuestMenu_Init(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

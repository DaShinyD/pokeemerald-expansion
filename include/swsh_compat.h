#ifndef GUARD_SWSH_COMPAT_H
#define GUARD_SWSH_COMPAT_H

// Bridges the 1.17-era API names used by the ported Sword/Shield screens onto the
// 1.11.3 names this repo provides. Include this after the regular headers so the
// real declarations are visible to the wrappers below.

#include "global.h"
#include "battle.h"
#include "battle_pyramid.h"
#include "decompress.h"
#include "item.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "sprite.h"
#include "text.h"
#include "window.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/tms_hms.h"

// Item getters were renamed from ItemId_Get* to GetItem*.
#define GetItemPocket(itemId)             ItemId_GetPocket(itemId)
#define GetItemEffect(itemId)             ItemId_GetEffect(itemId)
#define GetItemFieldFunc(itemId)          ItemId_GetFieldFunc(itemId)
#define GetItemHoldEffectParam(itemId)    ItemId_GetHoldEffectParam(itemId)
#define GetItemImportance(itemId)         ItemId_GetImportance(itemId)
#define GetItemSecondaryId(itemId)        ItemId_GetSecondaryId(itemId)
#define GetItemName(itemId)               ItemId_GetName(itemId)
#define GetItemDescription(itemId)        ItemId_GetDescription(itemId)
#define GetItemType(itemId)               ItemId_GetType(itemId)
#define GetItemBattleUsage(itemId)        ItemId_GetBattleUsage(itemId)
#define GetItemPrice(itemId)              ItemId_GetPrice(itemId)
#define GetItemHoldEffect(itemId)         ItemId_GetHoldEffect(itemId)
#define GetItemFlingPower(itemId)         ItemId_GetFlingPower(itemId)
#define POCKET_DUMMY                      0

#define B_TRAINER_PLAYER   0
#define B_TRAINER_PARTNER  1
#define B_BATTLER_2        2

#define gPartiesCount ((u8[2]){ CalculatePlayerPartyCount(), CalculatePlayerPartyCount() > 3 ? (CalculatePlayerPartyCount() - 3) : 0 })

static inline struct Pokemon *SwShParty(u32 trainer)
{
    if (trainer == B_TRAINER_PARTNER)
        return &gPlayerParty[3];
    return gPlayerParty;
}

#define CalculatePartnerPartyCount() ((CalculatePlayerPartyCount() > 3) ? (CalculatePlayerPartyCount() - 3) : 0)
#define CalculatePartyCount(trainer) ((trainer) == B_TRAINER_PARTNER ? CalculatePartnerPartyCount() : CalculatePlayerPartyCount())

// The smol pipeline replaced the LZ77 decompressors with header-aware ones.
#define DecompressDataWithHeaderWram(src, dest)  LZDecompressWram((const u32 *)(src), (dest))
#define DecompressDataWithHeaderVram(src, dest)  LZDecompressVram((const u32 *)(src), (dest))

static inline u16 GetItemTMHMMoveId(u16 itemId)
{
    return ItemId_GetSecondaryId(itemId);
}

static inline u16 GetItemTMHMIndex(u16 itemId)
{
    if (itemId >= ITEM_HM01)
        return NUM_TECHNICAL_MACHINES + (itemId - ITEM_HM01 + 1);
    if (itemId >= ITEM_TM01)
        return itemId - ITEM_TM01 + 1;
    return ItemId_GetSecondaryId(itemId);
}

static inline u16 GetTMHMMoveId(u16 tmhmIndex)
{
    if (tmhmIndex > NUM_TECHNICAL_MACHINES)
        return ItemId_GetSecondaryId(ITEM_HM01 + (tmhmIndex - NUM_TECHNICAL_MACHINES - 1));
    return ItemId_GetSecondaryId(ITEM_TM01 + tmhmIndex - 1);
}

static inline struct ItemSlot BagPocket_GetSlotData(struct BagPocket *pocket, u32 slot)
{
    return pocket->itemSlots[slot];
}

static inline void BagPocket_SetSlotData(struct BagPocket *pocket, u32 slot, struct ItemSlot data)
{
    pocket->itemSlots[slot] = data;
}

static inline void CompactItemsInBagPocketId(u8 pocketId)
{
    CompactItemsInBagPocket(&gBagPockets[pocketId]);
}

static inline void RemoveBagItemFromSlot(struct BagPocket *pocket, u32 slot, u16 count)
{
    u16 itemId;

    if (slot >= pocket->capacity)
        return;
    itemId = pocket->itemSlots[slot].itemId;
    if (itemId != ITEM_NONE)
        RemoveBagItem(itemId, count);
}

#define SWSH_SPRITE_TEXT_MAX 8

static u8 sSpriteTextIds[SWSH_SPRITE_TEXT_MAX] UNUSED;
static u8 sSpriteTextCols UNUSED;
static u8 sSpriteTextRows UNUSED;
static u8 sSpriteTextCellW UNUSED;
static u8 sSpriteTextCellH UNUSED;

static inline void SwShGetOamPixelSize(const struct OamData *oam, u8 *width, u8 *height)
{
    static const u8 sHRectW[] = {16, 32, 32, 64};
    static const u8 sHRectH[] = {8, 8, 16, 32};
    static const u8 sVRectW[] = {8, 8, 16, 32};
    static const u8 sVRectH[] = {16, 32, 32, 64};
    u32 size = oam->size;

    switch (oam->shape)
    {
    case ST_OAM_H_RECTANGLE:
        *width = sHRectW[size];
        *height = sHRectH[size];
        break;
    case ST_OAM_V_RECTANGLE:
        *width = sVRectW[size];
        *height = sVRectH[size];
        break;
    default:
        *width = 8 << size;
        *height = 8 << size;
        break;
    }
}

static inline bool32 SwShSpriteTextUsesCanvas(u8 spriteId)
{
    return sSpriteTextCols != 0 && sSpriteTextIds[0] == spriteId;
}

static inline void SwShSpriteTextSetPixel(u8 spriteId, s32 x, s32 y, u8 color)
{
    u8 cellW, cellH;
    s32 localX, localY;
    u8 *tiles;
    u32 tileIndex, byteIndex, shift;

    if (SwShSpriteTextUsesCanvas(spriteId))
    {
        u32 col, row;

        if (x < 0 || y < 0)
            return;
        cellW = sSpriteTextCellW;
        cellH = sSpriteTextCellH;
        col = x / cellW;
        row = y / cellH;
        if (col >= sSpriteTextCols || row >= sSpriteTextRows)
            return;
        spriteId = sSpriteTextIds[row * sSpriteTextCols + col];
        localX = x % cellW;
        localY = y % cellH;
    }
    else
    {
        SwShGetOamPixelSize(&gSprites[spriteId].oam, &cellW, &cellH);
        if (x < 0 || y < 0 || x >= cellW || y >= cellH)
            return;
        localX = x;
        localY = y;
    }

    tiles = (u8 *)(OBJ_VRAM0 + gSprites[spriteId].oam.tileNum * TILE_SIZE_4BPP);
    tileIndex = (localY / 8) * (cellW / 8) + (localX / 8);
    byteIndex = tileIndex * TILE_SIZE_4BPP + (localY % 8) * 4 + (localX % 8) / 2;
    shift = (localX & 1) ? 4 : 0;
    tiles[byteIndex] = (tiles[byteIndex] & ~(0xF << shift)) | ((color & 0xF) << shift);
}

static inline void FillSpriteRectColor(u8 spriteId, s32 x, s32 y, s32 width, s32 height, u8 color)
{
    s32 px, py;

    for (py = 0; py < height; py++)
        for (px = 0; px < width; px++)
            SwShSpriteTextSetPixel(spriteId, x + px, y + py, color);
}

static inline void FillSpriteRectSprite(u8 spriteId, s32 x, s32 y, s32 width, s32 height)
{
    FillSpriteRectColor(spriteId, x, y, width, height, 0);
}

static inline void SetupSpritesForTextPrinting(const u8 *spriteIds, const void *srcUnused, u32 cols, u32 rows)
{
    u32 i;
    u32 count = cols * rows;

    (void)srcUnused;
    sSpriteTextCols = cols;
    sSpriteTextRows = rows;
    if (count > SWSH_SPRITE_TEXT_MAX)
        count = SWSH_SPRITE_TEXT_MAX;
    for (i = 0; i < count; i++)
        sSpriteTextIds[i] = spriteIds[i];
    if (count != 0)
        SwShGetOamPixelSize(&gSprites[spriteIds[0]].oam, &sSpriteTextCellW, &sSpriteTextCellH);
}

static inline u8 CreateSpriteWithDummyFallback(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    u8 spriteId = CreateSprite(template, x, y, subpriority);

    if (spriteId != MAX_SPRITES)
    {
        if (gSprites[spriteId].callback == NULL)
            gSprites[spriteId].callback = SpriteCallbackDummy;
        if (gSprites[spriteId].anims == NULL)
            gSprites[spriteId].anims = gDummySpriteAnimTable;
        if (gSprites[spriteId].affineAnims == NULL)
            gSprites[spriteId].affineAnims = gDummySpriteAffineAnimTable;
    }
    return spriteId;
}

static inline u8 CreateSpriteUnchecked(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    return CreateSpriteWithDummyFallback(template, x, y, subpriority);
}

#define CreateSprite CreateSpriteWithDummyFallback


#define EGG_ID_NONE 0
#define EGG_ICON 0
#define NORMAL_ICON 0
#define FEMALE_ICON 1
#define ITEM_USE_BATTLER 0
#define CHECK_EVO 0
#define DO_EVO 1
#define BerryInfo Berry
#define EXT_CTRL_CODE_TEXT_COLORS 0xFC

static inline u16 EvolutionTargetSpeciesCompat(struct Pokemon *mon, u32 mode, u16 item, struct Pokemon *partner)
{
    return GetEvolutionTargetSpecies(mon, mode, item, partner);
}
#define GetEvolutionTargetSpecies(mon, mode, item, partner, ...) EvolutionTargetSpeciesCompat(mon, mode, item, partner)

static inline bool32 TryBoxMonFormChange(struct BoxPokemon *boxMon, u16 method)
{
    u16 target = GetFormChangeTargetSpeciesBoxMon(boxMon, method, 0);
    u16 current = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);

    if (target != current)
    {
        SetBoxMonData(boxMon, MON_DATA_SPECIES, &target);
        return TRUE;
    }
    return FALSE;
}

#define GetFormChangeTargetSpecies_Internal GetFormChangeTargetSpeciesBoxMon
#define IsBoxMonExcluded(boxMon) FALSE
#define CanBoxMonBeSelected(boxMon) TRUE
#define IsVictoryCatch() FALSE

static inline bool32 HasShedinjaHPHandling(u16 species)
{
    return species == SPECIES_SHEDINJA;
}

#define GetCurrentPPToMaxPPState GetCurrentPpToMaxPpState
#define GetMonIconTilesByIconType(species, iconType) GetMonIconTiles((species), 0)
#define GetMonIconPtrIsEgg(species, personality, isEgg) GetMonIconPtr((isEgg) ? SPECIES_EGG : (species), (personality))
#define GetMonSpritePalFromSpeciesAndPersonalityIsEgg(species, shiny, pid, isEgg) GetMonSpritePalFromSpeciesAndPersonality((isEgg) ? SPECIES_EGG : (species), (shiny), (pid))
#define MoveItemSlotInPocket(pocketId, from, to) MoveItemSlotInList(gBagPockets[pocketId].itemSlots, (from), (to))
#define SWAP_EXTRA_MOVES_KYUREM_WHITE 0
#define SWAP_EXTRA_MOVES_KYUREM_BLACK 0
#define FORGET_EXTRA_MOVES 0

struct FormChangeContext
{
    u16 method;
    u16 currentSpecies;
    u16 partyItemUsed;
    u32 status;
};

#undef GetFormChangeTargetSpecies_Internal
static inline u16 GetFormChangeTargetSpecies_Internal(struct FormChangeContext ctx)
{
    (void)ctx;
    return ctx.currentSpecies;
}

static inline struct ItemSlot GetBagItemIdAndQuantity(u8 pocket, u16 pos)
{
    struct ItemSlot slot;

    slot.itemId = BagGetItemIdByPocketPosition(pocket + 1, pos);
    slot.quantity = BagGetQuantityByPocketPosition(pocket + 1, pos);
    return slot;
}

static inline u16 GetBagItemId(u8 pocket, u16 pos)
{
    return BagGetItemIdByPocketPosition(pocket + 1, pos);
}

static inline u32 GetItemSellPrice(u16 itemId)
{
    return ItemId_GetPrice(itemId) / 2;
}

static inline void AddSpriteTextPrinterParameterized6(u8 spriteId, u8 fontId, s8 x, s8 y, u8 letterSpacing, u8 lineSpacing, const u8 *color, s32 speed, const u8 *str)
{
    struct WindowTemplate winTemplate = {0};
    u8 windowId;
    u8 *tileData;
    s32 winW, winH, px, py;

    (void)speed;
    if (str == NULL || spriteId >= MAX_SPRITES)
        return;

    winW = (GetStringWidth(fontId, str, letterSpacing) + 7) / 8;
    if (winW < 1)
        winW = 1;
    if (winW > 16)
        winW = 16;
    winH = 2;
    winTemplate.width = winW;
    winTemplate.height = winH;
    windowId = AddWindow(&winTemplate);
    if (windowId == WINDOW_NONE)
        return;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(color ? color[0] : 0));
    AddTextPrinterParameterized4(windowId, fontId, 0, 0, letterSpacing, lineSpacing, color, TEXT_SKIP_DRAW, str);
    tileData = (u8 *)GetWindowAttribute(windowId, WINDOW_TILE_DATA);

    for (py = 0; py < winH * 8; py++)
    {
        for (px = 0; px < winW * 8; px++)
        {
            u32 tile = (py / 8) * winW + (px / 8);
            u32 byteIndex = tile * TILE_SIZE_4BPP + (py % 8) * 4 + (px % 8) / 2;
            u8 pair = tileData[byteIndex];
            u8 pix = (px & 1) ? (pair >> 4) : (pair & 0xF);

            if (pix != 0)
                SwShSpriteTextSetPixel(spriteId, x + px, y + py, pix);
        }
    }

    RemoveWindow(windowId);
}

static inline u16 GetSpeciesAbility(u16 species, u8 abilityNum)
{
    return gSpeciesInfo[species].abilities[abilityNum];
}

static inline bool32 IsOnPlayerSide(u32 battler)
{
    return GetBattlerSide(battler) == B_SIDE_PLAYER;
}

static inline void LoadSpritePaletteWithTag(const u16 *pal, u16 tag)
{
    struct SpritePalette spritePalette = { pal, tag };

    LoadSpritePalette(&spritePalette);
}

// This is Emerald, so the FireRed/LeafGreen-only paths are compiled away. The
// stubs exist only so those branches still type-check.
#define IS_FRLG                             FALSE
#define FIRST_BATTLE_MSG_FLAG_PARTY_MENU    0

static inline bool32 BtlCtrl_OakOldMan_TestState2Flag(u32 flag)
{
    return FALSE;
}

static inline void BtlCtrl_OakOldMan_SetState2Flag(u32 flag)
{
}

// A multi battle stores the partner's mons in the upper half of the player party
// rather than a party of their own, so full-team multi battles never occur.
static inline bool32 AreMultiPartiesFullTeams(void)
{
    return FALSE;
}

// The Battle Pyramid bag is reached through InBattlePyramid here.
#define PYRAMID_LOCATION_NONE 0

static inline u32 CurrentBattlePyramidLocation(void)
{
    return InBattlePyramid() ? 1 : PYRAMID_LOCATION_NONE;
}

// Eggs use the dedicated egg icon rather than their species icon.
static inline u8 CreateMonIconIsEgg(u16 species, void (*callback)(struct Sprite *), s16 x, s16 y, u8 subpriority, u32 personality, bool32 isEgg)
{
    return CreateMonIcon(isEgg ? SPECIES_EGG : species, callback, x, y, subpriority, personality);
}

// The move relearner asks for confirmation the vanilla way.
#define P_ASK_MOVE_CONFIRMATION FALSE

static inline bool32 CanBoxMonRelearnMoves(struct BoxPokemon *boxMon, u32 unused)
{
    struct Pokemon mon;
    u16 moves[MAX_LEVEL_UP_MOVES];

    BoxMonToMon(boxMon, &mon);
    return GetMoveRelearnerMoves(&mon, moves) != 0;
}

static inline u16 MonTryLearningNewMoveAtLevel(struct Pokemon *mon, bool8 firstMove, u8 level)
{
    u8 storedLevel = GetMonData(mon, MON_DATA_LEVEL, NULL);
    u16 move;

    SetMonData(mon, MON_DATA_LEVEL, &level);
    move = MonTryLearningNewMove(mon, firstMove);
    SetMonData(mon, MON_DATA_LEVEL, &storedLevel);
    return move;
}

#endif // GUARD_SWSH_COMPAT_H

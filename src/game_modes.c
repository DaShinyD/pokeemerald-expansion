#include "global.h"
#include "game_modes.h"
#include "event_data.h"
#include "fieldmap.h"
#include "item.h"
#include "main.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "save.h"
#include "string_util.h"
#include "strings.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/region_map_sections.h"
#include "constants/vars.h"

#define GM_LEVEL_CAPS       (1 << 0)
#define GM_SCALED           (1 << 1)
#define GM_NUZLOCKE         (1 << 2)
#define GM_WHITEOUT_DELETE  (1 << 3)
#define GM_RAND_TRAINERS    (1 << 4)
#define GM_RAND_WILD        (1 << 5)
#define GM_RAND_STATIC      (1 << 6)
#define GM_RAND_ITEMS       (1 << 7)

#define GM_INITIALIZED      (1 << 0)
#define GM_NUZLOCKE_ACTIVE  (1 << 1)
#define GM_WANDERERS        (1 << 2)

#define NUZLOCKE_ROUTE_BYTES 32
#define NUZLOCKE_GRAVEYARD_BOX (TOTAL_BOXES_COUNT - 1)

struct GameModeSave
{
    u8 flags;
    u8 flags2;
    u16 seed;
};

struct GameModeEdit
{
    bool8 levelCaps;
    bool8 scaled;
    bool8 wanderers;
    bool8 nuzlocke;
    bool8 whiteoutDelete;
    bool8 randTrainers;
    bool8 randWild;
    bool8 randStatic;
    bool8 randItems;
};

static EWRAM_DATA bool8 sNewGameOptions;
static EWRAM_DATA struct GameModeEdit sEdit;
static EWRAM_DATA bool8 sBlockCatch;
static EWRAM_DATA bool8 sPendingClose;
static EWRAM_DATA u16 sPendingMapSec;

static const u16 sFloorItemPool[] =
{
    ITEM_POKE_BALL,
    ITEM_GREAT_BALL,
    ITEM_ULTRA_BALL,
    ITEM_ALPHA_BALL,
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_HYPER_POTION,
    ITEM_MAX_POTION,
    ITEM_FULL_HEAL,
    ITEM_REVIVE,
    ITEM_REPEL,
    ITEM_SUPER_REPEL,
    ITEM_MAX_REPEL,
    ITEM_ETHER,
    ITEM_ELIXIR,
    ITEM_ANTIDOTE,
    ITEM_PARALYZE_HEAL,
    ITEM_AWAKENING,
    ITEM_BURN_HEAL,
    ITEM_ICE_HEAL,
    ITEM_ESCAPE_ROPE,
};

static const u8 sText_LevelCap[] = _("LEVEL CAP");
static const u8 sText_Scaled[] = _("SCALE LEVELS");
static const u8 sText_Wanderers[] = _("WANDERERS");
static const u8 sText_ColorVars[] = _("COLOR VARS");
static const u8 sText_Nuzlocke[] = _("NUZLOCKE");
static const u8 sText_Whiteout[] = _("WHITEOUT");
static const u8 sText_RandFoes[] = _("RAND FOES");
static const u8 sText_RandWild[] = _("RAND WILD");
static const u8 sText_RandStatic[] = _("RAND STATIC");
static const u8 sText_RandItems[] = _("RAND ITEMS");
// The option menu recolors bytes 2 and 5, so these carry the same control codes as gText_BattleSceneOn.
static const u8 sText_Delete[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}DELETE");
static const u8 sText_Party[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}PARTY");
static const u8 sText_FallenBox[] = _("Fallen");

static const u16 sLegacyCapFlags[] =
{
    FLAG_LEVEL_CAP_ONE,
    FLAG_LEVEL_CAP_TWO,
    FLAG_LEVEL_CAP_THREE,
    FLAG_LEVEL_CAP_FOUR,
    FLAG_LEVEL_CAP_FIVE,
    FLAG_LEVEL_CAP_SIX,
    FLAG_LEVEL_CAP_SEVEN,
    FLAG_LEVEL_CAP_EIGHT,
    FLAG_LEVEL_CAP_NINE,
    FLAG_LEVEL_CAP_TEN,
    FLAG_LEVEL_CAP_ELEVEN,
    FLAG_LEVEL_CAP_TWELVE,
    FLAG_LEVEL_CAP_THIRTEEN,
    FLAG_LEVEL_CAP_FOURTEEN,
};

static const u8 *const sPage1Names[] =
{
    sText_LevelCap,
    sText_Scaled,
    sText_Wanderers,
    sText_ColorVars,
};

static const u8 *const sPage2Names[] =
{
    sText_Nuzlocke,
    sText_Whiteout,
    sText_RandFoes,
    sText_RandWild,
    sText_RandStatic,
    sText_RandItems,
};

static struct GameModeSave *GetModeSave(void)
{
    return (struct GameModeSave *)gSaveBlock2Ptr->filler_90;
}

static u8 *GetRouteBits(void)
{
    return gPokemonStoragePtr->nuzlockeRouteBits;
}

static u32 GameRand(u32 salt)
{
    u32 x = GetModeSave()->seed;

    if (x == 0)
        x = 1;
    x ^= salt * 1664525u;
    x = x * 1664525u + 1013904223u;
    return x;
}

static void SyncWanderers(void)
{
    if ((GetModeSave()->flags2 & GM_WANDERERS) && !IsNuzlockeEnabled())
        FlagClear(FLAG_SYS_ROUTE_WILD_WANDERERS_DISABLED);
    else
        FlagSet(FLAG_SYS_ROUTE_WILD_WANDERERS_DISABLED);
}

static void NameGraveyardBox(void)
{
    StringCopy(GetBoxNamePtr(NUZLOCKE_GRAVEYARD_BOX), sText_FallenBox);
}

static bool8 LegacyChoseNoCaps(void)
{
    u32 i;

    if (FlagGet(FLAG_LEVEL_CAP_MASTER))
        return FALSE;

    for (i = 0; i < ARRAY_COUNT(sLegacyCapFlags); i++)
    {
        if (!FlagGet(sLegacyCapFlags[i]))
            return FALSE;
    }
    return TRUE;
}

bool8 IsNuzlockeEnabled(void)
{
    return (GetModeSave()->flags & GM_NUZLOCKE) != 0;
}

bool8 IsNuzlockeActive(void)
{
    struct GameModeSave *mode = GetModeSave();

    if (!IsNuzlockeEnabled())
        return FALSE;
    if (mode->flags2 & GM_NUZLOCKE_ACTIVE)
        return TRUE;

    // The rules begin the moment the player owns Poke Balls.
    if (HasAtLeastOnePokeBall())
    {
        mode->flags2 |= GM_NUZLOCKE_ACTIVE;
        return TRUE;
    }
    return FALSE;
}

bool8 AreLevelCapsEnabled(void)
{
    struct GameModeSave *mode = GetModeSave();

    if (IsNuzlockeEnabled())
        return TRUE;
    if (mode->flags2 & GM_INITIALIZED)
        return (mode->flags & GM_LEVEL_CAPS) != 0;
    if (LegacyChoseNoCaps())
        return FALSE;
    return TRUE;
}

bool8 AreScaledLevelsEnabled(void)
{
    return (GetModeSave()->flags & GM_SCALED) != 0;
}

void GameMode_BeginNewGameOptions(void)
{
    sNewGameOptions = TRUE;
}

void GameMode_CancelNewGameOptions(void)
{
    sNewGameOptions = FALSE;
}

bool8 GameMode_IsNewGameOptions(void)
{
    return sNewGameOptions;
}

void GameMode_ApplyNewGame(void)
{
    struct GameModeSave *mode = GetModeSave();

    if (!(mode->flags2 & GM_INITIALIZED))
    {
        mode->flags = GM_LEVEL_CAPS;
        mode->flags2 = GM_INITIALIZED | GM_WANDERERS;
        mode->seed = Random();
        if (mode->seed == 0)
            mode->seed = 1;
    }

    memset(GetRouteBits(), 0, NUZLOCKE_ROUTE_BYTES);
    if (mode->flags & GM_NUZLOCKE)
    {
        mode->flags |= GM_LEVEL_CAPS;
        mode->flags2 &= ~GM_WANDERERS;
        NameGraveyardBox();
    }
    SyncWanderers();
    VarSet(VAR_LEVEL_CAP, 1);
    sNewGameOptions = FALSE;
}

void GameModeOptions_Load(void)
{
    struct GameModeSave *mode = GetModeSave();

    if (sNewGameOptions)
    {
        sEdit.levelCaps = TRUE;
        sEdit.scaled = FALSE;
        sEdit.wanderers = TRUE;
        sEdit.nuzlocke = FALSE;
        sEdit.whiteoutDelete = FALSE;
        sEdit.randTrainers = FALSE;
        sEdit.randWild = FALSE;
        sEdit.randStatic = FALSE;
        sEdit.randItems = FALSE;
        return;
    }

    sEdit.nuzlocke = (mode->flags & GM_NUZLOCKE) != 0;
    sEdit.levelCaps = AreLevelCapsEnabled();
    sEdit.scaled = (mode->flags & GM_SCALED) != 0;
    sEdit.whiteoutDelete = (mode->flags & GM_WHITEOUT_DELETE) != 0;
    sEdit.randTrainers = (mode->flags & GM_RAND_TRAINERS) != 0;
    sEdit.randWild = (mode->flags & GM_RAND_WILD) != 0;
    sEdit.randStatic = (mode->flags & GM_RAND_STATIC) != 0;
    sEdit.randItems = (mode->flags & GM_RAND_ITEMS) != 0;
    sEdit.wanderers = !FlagGet(FLAG_SYS_ROUTE_WILD_WANDERERS_DISABLED);
    if (sEdit.nuzlocke)
    {
        sEdit.levelCaps = TRUE;
        sEdit.wanderers = FALSE;
    }
}

void GameModeOptions_Commit(void)
{
    struct GameModeSave *mode = GetModeSave();
    u8 active = mode->flags2 & GM_NUZLOCKE_ACTIVE;

    if (sEdit.nuzlocke)
    {
        sEdit.levelCaps = TRUE;
        sEdit.wanderers = FALSE;
    }

    mode->flags = 0;
    if (sEdit.levelCaps)
        mode->flags |= GM_LEVEL_CAPS;
    if (sEdit.scaled)
        mode->flags |= GM_SCALED;
    if (sEdit.nuzlocke)
        mode->flags |= GM_NUZLOCKE;
    if (sEdit.whiteoutDelete && sEdit.nuzlocke)
        mode->flags |= GM_WHITEOUT_DELETE;
    if (sEdit.randTrainers)
        mode->flags |= GM_RAND_TRAINERS;
    if (sEdit.randWild)
        mode->flags |= GM_RAND_WILD;
    if (sEdit.randStatic)
        mode->flags |= GM_RAND_STATIC;
    if (sEdit.randItems)
        mode->flags |= GM_RAND_ITEMS;

    mode->flags2 = GM_INITIALIZED | active;
    if (sEdit.wanderers && !sEdit.nuzlocke)
        mode->flags2 |= GM_WANDERERS;
    if (mode->seed == 0)
    {
        mode->seed = Random();
        if (mode->seed == 0)
            mode->seed = 1;
    }

    SyncWanderers();
    if (sEdit.nuzlocke)
        NameGraveyardBox();
    sNewGameOptions = FALSE;
}

u8 GameModeOptions_PageCount(void)
{
    return sNewGameOptions ? 3 : 2;
}

u8 GameModeOptions_ItemCount(u8 page)
{
    if (page == 1)
        return 4;
    if (page == 2)
        return 6;
    return 0;
}

const u8 *GameModeOptions_ItemName(u8 page, u8 index)
{
    if (page == 2)
        return sPage2Names[index];
    return sPage1Names[index];
}

const u8 *GameModeOptions_ChoiceOn(u8 page, u8 index)
{
    if (page == 2 && index == 1)
        return sText_Delete;
    return gText_BattleSceneOn;
}

const u8 *GameModeOptions_ChoiceOff(u8 page, u8 index)
{
    if (page == 2 && index == 1)
        return sText_Party;
    return gText_BattleSceneOff;
}

bool8 GameModeOptions_Get(u8 page, u8 index)
{
    if (page == 1)
    {
        switch (index)
        {
        case 0: return sEdit.levelCaps;
        case 1: return sEdit.scaled;
        case 2: return sEdit.wanderers;
        case 3: return !gSaveBlock2Ptr->optionsColorVariantsOff;
        }
    }
    else if (page == 2)
    {
        switch (index)
        {
        case 0: return sEdit.nuzlocke;
        case 1: return sEdit.whiteoutDelete;
        case 2: return sEdit.randTrainers;
        case 3: return sEdit.randWild;
        case 4: return sEdit.randStatic;
        case 5: return sEdit.randItems;
        }
    }
    return FALSE;
}

bool8 GameModeOptions_IsLocked(u8 page, u8 index)
{
    if (!sNewGameOptions && page == 2)
        return TRUE;
    if (sEdit.nuzlocke && page == 1 && (index == 0 || index == 2))
        return TRUE;
    if (page == 2 && index == 1 && !sEdit.nuzlocke)
        return TRUE;
    if (!sNewGameOptions && page == 2)
        return TRUE;
    return FALSE;
}

void GameModeOptions_Toggle(u8 page, u8 index)
{
    if (GameModeOptions_IsLocked(page, index))
        return;

    if (page == 1)
    {
        switch (index)
        {
        case 0: sEdit.levelCaps ^= 1; break;
        case 1: sEdit.scaled ^= 1; break;
        case 2: sEdit.wanderers ^= 1; break;
        case 3: gSaveBlock2Ptr->optionsColorVariantsOff ^= 1; break;
        }
    }
    else if (page == 2)
    {
        switch (index)
        {
        case 0:
            sEdit.nuzlocke ^= 1;
            if (sEdit.nuzlocke)
            {
                sEdit.levelCaps = TRUE;
                sEdit.wanderers = FALSE;
            }
            break;
        case 1: sEdit.whiteoutDelete ^= 1; break;
        case 2: sEdit.randTrainers ^= 1; break;
        case 3: sEdit.randWild ^= 1; break;
        case 4: sEdit.randStatic ^= 1; break;
        case 5: sEdit.randItems ^= 1; break;
        }
    }
}

static bool8 IsUsableSpecies(u16 species)
{
    const struct SpeciesInfo *info;

    if (species == SPECIES_NONE || species == SPECIES_EGG || species >= NUM_SPECIES)
        return FALSE;

    info = &gSpeciesInfo[species];
    if (info->baseHP == 0)
        return FALSE;
    if (info->isMegaEvolution || info->isPrimalReversion || info->isUltraBurst
     || info->isGigantamax || info->isTeraForm || info->isTotem)
        return FALSE;

    // Regional variants are kept; every other alternate form falls back to its base species.
    if (info->isAlolanForm || info->isGalarianForm || info->isHisuianForm || info->isPaldeanForm)
        return TRUE;
    return species == GET_BASE_SPECIES_ID(species);
}

static u16 RandomSpecies(u32 salt)
{
    u32 i;

    for (i = 0; i < 64; i++)
    {
        u16 species = (GameRand(salt + i) % (NUM_SPECIES - 1)) + 1;
        if (IsUsableSpecies(species))
            return species;
    }
    return SPECIES_RATTATA;
}

static u8 HighestPlayerLevel(void)
{
    u32 i;
    u8 highest = 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u8 level;

        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE)
            continue;
        if (GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            continue;
        level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        if (level > highest)
            highest = level;
    }
    return highest;
}

void GameMode_AdjustEnemyMon(struct Pokemon *mon, u32 salt)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES);
    u8 level = GetMonData(mon, MON_DATA_LEVEL);
    bool8 changed = FALSE;

    if (GetModeSave()->flags & GM_RAND_TRAINERS)
    {
        species = RandomSpecies(salt);
        changed = TRUE;
    }
    if (AreScaledLevelsEnabled() && CalculatePlayerPartyCount() != 0)
    {
        u8 highest = HighestPlayerLevel();
        if (highest != 0 && highest != level)
        {
            level = highest;
            changed = TRUE;
        }
    }
    if (changed)
        CreateMon(mon, species, level, 0, FALSE, 0, OT_ID_RANDOM_NO_SHINY, 0);
}

u16 GameMode_RandomWildSpecies(u16 species, u32 salt)
{
    if (!(GetModeSave()->flags & GM_RAND_WILD))
        return species;
    return RandomSpecies(salt ^ species);
}

u16 GameMode_RandomStaticSpecies(u16 species)
{
    if (!(GetModeSave()->flags & GM_RAND_STATIC) || species == SPECIES_NONE)
        return species;
    return RandomSpecies(0x510000u ^ species);
}

static bool8 CanRandomizeItem(u16 itemId)
{
    u8 pocket;

    if (itemId == ITEM_NONE || itemId >= ITEMS_COUNT)
        return FALSE;
    pocket = ItemId_GetPocket(itemId);
    if (pocket != POCKET_ITEMS && pocket != POCKET_POKE_BALLS && pocket != POCKET_BERRIES)
        return FALSE;
    if (gItemsInfo[itemId].price == 0)
        return FALSE;
    return TRUE;
}

u16 GameMode_RandomizeFoundItem(u16 itemId, u32 salt)
{
    if (!(GetModeSave()->flags & GM_RAND_ITEMS) || !CanRandomizeItem(itemId))
        return itemId;
    return sFloorItemPool[GameRand(0x170000u ^ salt ^ itemId) % ARRAY_COUNT(sFloorItemPool)];
}

static bool8 RouteIsClosed(u16 mapsec)
{
    if (mapsec >= NUZLOCKE_ROUTE_BYTES * 8)
        return FALSE;
    return (GetRouteBits()[mapsec / 8] >> (mapsec % 8)) & 1;
}

static void RouteClose(u16 mapsec)
{
    if (mapsec >= NUZLOCKE_ROUTE_BYTES * 8)
        return;
    GetRouteBits()[mapsec / 8] |= 1 << (mapsec % 8);
}

static u16 FamilyRoot(u16 species)
{
    u32 guard;

    species = SanitizeSpeciesId(species);
    for (guard = 0; guard < 16; guard++)
    {
        u16 pre = GetSpeciesPreEvolution(species);
        if (pre == SPECIES_NONE || pre == species)
            break;
        species = pre;
    }
    return species;
}

static bool8 BoxMonOwned(struct BoxPokemon *boxMon, u16 root)
{
    u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    if (species == SPECIES_NONE || GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL))
        return FALSE;
    return FamilyRoot(species) == root;
}

static bool8 PlayerOwnsFamily(u16 species)
{
    u16 root = FamilyRoot(species);
    u32 box, pos, i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 partySpecies = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (partySpecies == SPECIES_NONE || GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            continue;
        if (FamilyRoot(partySpecies) == root)
            return TRUE;
    }

    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            if (BoxMonOwned(GetBoxedMonPtr(box, pos), root))
                return TRUE;
        }
    }
    return FALSE;
}

void GameMode_NoteWildEncounter(u16 species)
{
    u16 mapsec = gMapHeader.regionMapSectionId;

    sBlockCatch = FALSE;
    sPendingClose = FALSE;
    sPendingMapSec = MAPSEC_NONE;
    if (!IsNuzlockeActive() || mapsec == MAPSEC_NONE)
        return;
    if (RouteIsClosed(mapsec))
    {
        sBlockCatch = TRUE;
        return;
    }
    if (PlayerOwnsFamily(species))
        return;

    sPendingClose = TRUE;
    sPendingMapSec = mapsec;
}

void GameMode_FinishWildEncounter(void)
{
    if (sPendingClose)
        RouteClose(sPendingMapSec);
    sPendingClose = FALSE;
    sBlockCatch = FALSE;
    sPendingMapSec = MAPSEC_NONE;
}

bool8 Nuzlocke_ShouldBlockCatch(void)
{
    return IsNuzlockeActive() && sBlockCatch;
}

static bool8 DepositInGraveyard(struct Pokemon *mon)
{
    u32 pos;

    for (pos = 0; pos < IN_BOX_COUNT; pos++)
    {
        struct BoxPokemon *slot = GetBoxedMonPtr(NUZLOCKE_GRAVEYARD_BOX, pos);
        if (GetBoxMonData(slot, MON_DATA_SPECIES, NULL) == SPECIES_NONE)
        {
            CopyMon(slot, &mon->box, sizeof(mon->box));
            return TRUE;
        }
    }
    return FALSE;
}

static bool8 MonIsLiving(struct Pokemon *mon)
{
    return GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE
        && !GetMonData(mon, MON_DATA_IS_EGG)
        && GetMonData(mon, MON_DATA_HP) != 0;
}

static bool8 StorageHasLivingMon(void)
{
    u32 box, pos;

    for (box = 0; box < NUZLOCKE_GRAVEYARD_BOX; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            struct BoxPokemon *slot = GetBoxedMonPtr(box, pos);
            if (GetBoxMonData(slot, MON_DATA_SPECIES, NULL) != SPECIES_NONE
             && !GetBoxMonData(slot, MON_DATA_IS_EGG, NULL))
                return TRUE;
        }
    }
    return FALSE;
}

void Nuzlocke_BuryFaintedParty(void)
{
    u32 i;
    bool8 buried = FALSE;

    if (!IsNuzlockeActive())
        return;

    NameGraveyardBox();
    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];

        if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE)
            continue;
        if (GetMonData(mon, MON_DATA_IS_EGG))
            continue;
        if (GetMonData(mon, MON_DATA_HP) != 0)
            continue;
        DepositInGraveyard(mon);
        ZeroMonData(mon);
        buried = TRUE;
    }
    if (buried)
    {
        CompactPartySlots();
        CalculatePlayerPartyCount();
    }
}

bool8 Nuzlocke_OnWhiteOut(void)
{
    bool8 partyLiving = FALSE;
    bool8 noSurvivors;
    u32 i;

    // Save deletion is impossible unless nuzlocke was chosen and the rules are active.
    if (!IsNuzlockeEnabled() || !IsNuzlockeActive())
        return FALSE;

    Nuzlocke_BuryFaintedParty();
    GameMode_FinishWildEncounter();
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (MonIsLiving(&gPlayerParty[i]))
            partyLiving = TRUE;
    }
    noSurvivors = !partyLiving && !StorageHasLivingMon();

    // DELETE mode always ends the run. New Party ends it only when nothing living remains.
    if ((GetModeSave()->flags & GM_WHITEOUT_DELETE) || noSurvivors)
    {
        if (!IsNuzlockeEnabled() || !IsNuzlockeActive())
            return FALSE;
        ClearSaveData();
        DoSoftReset();
        return TRUE;
    }
    return FALSE;
}

bool8 Nuzlocke_IsGraveyardBox(u8 boxId)
{
    return IsNuzlockeActive() && boxId == NUZLOCKE_GRAVEYARD_BOX;
}

u16 Special_AreLevelCapsEnabled(void)
{
    return AreLevelCapsEnabled();
}

u16 Special_ActivateNuzlockeRules(void)
{
    struct GameModeSave *mode = GetModeSave();

    if (!IsNuzlockeEnabled())
        return FALSE;
    mode->flags |= GM_LEVEL_CAPS;
    mode->flags2 |= GM_NUZLOCKE_ACTIVE;
    mode->flags2 &= ~GM_WANDERERS;
    SyncWanderers();
    NameGraveyardBox();
    return TRUE;
}

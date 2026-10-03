#include "global.h"
#include "bg.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_effect.h"
#include "fieldmap.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "mystery_dungeon.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"

#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

// A Pokemon Mystery Dungeon style minigame played on the Tester map.
//
// The player becomes a Squirtle and shares the floor with up to MD_MAX_ENEMIES
// wild Pokemon. Everything is strictly turn based: the player moves or attacks,
// then every living enemy takes one action. An NPC stands in for the stairs;
// walking into it clears the floor.

#define MD_MAX_ENEMIES          4
#define MD_ENEMY_LOCALID_BASE   60 // Tester only uses localId 1, so 60+ is free.
#define MD_STAIRS_LOCALID       (MD_ENEMY_LOCALID_BASE + MD_MAX_ENEMIES)

#define MD_STAIRS_GFX           OBJ_EVENT_GFX_GENTLEMAN
#define MD_START_NPC_LOCALID    1 // Tester Pikachu that starts the run.

// Every tile of Tester is elevation 0, and elevation 0 is compatible with any
// other elevation, so spawns always collide with the player no matter what
// elevation they walked in carrying.
#define MD_ELEVATION            0

// Where enemies and the stairs are placed relative to the player at floor start.
#define MD_SPAWN_RADIUS         9
#define MD_SPAWN_MIN_DIST       4
#define MD_STAIRS_RADIUS        12
#define MD_STAIRS_MIN_DIST      8
#define MD_PLACEMENT_TRIES      48

// A new enemy wanders in this many player turns after the cap drops below max.
#define MD_RESPAWN_TURNS        8

// One HP returns after this many tiles the player actually walks.
#define MD_REGEN_STEPS          4

#define MD_MESSAGE_FRAMES       70
#define MD_INTRO_FRAMES         110

// Everyone on the floor is a mon overworld sprite, and those use
// sAnimTable_Following, where the 'faster' slots hold the enter-pokeball
// animation instead of a walk cycle. 'Fast' is the quickest step that still
// animates: 8 frames a tile, two of them showing the stepping frame.
#define MD_WALK_ACTION(dir)     (MOVEMENT_ACTION_WALK_FAST_DOWN + ((dir) - 1))

// --- Types ------------------------------------------------------------------
// Only the types this minigame actually uses. Mystery Dungeon multipliers are
// much gentler than the main series: super effective is x1.4 and not very
// effective is x0.7, stored here as percentages.

enum {
    MD_TYPE_NORMAL,
    MD_TYPE_WATER,
    MD_TYPE_ROCK,
    MD_TYPE_GROUND,
    MD_TYPE_FIRE,
    MD_TYPE_GRASS,
    MD_TYPE_NONE,
    MD_TYPE_COUNT,
};

static const u8 sMdTypeChart[MD_TYPE_NONE][MD_TYPE_NONE] =
{
//  attacker          vs NORMAL  WATER  ROCK  GROUND  FIRE  GRASS
    [MD_TYPE_NORMAL] =  { 100,    100,    70,   100,   100,   100 },
    [MD_TYPE_WATER]  =  { 100,     70,   140,   140,   140,    70 },
    [MD_TYPE_ROCK]   =  { 100,    100,   100,    70,   140,   100 },
    [MD_TYPE_GROUND] =  { 100,    100,   140,   100,   140,    70 },
    [MD_TYPE_FIRE]   =  { 100,     70,    70,   100,    70,   140 },
    [MD_TYPE_GRASS]  =  { 100,    140,   140,   140,    70,    70 },
};

// --- Moves ------------------------------------------------------------------

enum {
    MD_MOVE_ATTACK,
    MD_MOVE_BUBBLE,
    MD_MOVE_TACKLE,
    MD_MOVE_ROCK_THROW,
    MD_MOVE_SCRATCH,
    MD_MOVE_EMBER,
    MD_MOVE_VINE_WHIP,
    MD_MOVE_COUNT,
};

struct MdMove
{
    const u8 *name;
    u8 type;
    u8 power;
    u8 maxPp;    // 0 means unlimited.
    u8 range;    // 1 hits the tile in front, higher values fire in a straight line.
    bool8 isSpecial;
};

static const struct MdMove sMdMoves[MD_MOVE_COUNT] =
{
    [MD_MOVE_ATTACK]     = { .name = COMPOUND_STRING("Attack"),     .type = MD_TYPE_NORMAL, .power =  2, .maxPp =  0, .range = 1, .isSpecial = FALSE },
    [MD_MOVE_BUBBLE]     = { .name = COMPOUND_STRING("Bubble"),     .type = MD_TYPE_WATER,  .power =  4, .maxPp = 18, .range = 4, .isSpecial = TRUE  },
    [MD_MOVE_TACKLE]     = { .name = COMPOUND_STRING("Tackle"),     .type = MD_TYPE_NORMAL, .power =  4, .maxPp = 22, .range = 1, .isSpecial = FALSE },
    [MD_MOVE_ROCK_THROW] = { .name = COMPOUND_STRING("Rock Throw"), .type = MD_TYPE_ROCK,   .power =  5, .maxPp = 16, .range = 1, .isSpecial = FALSE },
    [MD_MOVE_SCRATCH]    = { .name = COMPOUND_STRING("Scratch"),    .type = MD_TYPE_NORMAL, .power =  4, .maxPp = 22, .range = 1, .isSpecial = FALSE },
    [MD_MOVE_EMBER]      = { .name = COMPOUND_STRING("Ember"),      .type = MD_TYPE_FIRE,   .power =  5, .maxPp = 18, .range = 1, .isSpecial = TRUE  },
    [MD_MOVE_VINE_WHIP]  = { .name = COMPOUND_STRING("Vine Whip"),  .type = MD_TYPE_GRASS,  .power =  4, .maxPp = 20, .range = 2, .isSpecial = FALSE },
};

#define MD_PLAYER_MOVE_COUNT 3
#define MD_ENEMY_MOVE_COUNT  2

static const u8 sMdEnemyMoves[MD_ENEMY_MOVE_COUNT] = { MD_MOVE_TACKLE, MD_MOVE_ROCK_THROW };

// --- Species ----------------------------------------------------------------

struct MdSpeciesStats
{
    u16 species;
    const u8 *name;
    u8 type1;
    u8 type2;
    u8 level;
    u16 maxHp;
    u16 atk;
    u16 def;
    u16 spAtk;
    u16 spDef;
};

// The guide's menu returns one of these, so the order has to match
// MultichoiceList_MysteryDungeonStarter.
enum {
    MD_STARTER_SQUIRTLE,
    MD_STARTER_BULBASAUR,
    MD_STARTER_CHARMANDER,
    MD_STARTER_COUNT,
};

static const struct MdSpeciesStats sMdStarterStats[MD_STARTER_COUNT] =
{
    [MD_STARTER_SQUIRTLE] =
    {
        .species = SPECIES_SQUIRTLE,
        .name = COMPOUND_STRING("Squirtle"),
        .type1 = MD_TYPE_WATER, .type2 = MD_TYPE_NONE,
        .level = 10, .maxHp = 44, .atk = 11, .def = 10, .spAtk = 13, .spDef = 10,
    },
    [MD_STARTER_BULBASAUR] =
    {
        .species = SPECIES_BULBASAUR,
        .name = COMPOUND_STRING("Bulbasaur"),
        .type1 = MD_TYPE_GRASS, .type2 = MD_TYPE_NONE,
        .level = 10, .maxHp = 44, .atk = 11, .def = 11, .spAtk = 13, .spDef = 13,
    },
    [MD_STARTER_CHARMANDER] =
    {
        .species = SPECIES_CHARMANDER,
        .name = COMPOUND_STRING("Charmander"),
        .type1 = MD_TYPE_FIRE, .type2 = MD_TYPE_NONE,
        .level = 10, .maxHp = 41, .atk = 12, .def = 10, .spAtk = 13, .spDef = 11,
    },
};

// Slot 0 is the free basic attack on A, slot 1 is the signature move on R+A,
// slot 2 is the physical one on R+B. The control hints print these names, so
// the order here is the order the player reads along the bottom of the screen.
static const u8 sMdStarterMoves[MD_STARTER_COUNT][MD_PLAYER_MOVE_COUNT] =
{
    [MD_STARTER_SQUIRTLE]   = { MD_MOVE_ATTACK, MD_MOVE_BUBBLE,    MD_MOVE_TACKLE  },
    [MD_STARTER_BULBASAUR]  = { MD_MOVE_ATTACK, MD_MOVE_VINE_WHIP, MD_MOVE_TACKLE  },
    [MD_STARTER_CHARMANDER] = { MD_MOVE_ATTACK, MD_MOVE_EMBER,     MD_MOVE_SCRATCH },
};

enum {
    MD_ENEMY_GEODUDE,
    MD_ENEMY_DIGLETT,
    MD_ENEMY_RHYHORN,
    MD_ENEMY_SPECIES_COUNT,
};

static const struct MdSpeciesStats sMdEnemyStats[MD_ENEMY_SPECIES_COUNT] =
{
    [MD_ENEMY_GEODUDE] =
    {
        .species = SPECIES_GEODUDE,
        .name = COMPOUND_STRING("Geodude"),
        .type1 = MD_TYPE_ROCK, .type2 = MD_TYPE_GROUND,
        .level = 10, .maxHp = 38, .atk = 12, .def = 16, .spAtk = 7, .spDef = 7,
    },
    [MD_ENEMY_DIGLETT] =
    {
        .species = SPECIES_DIGLETT,
        .name = COMPOUND_STRING("Diglett"),
        .type1 = MD_TYPE_GROUND, .type2 = MD_TYPE_NONE,
        .level = 8, .maxHp = 28, .atk = 10, .def = 8, .spAtk = 7, .spDef = 7,
    },
    [MD_ENEMY_RHYHORN] =
    {
        .species = SPECIES_RHYHORN,
        .name = COMPOUND_STRING("Rhyhorn"),
        .type1 = MD_TYPE_GROUND, .type2 = MD_TYPE_ROCK,
        .level = 12, .maxHp = 52, .atk = 15, .def = 14, .spAtk = 7, .spDef = 8,
    },
};

// --- Session ----------------------------------------------------------------

enum {
    MD_STATE_INPUT,
    MD_STATE_PLAYER_MOVING,
    MD_STATE_MESSAGE,
    MD_STATE_ENEMY_TURN,
    MD_STATE_ENEMY_MOVING,
};

enum {
    MD_FADE_NONE,
    MD_FADE_OUT_START,
    MD_FADE_SHOW_START,
    MD_FADE_IN_START,
    MD_FADE_OUT_END,
    MD_FADE_SHOW_END,
    MD_FADE_IN_END,
};

struct MdEnemy
{
    bool8 active;
    u8 statsIdx;
    u8 localId;
    u8 objEventId;
    s16 hp;
    u8 pp[MD_ENEMY_MOVE_COUNT];
};

struct MdSession
{
    bool8 active;
    bool8 ending;   // Set once Md_End runs so the UI is not rebuilt on the way out.
    u8 taskId;
    u8 state;
    u8 nextState;

    u8 starter;
    s16 playerHp;
    u8 playerLevel;
    u16 playerExp;
    u16 expToNext;
    u16 playerMaxHp;
    u8 stepsUntilRegen;
    u8 playerPp[MD_PLAYER_MOVE_COUNT];

    struct MdEnemy enemies[MD_MAX_ENEMIES];
    u8 enemyIndex;      // Which enemy is acting during MD_STATE_ENEMY_TURN.
    u8 enemyActed;      // Bit per enemy that already attacked this round.
    u8 focusEnemy;      // Enemy shown in the status window.
    s16 uiPlayerHp;
    s16 uiFocusHp;
    u8 respawnTimer;

    u16 messageTimer;
    u8 messageSerial;   // Bumped per message so the UI knows to redraw.
    u32 uiSignature;
    bool8 pendingEnd;
    bool8 reachedStairs;
    bool8 messageHold;  // Stays up until a button, instead of timing out.
    u8 levelsGained;    // Follows the KO text with the level-up box.

    bool8 stairsSpawned;
    bool8 showControls;
    u16 objWasVisible;

    // Windows.
    u8 playerWindowId;
    u8 enemyWindowId;
    u8 messageWindowId;
    u8 fadeTextWindowId;

    // Restore state.
    u16 originalPlayerGraphicsId;
    bool8 followerWasHidden;
    u8 fadeState;
    u16 fadeTextTimer;
};

static EWRAM_DATA struct MdSession sMd = {0};

#define MD_WINDOW_BG 0

// BG0 draws its tiles from char base 2 (VRAM 0x8000), and the four overworld
// tilemaps start at VRAM 0xE000, so only tiles 0x000-0x2FF belong to BG0.
// A window whose tiles run past 0x2FF writes straight into the map's tilemap
// and the floor turns into repeating garbage.
//
// Reserved by the overworld and reloaded only on map load, so leave alone:
//   0x200-0x20D  message box frame
//   0x214-0x222  standard window border (the HUD frames point at these)
// Everything else is owned by windows that redraw their own tiles on use.
#define MD_MESSAGE_WIN_BASE  0x001
#define MD_PLAYER_WIN_BASE   (MD_MESSAGE_WIN_BASE + 28 * 6)
#define MD_ENEMY_WIN_BASE    (MD_PLAYER_WIN_BASE + 11 * 3)

// Shown only while the screen is black, so it can share space with the help
// window that the script opens before the run.
#define MD_FADE_WIN_BASE     0x223

// The status readouts have no frame and a transparent background, so they
// float over the floor instead of boxing off the corners.
static const struct WindowTemplate sMdPlayerWindowTemplate =
{
    .bg = MD_WINDOW_BG, .tilemapLeft = 0, .tilemapTop = 0, .width = 11, .height = 3,
    .paletteNum = STD_WINDOW_PALETTE_NUM, .baseBlock = MD_PLAYER_WIN_BASE,
};

static const struct WindowTemplate sMdEnemyWindowTemplate =
{
    .bg = MD_WINDOW_BG, .tilemapLeft = 19, .tilemapTop = 0, .width = 11, .height = 3,
    .paletteNum = STD_WINDOW_PALETTE_NUM, .baseBlock = MD_ENEMY_WIN_BASE,
};

static const struct WindowTemplate sMdMessageWindowTemplate =
{
    .bg = MD_WINDOW_BG, .tilemapLeft = 1, .tilemapTop = 13, .width = 28, .height = 6,
    .paletteNum = STD_WINDOW_PALETTE_NUM, .baseBlock = MD_MESSAGE_WIN_BASE,
};

static const u8 sMdTextColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
// Unboxed text sits on the map, so it needs a light fill and a dark edge.
static const u8 sMdStatusTextColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sMdFadeTextColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};

static EWRAM_DATA u8 sMdMessageBuffer[200] = {0};

static void Task_MysteryDungeon(u8 taskId);
static void Task_MysteryDungeonFade(u8 taskId);
static void Md_End(bool8 reachedStairs);
static void Md_DrawUi(void);
static void Md_DestroyUi(void);
static void Md_DestroyWindow(u8 *windowId);
static void Md_SetMessage(u8 nextState);
static void Md_PlayMoveFx(u8 moveId, s16 x, s16 y);
static void Md_SpawnStairs(void);
static bool8 Md_SpawnEnemy(void);
static void Md_DespawnAll(void);

static const struct MdSpeciesStats *Md_BaseStats(void)
{
    return &sMdStarterStats[sMd.starter];
}

static u8 Md_PlayerMoveId(u8 slot)
{
    return sMdStarterMoves[sMd.starter][slot];
}

// --- Small math helpers -----------------------------------------------------

// ln(x) in Q12 fixed point, for x >= 1. log2 comes from the integer exponent
// plus a linear interpolation across the mantissa, then scaled by ln(2).
static u32 Md_NaturalLogQ12(u32 x)
{
    static const u16 sLog2FracQ12[17] =
    {
        0, 358, 696, 1016, 1319, 1607, 1882, 2145, 2396,
        2638, 2869, 3092, 3307, 3514, 3715, 3909, 4096,
    };
    u32 exponent = 0;
    u32 pow2 = 1;
    u32 scaled, index, rem, fracQ12, log2Q12;

    if (x < 1)
        x = 1;

    while ((pow2 << 1) <= x)
    {
        pow2 <<= 1;
        exponent++;
    }

    // mantissa = x / pow2, in [1, 2). Work in sixteenths to index the table.
    scaled = ((x - pow2) * 16) / pow2; // 0..15
    rem = ((x - pow2) * 16) % pow2;
    index = scaled;
    if (index > 15)
        index = 15;

    fracQ12 = sLog2FracQ12[index];
    fracQ12 += ((sLog2FracQ12[index + 1] - sLog2FracQ12[index]) * rem) / pow2;

    log2Q12 = (exponent << 12) + fracQ12;

    // ln(x) = log2(x) * ln(2); ln(2) in Q12 is 2839.
    return (log2Q12 * 2839) >> 12;
}

static u8 Md_TypeMultiplierPercent(u8 moveType, u8 defType1, u8 defType2)
{
    u32 mult;

    if (moveType >= MD_TYPE_NONE)
        return 100;

    mult = 100;
    if (defType1 < MD_TYPE_NONE)
        mult = sMdTypeChart[moveType][defType1];
    if (defType2 < MD_TYPE_NONE)
        mult = (mult * sMdTypeChart[moveType][defType2]) / 100;

    return mult;
}

// Explorers of Sky / Time / Darkness:
//   ((A + P) * 39168 / 65536) - (D / 2) + 50 * ln(((A - D) / 8 + L + 50) * 10) - 311
// Wild attackers then take the 256/340 team-member penalty. After that: STAB,
// the gentler Mystery Dungeon type chart, and a +/-12.5% roll.
static s32 Md_CalcDamage(const struct MdSpeciesStats *attacker, const struct MdSpeciesStats *defender, const struct MdMove *move, bool8 wildAttacker)
{
    s32 atk = move->isSpecial ? attacker->spAtk : attacker->atk;
    s32 def = move->isSpecial ? defender->spDef : defender->def;
    s32 logArg = ((atk - def) / 8 + attacker->level + 50) * 10;
    s32 damage;
    u32 typePercent;

    if (logArg < 1)
        logArg = 1;
    if (logArg > 4095)
        logArg = 4095;

    damage = ((atk + move->power) * 39168) / 65536;
    damage -= def / 2;
    damage += (s32)((50 * Md_NaturalLogQ12((u32)logArg)) >> 12);
    damage -= 311;

    if (wildAttacker)
        damage = (damage * 256) / 340;

    if (damage < 1)
        damage = 1;

    if (move->type == attacker->type1 || move->type == attacker->type2)
        damage = (damage * 3) / 2;

    typePercent = Md_TypeMultiplierPercent(move->type, defender->type1, defender->type2);
    if (typePercent == 0)
        return 0;

    damage = (damage * (s32)typePercent) / 100;

    // Random roll between 87.5% and 112.5%.
    damage = (damage * (875 + (Random() % 251))) / 1000;

    if (damage < 1)
        damage = 1;

    return damage;
}

// --- Grid helpers -----------------------------------------------------------

static struct ObjectEvent *Md_PlayerObj(void)
{
    return &gObjectEvents[gPlayerAvatar.objectEventId];
}

static void Md_DirToDelta(u8 dir, s16 *dx, s16 *dy)
{
    *dx = 0;
    *dy = 0;
    switch (dir)
    {
    case DIR_SOUTH: *dy = 1; break;
    case DIR_NORTH: *dy = -1; break;
    case DIR_WEST:  *dx = -1; break;
    case DIR_EAST:  *dx = 1; break;
    }
}

static bool8 Md_IsTileFree(s16 x, s16 y)
{
    return GetCollisionAtCoords(Md_PlayerObj(), x, y, DIR_SOUTH) == 0;
}

// Walls only. Bodies move every round, so leaving them out keeps the chase
// map stable; whoever is standing where is checked again at step time.
static bool8 Md_IsWallFree(s16 x, s16 y)
{
    return MapGridGetCollisionAt(x, y) == 0 && GetMapBorderIdAt(x, y) != CONNECTION_INVALID;
}

static s8 Md_EnemyAt(s16 x, s16 y)
{
    u8 i;

    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        struct MdEnemy *enemy = &sMd.enemies[i];
        if (!enemy->active || enemy->objEventId >= OBJECT_EVENTS_COUNT)
            continue;
        if (!gObjectEvents[enemy->objEventId].active)
            continue;
        if (gObjectEvents[enemy->objEventId].currentCoords.x == x
         && gObjectEvents[enemy->objEventId].currentCoords.y == y)
            return i;
    }

    return -1;
}

static bool8 Md_IsStartNpc(struct ObjectEvent *obj)
{
    return obj->localId == MD_START_NPC_LOCALID
        && obj->mapNum == gSaveBlock1Ptr->location.mapNum
        && obj->mapGroup == gSaveBlock1Ptr->location.mapGroup;
}

static void Md_HideStartNpc(void)
{
    u8 objId = GetObjectEventIdByLocalIdAndMap(MD_START_NPC_LOCALID,
                                               gSaveBlock1Ptr->location.mapNum,
                                               gSaveBlock1Ptr->location.mapGroup);
    if (objId >= OBJECT_EVENTS_COUNT)
        return;

    gObjectEvents[objId].invisible = TRUE;
    if (gObjectEvents[objId].spriteId < MAX_SPRITES)
        gSprites[gObjectEvents[objId].spriteId].invisible = TRUE;
}

static void Md_ShowStartNpc(void)
{
    u8 objId = GetObjectEventIdByLocalIdAndMap(MD_START_NPC_LOCALID,
                                               gSaveBlock1Ptr->location.mapNum,
                                               gSaveBlock1Ptr->location.mapGroup);
    if (objId >= OBJECT_EVENTS_COUNT)
        return;

    // Md_KeepFloorLoaded parked its spawn anchor on the player; it stands still,
    // so its current tile is where it started.
    gObjectEvents[objId].initialCoords = gObjectEvents[objId].currentCoords;

    gObjectEvents[objId].invisible = FALSE;
    if (gObjectEvents[objId].spriteId < MAX_SPRITES)
        gSprites[gObjectEvents[objId].spriteId].invisible = FALSE;
}

static bool8 Md_IsStairsAt(s16 x, s16 y)
{
    u8 objId;

    if (!sMd.stairsSpawned)
        return FALSE;

    objId = GetObjectEventIdByLocalIdAndMap(MD_STAIRS_LOCALID,
                                            gSaveBlock1Ptr->location.mapNum,
                                            gSaveBlock1Ptr->location.mapGroup);
    if (objId >= OBJECT_EVENTS_COUNT || !gObjectEvents[objId].active)
        return FALSE;

    return gObjectEvents[objId].currentCoords.x == x && gObjectEvents[objId].currentCoords.y == y;
}

static u8 Md_LivingEnemyCount(void)
{
    u8 i, count = 0;

    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        if (sMd.enemies[i].active)
            count++;
    }

    return count;
}

// Nearest living enemy, used for the status window.
static void Md_UpdateFocusEnemy(void)
{
    struct ObjectEvent *playerObj = Md_PlayerObj();
    u8 i;
    u16 best = 0xFFFF;
    u8 bestIdx = MD_MAX_ENEMIES;

    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        struct MdEnemy *enemy = &sMd.enemies[i];
        u16 dist;

        if (!enemy->active || enemy->objEventId >= OBJECT_EVENTS_COUNT)
            continue;

        dist = abs(gObjectEvents[enemy->objEventId].currentCoords.x - playerObj->currentCoords.x)
             + abs(gObjectEvents[enemy->objEventId].currentCoords.y - playerObj->currentCoords.y);
        if (dist < best)
        {
            best = dist;
            bestIdx = i;
        }
    }

    sMd.focusEnemy = bestIdx;
}

// --- Spawning ---------------------------------------------------------------

static bool8 Md_FindSpot(s16 radius, s16 minDist, s16 *outX, s16 *outY)
{
    struct ObjectEvent *playerObj = Md_PlayerObj();
    u8 tries;

    for (tries = 0; tries < MD_PLACEMENT_TRIES; tries++)
    {
        s16 dx = (Random() % (radius * 2 + 1)) - radius;
        s16 dy = (Random() % (radius * 2 + 1)) - radius;
        s16 x = playerObj->currentCoords.x + dx;
        s16 y = playerObj->currentCoords.y + dy;

        if (abs(dx) + abs(dy) < minDist)
            continue;
        if (!Md_IsTileFree(x, y))
            continue;

        *outX = x;
        *outY = y;
        return TRUE;
    }

    return FALSE;
}

static bool8 Md_SpawnEnemy(void)
{
    u8 slot, statsIdx, objId;
    s16 x, y;

    for (slot = 0; slot < MD_MAX_ENEMIES; slot++)
    {
        if (!sMd.enemies[slot].active)
            break;
    }
    if (slot >= MD_MAX_ENEMIES)
        return FALSE;

    if (!Md_FindSpot(MD_SPAWN_RADIUS, MD_SPAWN_MIN_DIST, &x, &y))
        return FALSE;

    statsIdx = Random() % MD_ENEMY_SPECIES_COUNT;

    objId = SpawnSpecialObjectEventParameterized(
        OBJ_EVENT_MON + sMdEnemyStats[statsIdx].species,
        MOVEMENT_TYPE_NONE,
        MD_ENEMY_LOCALID_BASE + slot,
        x, y, MD_ELEVATION);

    if (objId >= OBJECT_EVENTS_COUNT)
        return FALSE;

    sMd.enemies[slot].active = TRUE;
    sMd.enemies[slot].statsIdx = statsIdx;
    sMd.enemies[slot].localId = MD_ENEMY_LOCALID_BASE + slot;
    sMd.enemies[slot].objEventId = objId;
    sMd.enemies[slot].hp = sMdEnemyStats[statsIdx].maxHp;
    sMd.enemies[slot].pp[0] = sMdMoves[sMdEnemyMoves[0]].maxPp;
    sMd.enemies[slot].pp[1] = sMdMoves[sMdEnemyMoves[1]].maxPp;

    return TRUE;
}

static void Md_SpawnStairs(void)
{
    s16 x, y;
    u8 objId;

    // Without a guide the floor cannot be cleared, so settle for a closer tile
    // rather than leaving the player to wander.
    if (!Md_FindSpot(MD_STAIRS_RADIUS, MD_STAIRS_MIN_DIST, &x, &y)
     && !Md_FindSpot(MD_SPAWN_RADIUS, MD_SPAWN_MIN_DIST, &x, &y))
        return;

    objId = SpawnSpecialObjectEventParameterized(MD_STAIRS_GFX, MOVEMENT_TYPE_NONE,
                                                 MD_STAIRS_LOCALID, x, y, MD_ELEVATION);
    if (objId < OBJECT_EVENTS_COUNT)
        sMd.stairsSpawned = TRUE;
}

// Every camera update runs RemoveObjectEventIfOutsideView, which deletes any
// object event that has wandered off screen. That was quietly eating the guide
// the moment it spawned (it starts up to twelve tiles away) and picking off
// enemies as the player walked, leaving their health bars behind with nothing
// on the floor to match.
//
// The cull spares anything whose initialCoords are still in view, so the whole
// cast parks its initialCoords on the player and rides along with the camera.
// Nothing on this floor reads initialCoords for anything else: they all run
// MOVEMENT_TYPE_NONE with no movement range.
static void Md_KeepFloorLoaded(void)
{
    struct ObjectEvent *playerObj = Md_PlayerObj();
    u8 localIds[MD_MAX_ENEMIES + 2];
    u8 count = 0;
    u8 i;

    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        struct MdEnemy *enemy = &sMd.enemies[i];

        if (!enemy->active)
            continue;

        // A slot whose object event is gone can never be found or fought again.
        if (enemy->objEventId >= OBJECT_EVENTS_COUNT
         || !gObjectEvents[enemy->objEventId].active
         || gObjectEvents[enemy->objEventId].localId != enemy->localId)
        {
            enemy->active = FALSE;
            continue;
        }

        localIds[count++] = enemy->localId;
    }

    if (sMd.stairsSpawned)
        localIds[count++] = MD_STAIRS_LOCALID;
    localIds[count++] = MD_START_NPC_LOCALID;

    for (i = 0; i < count; i++)
    {
        u8 objId = GetObjectEventIdByLocalIdAndMap(localIds[i],
                                                   gSaveBlock1Ptr->location.mapNum,
                                                   gSaveBlock1Ptr->location.mapGroup);
        if (objId >= OBJECT_EVENTS_COUNT)
            continue;

        gObjectEvents[objId].initialCoords.x = playerObj->currentCoords.x;
        gObjectEvents[objId].initialCoords.y = playerObj->currentCoords.y;
    }

    // The map respawns the starter Pikachu from its template if it ever is
    // culled, and a fresh spawn comes back visible.
    Md_HideStartNpc();

    // A floor with no guide cannot be cleared, so put one back if it is lost.
    if (sMd.stairsSpawned)
    {
        u8 objId = GetObjectEventIdByLocalIdAndMap(MD_STAIRS_LOCALID,
                                                   gSaveBlock1Ptr->location.mapNum,
                                                   gSaveBlock1Ptr->location.mapGroup);
        if (objId >= OBJECT_EVENTS_COUNT || !gObjectEvents[objId].active)
        {
            sMd.stairsSpawned = FALSE;
            Md_SpawnStairs();
        }
    }
}

static void Md_DespawnAll(void)
{
    u8 i;

    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        if (sMd.enemies[i].active)
        {
            RemoveObjectEventByLocalIdAndMap(sMd.enemies[i].localId,
                                             gSaveBlock1Ptr->location.mapNum,
                                             gSaveBlock1Ptr->location.mapGroup);
            sMd.enemies[i].active = FALSE;
        }
    }

    if (sMd.stairsSpawned)
    {
        RemoveObjectEventByLocalIdAndMap(MD_STAIRS_LOCALID,
                                         gSaveBlock1Ptr->location.mapNum,
                                         gSaveBlock1Ptr->location.mapGroup);
        sMd.stairsSpawned = FALSE;
    }
}

static void Md_KillEnemy(u8 index)
{
    if (!sMd.enemies[index].active)
        return;

    RemoveObjectEventByLocalIdAndMap(sMd.enemies[index].localId,
                                     gSaveBlock1Ptr->location.mapNum,
                                     gSaveBlock1Ptr->location.mapGroup);
    sMd.enemies[index].active = FALSE;
}

// --- Messages ---------------------------------------------------------------

static void Md_SetMessage(u8 nextState)
{
    sMd.messageTimer = MD_MESSAGE_FRAMES;
    sMd.nextState = nextState;
    sMd.state = MD_STATE_MESSAGE;
    sMd.messageSerial++;
}

static void Md_BuildHitMessage(const u8 *attackerName, const struct MdMove *move,
                               const u8 *targetName, s32 damage, u8 typePercent, bool8 fainted)
{
    u8 *str = sMdMessageBuffer;

    str = StringCopy(str, attackerName);
    str = StringCopy(str, COMPOUND_STRING(" used "));
    str = StringCopy(str, move->name);
    str = StringCopy(str, COMPOUND_STRING("!\n"));

    if (typePercent == 0)
    {
        str = StringCopy(str, COMPOUND_STRING("No effect on "));
        str = StringCopy(str, targetName);
        str = StringCopy(str, COMPOUND_STRING("..."));
        return;
    }

    if (typePercent > 100)
        str = StringCopy(str, COMPOUND_STRING("Super effective!\n"));
    else if (typePercent < 100)
        str = StringCopy(str, COMPOUND_STRING("Not very effective.\n"));

    str = StringCopy(str, targetName);
    str = StringCopy(str, COMPOUND_STRING(" took "));
    str = ConvertIntToDecimalStringN(str, damage, STR_CONV_MODE_LEFT_ALIGN, 3);
    str = StringCopy(str, COMPOUND_STRING(" damage!"));

    if (fainted)
    {
        str = StringCopy(str, COMPOUND_STRING("\n"));
        str = StringCopy(str, targetName);
        str = StringCopy(str, COMPOUND_STRING(" fainted!"));
    }
}

static void Md_PlayMoveFx(u8 moveId, s16 x, s16 y)
{
    gFieldEffectArguments[0] = x;
    gFieldEffectArguments[1] = y;
    gFieldEffectArguments[2] = 0;
    gFieldEffectArguments[3] = 1;

    // The ground-impact puff is a 16x8 smudge that barely reads on the floor, so
    // the moves that are supposed to look like something use the 16x16 effects.
    switch (moveId)
    {
    case MD_MOVE_BUBBLE:
        FieldEffectStart(FLDEFF_BUBBLES);
        PlaySE(SE_M_BUBBLE);
        break;
    case MD_MOVE_ROCK_THROW:
        FieldEffectStart(FLDEFF_ASH_LAUNCH);
        PlaySE(SE_M_ROCK_THROW);
        break;
    case MD_MOVE_EMBER:
        FieldEffectStart(FLDEFF_ASH_PUFF);
        PlaySE(SE_M_EMBER);
        break;
    case MD_MOVE_VINE_WHIP:
        FieldEffectStart(FLDEFF_JUMP_LONG_GRASS);
        PlaySE(SE_M_BIND);
        break;
    case MD_MOVE_SCRATCH:
        FieldEffectStart(FLDEFF_DUST);
        PlaySE(SE_M_SCRATCH);
        break;
    case MD_MOVE_TACKLE:
        FieldEffectStart(FLDEFF_DUST);
        PlaySE(SE_M_HEADBUTT);
        break;
    default:
        gFieldEffectArguments[0] = x - MAP_OFFSET;
        gFieldEffectArguments[1] = y - MAP_OFFSET;
        gFieldEffectArguments[2] = 1;
        FieldEffectStart(FLDEFF_SPARKLE);
        PlaySE(SE_M_COMET_PUNCH);
        break;
    }
}

// --- Attacks ----------------------------------------------------------------

// Walks outward from the attacker to find the first enemy within the move's range.
static s8 Md_FindTargetInLine(s16 fromX, s16 fromY, u8 dir, u8 range)
{
    s16 dx, dy;
    u8 step;

    Md_DirToDelta(dir, &dx, &dy);

    for (step = 1; step <= range; step++)
    {
        s16 x = fromX + dx * step;
        s16 y = fromY + dy * step;
        s8 enemyIdx = Md_EnemyAt(x, y);

        if (enemyIdx >= 0)
            return enemyIdx;

        // Ranged moves stop at the first wall or bystander.
        if (!Md_IsTileFree(x, y))
            return -1;
    }

    return -1;
}

// Level and HP grow during the run. The species table stays the level-10 baseline.
static struct MdSpeciesStats Md_PlayerStats(void)
{
    struct MdSpeciesStats stats = *Md_BaseStats();
    u8 bonus = 0;

    stats.level = sMd.playerLevel;
    stats.maxHp = sMd.playerMaxHp;
    if (sMd.playerLevel > Md_BaseStats()->level)
        bonus = sMd.playerLevel - Md_BaseStats()->level;
    stats.atk += bonus;
    stats.def += bonus;
    stats.spAtk += bonus;
    stats.spDef += bonus;
    return stats;
}

static u16 Md_ExpForLevel(u8 level)
{
    return (u16)level * 4;
}

// Returns how many levels were gained, so the caller can announce them.
static u8 Md_GainExp(u16 amount)
{
    u8 gained = 0;

    if (sMd.playerLevel >= 100)
        return 0;

    sMd.playerExp += amount;
    while (sMd.playerLevel < 100 && sMd.playerExp >= sMd.expToNext)
    {
        sMd.playerExp -= sMd.expToNext;
        sMd.playerLevel++;
        sMd.playerMaxHp += 2;
        sMd.playerHp += 2;
        if (sMd.playerHp > sMd.playerMaxHp)
            sMd.playerHp = sMd.playerMaxHp;
        sMd.expToNext = Md_ExpForLevel(sMd.playerLevel);
        gained++;
    }

    return gained;
}

// One level is +2 max HP (and that much current HP) and +1 to each other stat.
static void Md_BuildLevelUpMessage(void)
{
    u8 *str = sMdMessageBuffer;
    u8 levels = sMd.levelsGained;

    str = StringCopy(str, Md_BaseStats()->name);
    str = StringCopy(str, COMPOUND_STRING(" grew to Lv. "));
    str = ConvertIntToDecimalStringN(str, sMd.playerLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    str = StringCopy(str, COMPOUND_STRING("!\nMax HP +"));
    str = ConvertIntToDecimalStringN(str, levels * 2, STR_CONV_MODE_LEFT_ALIGN, 3);
    str = StringCopy(str, COMPOUND_STRING("\nAttack +"));
    str = ConvertIntToDecimalStringN(str, levels, STR_CONV_MODE_LEFT_ALIGN, 2);
    str = StringCopy(str, COMPOUND_STRING("   Defense +"));
    str = ConvertIntToDecimalStringN(str, levels, STR_CONV_MODE_LEFT_ALIGN, 2);
    str = StringCopy(str, COMPOUND_STRING("\nSp. Atk +"));
    str = ConvertIntToDecimalStringN(str, levels, STR_CONV_MODE_LEFT_ALIGN, 2);
    str = StringCopy(str, COMPOUND_STRING("   Sp. Def +"));
    ConvertIntToDecimalStringN(str, levels, STR_CONV_MODE_LEFT_ALIGN, 2);
}

static void Md_PlayerUseMove(u8 slot)
{
    u8 moveId = Md_PlayerMoveId(slot);
    const struct MdMove *move = &sMdMoves[moveId];
    struct MdSpeciesStats player = Md_PlayerStats();
    struct ObjectEvent *playerObj = Md_PlayerObj();
    s8 targetIdx;
    s32 damage;
    u8 typePercent;
    bool8 fainted = FALSE;

    if (move->maxPp != 0 && sMd.playerPp[slot] == 0)
    {
        StringCopy(sMdMessageBuffer, COMPOUND_STRING("There's no PP left for that move!"));
        Md_SetMessage(MD_STATE_INPUT);
        PlaySE(SE_FAILURE);
        return;
    }

    if (move->maxPp != 0)
        sMd.playerPp[slot]--;

    targetIdx = Md_FindTargetInLine(playerObj->currentCoords.x, playerObj->currentCoords.y,
                                    playerObj->facingDirection, move->range);

    if (targetIdx < 0)
    {
        s16 dx, dy;
        u8 *str = sMdMessageBuffer;

        Md_DirToDelta(playerObj->facingDirection, &dx, &dy);
        Md_PlayMoveFx(moveId, playerObj->currentCoords.x + dx, playerObj->currentCoords.y + dy);
        str = StringCopy(str, player.name);
        str = StringCopy(str, COMPOUND_STRING(" used "));
        str = StringCopy(str, move->name);
        str = StringCopy(str, COMPOUND_STRING("!\nIt missed."));
        Md_SetMessage(MD_STATE_ENEMY_TURN);
        sMd.enemyIndex = 0;
        return;
    }

    {
        struct MdEnemy *enemy = &sMd.enemies[targetIdx];
        const struct MdSpeciesStats *defStats = &sMdEnemyStats[enemy->statsIdx];

        typePercent = Md_TypeMultiplierPercent(move->type, defStats->type1, defStats->type2);
        damage = Md_CalcDamage(&player, defStats, move, FALSE);

        enemy->hp -= damage;
        if (enemy->hp <= 0)
        {
            enemy->hp = 0;
            fainted = TRUE;
        }

        Md_PlayMoveFx(moveId, gObjectEvents[enemy->objEventId].currentCoords.x,
                      gObjectEvents[enemy->objEventId].currentCoords.y);
        Md_BuildHitMessage(player.name, move, defStats->name, damage, typePercent, fainted);
        if (typePercent > 100)
            PlaySE(SE_SUPER_EFFECTIVE);

        if (fainted)
        {
            sMd.levelsGained = Md_GainExp(defStats->level);
            Md_KillEnemy(targetIdx);
        }
    }

    sMd.enemyIndex = 0;
    Md_SetMessage(MD_STATE_ENEMY_TURN);
}

static void Md_EnemyUseMove(u8 enemyIdx, u8 moveSlot)
{
    struct MdEnemy *enemy = &sMd.enemies[enemyIdx];
    const struct MdMove *move = &sMdMoves[sMdEnemyMoves[moveSlot]];
    const struct MdSpeciesStats *atkStats = &sMdEnemyStats[enemy->statsIdx];
    struct MdSpeciesStats player = Md_PlayerStats();
    s32 damage;
    u8 typePercent;
    bool8 fainted = FALSE;

    if (move->maxPp != 0 && enemy->pp[moveSlot] != 0)
        enemy->pp[moveSlot]--;

    typePercent = Md_TypeMultiplierPercent(move->type, player.type1, player.type2);
    damage = Md_CalcDamage(atkStats, &player, move, TRUE);

    sMd.playerHp -= damage;
    if (sMd.playerHp <= 0)
    {
        sMd.playerHp = 0;
        fainted = TRUE;
    }

    Md_PlayMoveFx(sMdEnemyMoves[moveSlot], Md_PlayerObj()->currentCoords.x, Md_PlayerObj()->currentCoords.y);
    Md_BuildHitMessage(atkStats->name, move, player.name, damage, typePercent, fainted);
    if (typePercent > 100)
        PlaySE(SE_SUPER_EFFECTIVE);

    if (fainted)
    {
        sMd.pendingEnd = TRUE;
        sMd.reachedStairs = FALSE;
    }
}

// --- Enemy AI ---------------------------------------------------------------

// Returns TRUE if the enemy attacked, FALSE if it should move instead.
static bool8 Md_EnemyTryAttack(u8 enemyIdx)
{
    struct MdEnemy *enemy = &sMd.enemies[enemyIdx];
    struct ObjectEvent *enemyObj = &gObjectEvents[enemy->objEventId];
    struct ObjectEvent *playerObj = Md_PlayerObj();
    s16 dx = playerObj->currentCoords.x - enemyObj->currentCoords.x;
    s16 dy = playerObj->currentCoords.y - enemyObj->currentCoords.y;
    u8 dir = DIR_NONE;
    u16 dist = abs(dx) + abs(dy);

    if (dx == 0 && dy != 0)
        dir = (dy > 0) ? DIR_SOUTH : DIR_NORTH;
    else if (dy == 0 && dx != 0)
        dir = (dx > 0) ? DIR_EAST : DIR_WEST;

    if (dir == DIR_NONE)
        return FALSE;

    ObjectEventTurn(enemyObj, dir);

    // Both enemy moves only reach the tile in front. Rock Throw goes first
    // while it has PP; Tackle covers the same tile once that runs out.
    if (dist != 1)
        return FALSE;

    if (enemy->pp[1] != 0)
        Md_EnemyUseMove(enemyIdx, 1);
    else
        Md_EnemyUseMove(enemyIdx, 0);

    return TRUE;
}

// Everyone walks on the same frame, so object coordinates have not updated yet
// when the next enemy picks its step. Tiles taken this round are held here so
// two enemies cannot claim the same one.
static s16 sMdClaimedX[MD_MAX_ENEMIES];
static s16 sMdClaimedY[MD_MAX_ENEMIES];
static u8 sMdClaimedCount;

static bool8 Md_TileIsClaimed(s16 x, s16 y)
{
    u8 i;

    for (i = 0; i < sMdClaimedCount; i++)
    {
        if (sMdClaimedX[i] == x && sMdClaimedY[i] == y)
            return TRUE;
    }

    return FALSE;
}

// --- Chase map --------------------------------------------------------------
// Walking straight at the player wedges anyone whose only route is around a
// wall. Instead, flood the walkable tiles outward from the player once per
// round and let each enemy step to whichever neighbour is closer along that
// flood. Corners and dead ends solve themselves, and one flood serves the
// whole floor, so the cost does not grow with the number of enemies.

#define MD_CHASE_RADIUS  10 // Covers the screen and the whole spawn ring.
#define MD_CHASE_SIZE    (MD_CHASE_RADIUS * 2 + 1)
#define MD_CHASE_CELLS   (MD_CHASE_SIZE * MD_CHASE_SIZE)
#define MD_CHASE_FAR     0xFF

static EWRAM_DATA u8 sMdChaseDist[MD_CHASE_CELLS] = {0};
static EWRAM_DATA u16 sMdChaseQueue[MD_CHASE_CELLS] = {0};
static EWRAM_DATA s16 sMdChaseOriginX = 0;
static EWRAM_DATA s16 sMdChaseOriginY = 0;

static const u8 sMdStepDirs[4] = { DIR_SOUTH, DIR_NORTH, DIR_WEST, DIR_EAST };

static void Md_BuildChaseMap(void)
{
    struct ObjectEvent *playerObj = Md_PlayerObj();
    u16 head = 0;
    u16 tail = 0;
    u16 start;
    u16 i;

    sMdChaseOriginX = playerObj->currentCoords.x - MD_CHASE_RADIUS;
    sMdChaseOriginY = playerObj->currentCoords.y - MD_CHASE_RADIUS;

    for (i = 0; i < MD_CHASE_CELLS; i++)
        sMdChaseDist[i] = MD_CHASE_FAR;

    start = MD_CHASE_RADIUS * MD_CHASE_SIZE + MD_CHASE_RADIUS;
    sMdChaseDist[start] = 0;
    sMdChaseQueue[tail++] = start;

    while (head < tail)
    {
        u16 cell = sMdChaseQueue[head++];
        s16 cellX = cell % MD_CHASE_SIZE;
        s16 cellY = cell / MD_CHASE_SIZE;
        u8 nextDist = sMdChaseDist[cell] + 1;
        u8 d;

        if (nextDist == MD_CHASE_FAR)
            continue;

        for (d = 0; d < ARRAY_COUNT(sMdStepDirs); d++)
        {
            s16 stepX, stepY, nextX, nextY;
            u16 next;

            Md_DirToDelta(sMdStepDirs[d], &stepX, &stepY);
            nextX = cellX + stepX;
            nextY = cellY + stepY;

            if (nextX < 0 || nextX >= MD_CHASE_SIZE || nextY < 0 || nextY >= MD_CHASE_SIZE)
                continue;

            next = nextY * MD_CHASE_SIZE + nextX;
            if (sMdChaseDist[next] != MD_CHASE_FAR)
                continue;
            if (!Md_IsWallFree(sMdChaseOriginX + nextX, sMdChaseOriginY + nextY))
                continue;

            sMdChaseDist[next] = nextDist;
            sMdChaseQueue[tail++] = next;
        }
    }
}

static u8 Md_ChaseDistAt(s16 x, s16 y)
{
    s16 cellX = x - sMdChaseOriginX;
    s16 cellY = y - sMdChaseOriginY;

    if (cellX < 0 || cellX >= MD_CHASE_SIZE || cellY < 0 || cellY >= MD_CHASE_SIZE)
        return MD_CHASE_FAR;

    return sMdChaseDist[cellY * MD_CHASE_SIZE + cellX];
}

static bool8 Md_EnemyTryMove(u8 enemyIdx)
{
    struct MdEnemy *enemy = &sMd.enemies[enemyIdx];
    struct ObjectEvent *enemyObj = &gObjectEvents[enemy->objEventId];
    u8 here = Md_ChaseDistAt(enemyObj->currentCoords.x, enemyObj->currentCoords.y);
    u8 bestDir = DIR_NONE;
    u8 bestDist = here;
    s16 bestX = 0, bestY = 0;
    u8 i;

    if (here == MD_CHASE_FAR)
        return FALSE; // Walled off from the player, or too far to care.

    // Rotating where each enemy starts looking keeps a pack from filing into
    // the same tile order every round.
    for (i = 0; i < ARRAY_COUNT(sMdStepDirs); i++)
    {
        u8 dir = sMdStepDirs[(i + enemyIdx) & 3];
        s16 stepX, stepY, targetX, targetY;
        u8 dist;

        Md_DirToDelta(dir, &stepX, &stepY);
        targetX = enemyObj->currentCoords.x + stepX;
        targetY = enemyObj->currentCoords.y + stepY;

        dist = Md_ChaseDistAt(targetX, targetY);
        if (dist >= bestDist)
            continue;
        if (Md_TileIsClaimed(targetX, targetY))
            continue;
        if (GetCollisionAtCoords(enemyObj, targetX, targetY, dir) != 0)
            continue;

        bestDir = dir;
        bestDist = dist;
        bestX = targetX;
        bestY = targetY;
    }

    if (bestDir == DIR_NONE)
        return FALSE; // Boxed in this round; try again after everyone shuffles.

    if (sMdClaimedCount < MD_MAX_ENEMIES)
    {
        sMdClaimedX[sMdClaimedCount] = bestX;
        sMdClaimedY[sMdClaimedCount] = bestY;
        sMdClaimedCount++;
    }

    ObjectEventClearHeldMovementIfFinished(enemyObj);
    ObjectEventSetHeldMovement(enemyObj, MD_WALK_ACTION(bestDir));
    return TRUE;
}

// --- UI ---------------------------------------------------------------------

#define MD_STATUS_WIDTH   88 // 11 tiles.
#define MD_HP_BAR_WIDTH   46
#define MD_HP_BAR_HEIGHT  6

// Outlined so the bar still reads against grass or stone.
static void Md_DrawHpBar(u8 windowId, u8 x, u8 y, s32 hp, s32 maxHp)
{
    u8 track = MD_HP_BAR_WIDTH - 2;
    u8 filled;
    u8 color;

    FillWindowPixelRect(windowId, PIXEL_FILL(2), x, y, MD_HP_BAR_WIDTH, MD_HP_BAR_HEIGHT);

    if (maxHp <= 0 || hp <= 0)
        return;

    filled = (u32)track * hp / maxHp;
    if (filled == 0)
        filled = 1;

    if (hp * 5 <= maxHp)
        color = 4; // red
    else if (hp * 2 <= maxHp)
        color = 5; // orange
    else
        color = 6; // green

    FillWindowPixelRect(windowId, PIXEL_FILL(color), x + 1, y + 1, filled, MD_HP_BAR_HEIGHT - 2);
}

static void Md_DrawStatusWindow(u8 windowId, const u8 *name, s32 hp, s32 maxHp, u8 level)
{
    u8 buffer[32];
    u8 *str;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    AddTextPrinterParameterized3(windowId, FONT_SMALL, 0, 0, sMdStatusTextColor, TEXT_SKIP_DRAW, name);

    if (level != 0)
    {
        str = StringCopy(buffer, COMPOUND_STRING("Lv."));
        ConvertIntToDecimalStringN(str, level, STR_CONV_MODE_LEFT_ALIGN, 3);
        AddTextPrinterParameterized3(windowId, FONT_SMALL,
                                     GetStringRightAlignXOffset(FONT_SMALL, buffer, MD_STATUS_WIDTH),
                                     0, sMdStatusTextColor, TEXT_SKIP_DRAW, buffer);
    }

    Md_DrawHpBar(windowId, 0, 15, hp, maxHp);

    str = ConvertIntToDecimalStringN(buffer, hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    str = StringCopy(str, COMPOUND_STRING("/"));
    ConvertIntToDecimalStringN(str, maxHp, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(windowId, FONT_SMALL,
                                 GetStringRightAlignXOffset(FONT_SMALL, buffer, MD_STATUS_WIDTH),
                                 12, sMdStatusTextColor, TEXT_SKIP_DRAW, buffer);

    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void Md_DrawUi(void)
{
    u32 signature;
    s32 focusHp = 0;
    bool8 needControls = (sMd.state == MD_STATE_MESSAGE) || sMd.showControls;

    if (sMd.playerWindowId == WINDOW_NONE)
    {
        sMd.playerWindowId = AddWindow(&sMdPlayerWindowTemplate);
        LoadUserWindowBorderGfx(sMd.playerWindowId, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
        Menu_LoadStdPal();
        PutWindowTilemap(sMd.playerWindowId);
        ScheduleBgCopyTilemapToVram(MD_WINDOW_BG);
    }

    if (sMd.enemyWindowId == WINDOW_NONE)
    {
        sMd.enemyWindowId = AddWindow(&sMdEnemyWindowTemplate);
        PutWindowTilemap(sMd.enemyWindowId);
        ScheduleBgCopyTilemapToVram(MD_WINDOW_BG);
    }

    if (needControls && sMd.messageWindowId == WINDOW_NONE)
    {
        sMd.messageWindowId = AddWindow(&sMdMessageWindowTemplate);
        PutWindowTilemap(sMd.messageWindowId);
        DrawTextBorderOuter(sMd.messageWindowId, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM);
        ScheduleBgCopyTilemapToVram(MD_WINDOW_BG);
    }
    else if (!needControls && sMd.messageWindowId != WINDOW_NONE)
    {
        Md_DestroyWindow(&sMd.messageWindowId);
        ScheduleBgCopyTilemapToVram(MD_WINDOW_BG);
    }

    // Re-rendering every string each frame is wasteful, so only redraw on change.
    if (sMd.focusEnemy < MD_MAX_ENEMIES && sMd.enemies[sMd.focusEnemy].active)
        focusHp = sMd.enemies[sMd.focusEnemy].hp;

    // HP climbs past six bits once a few levels are in, so it is compared
    // on its own rather than packed into the signature.
    signature = (u32)sMd.state
              | ((u32)sMd.showControls << 3)
              | ((u32)sMd.playerPp[1] << 4)
              | ((u32)sMd.playerPp[2] << 9)
              | ((u32)sMd.focusEnemy << 15)
              | ((u32)sMd.playerLevel << 18)
              | ((u32)sMd.messageSerial << 25);

    if (signature == sMd.uiSignature && sMd.uiPlayerHp == sMd.playerHp && sMd.uiFocusHp == focusHp)
        return;
    sMd.uiSignature = signature;
    sMd.uiPlayerHp = sMd.playerHp;
    sMd.uiFocusHp = focusHp;

    Md_DrawStatusWindow(sMd.playerWindowId, Md_BaseStats()->name, sMd.playerHp, sMd.playerMaxHp, sMd.playerLevel);

    if (sMd.focusEnemy < MD_MAX_ENEMIES && sMd.enemies[sMd.focusEnemy].active)
    {
        struct MdEnemy *enemy = &sMd.enemies[sMd.focusEnemy];
        Md_DrawStatusWindow(sMd.enemyWindowId, sMdEnemyStats[enemy->statsIdx].name,
                            enemy->hp, sMdEnemyStats[enemy->statsIdx].maxHp, 0);
    }
    else
    {
        FillWindowPixelBuffer(sMd.enemyWindowId, PIXEL_FILL(0));
        AddTextPrinterParameterized3(sMd.enemyWindowId, FONT_SMALL,
                                     GetStringRightAlignXOffset(FONT_SMALL, COMPOUND_STRING("No foes near"), MD_STATUS_WIDTH),
                                     0, sMdStatusTextColor, TEXT_SKIP_DRAW,
                                     COMPOUND_STRING("No foes near"));
        CopyWindowToVram(sMd.enemyWindowId, COPYWIN_GFX);
    }

    if (sMd.messageWindowId == WINDOW_NONE)
        return;

    // Message area doubles as the control hint while idle.
    FillWindowPixelBuffer(sMd.messageWindowId, PIXEL_FILL(1));
    if (sMd.state == MD_STATE_MESSAGE)
    {
        AddTextPrinterParameterized3(sMd.messageWindowId, FONT_SMALL, 0, 0, sMdTextColor, TEXT_SKIP_DRAW, sMdMessageBuffer);
    }
    else
    {
        const struct MdMove *special = &sMdMoves[Md_PlayerMoveId(1)];
        const struct MdMove *physical = &sMdMoves[Md_PlayerMoveId(2)];
        u8 *str = sMdMessageBuffer;

        str = StringCopy(str, COMPOUND_STRING("{DPAD_NONE} Move   {A_BUTTON} Attack   {B_BUTTON}+{DPAD_NONE} Turn   {L_BUTTON} Quit\n"));
        str = StringCopy(str, COMPOUND_STRING("{R_BUTTON}+{A_BUTTON} "));
        str = StringCopy(str, special->name);
        str = StringCopy(str, COMPOUND_STRING(" "));
        str = ConvertIntToDecimalStringN(str, sMd.playerPp[1], STR_CONV_MODE_LEFT_ALIGN, 2);
        str = StringCopy(str, COMPOUND_STRING("/"));
        str = ConvertIntToDecimalStringN(str, special->maxPp, STR_CONV_MODE_LEFT_ALIGN, 2);
        str = StringCopy(str, COMPOUND_STRING("   {R_BUTTON}+{B_BUTTON} "));
        str = StringCopy(str, physical->name);
        str = StringCopy(str, COMPOUND_STRING(" "));
        str = ConvertIntToDecimalStringN(str, sMd.playerPp[2], STR_CONV_MODE_LEFT_ALIGN, 2);
        str = StringCopy(str, COMPOUND_STRING("/"));
        str = ConvertIntToDecimalStringN(str, physical->maxPp, STR_CONV_MODE_LEFT_ALIGN, 2);
        AddTextPrinterParameterized3(sMd.messageWindowId, FONT_SMALL, 0, 1, sMdTextColor, TEXT_SKIP_DRAW, sMdMessageBuffer);
    }
    CopyWindowToVram(sMd.messageWindowId, COPYWIN_GFX);
}

static void Md_DestroyWindow(u8 *windowId)
{
    s32 left, top, right, bottom;

    if (*windowId == WINDOW_NONE)
        return;

    // Clear a tile past each edge to take any frame with it, without stepping
    // off the tilemap when the window is flush against the screen.
    left = GetWindowAttribute(*windowId, WINDOW_TILEMAP_LEFT);
    top = GetWindowAttribute(*windowId, WINDOW_TILEMAP_TOP);
    right = left + GetWindowAttribute(*windowId, WINDOW_WIDTH) + 1;
    bottom = top + GetWindowAttribute(*windowId, WINDOW_HEIGHT) + 1;

    if (--left < 0)
        left = 0;
    if (--top < 0)
        top = 0;
    if (right > 31)
        right = 31;
    if (bottom > 31)
        bottom = 31;

    FillBgTilemapBufferRect(MD_WINDOW_BG, 0, left, top, right - left + 1, bottom - top + 1, STD_WINDOW_PALETTE_NUM);
    RemoveWindow(*windowId);
    *windowId = WINDOW_NONE;
}

static void Md_DestroyUi(void)
{
    Md_DestroyWindow(&sMd.messageWindowId);
    Md_DestroyWindow(&sMd.enemyWindowId);
    Md_DestroyWindow(&sMd.playerWindowId);
    ScheduleBgCopyTilemapToVram(MD_WINDOW_BG);
}

// --- Turn handling ----------------------------------------------------------

static void Md_BeginEnemyTurn(void)
{
    sMd.enemyIndex = 0;
    sMd.enemyActed = 0;
    sMd.state = MD_STATE_ENEMY_TURN;
}

static void Md_OnPlayerStep(void)
{
    if (sMd.stepsUntilRegen != 0)
        sMd.stepsUntilRegen--;

    if (sMd.stepsUntilRegen == 0)
    {
        sMd.stepsUntilRegen = MD_REGEN_STEPS;
        if (sMd.playerHp > 0 && sMd.playerHp < sMd.playerMaxHp)
            sMd.playerHp++;
    }
}

static void Md_EndRound(void)
{
    if (sMd.respawnTimer != 0)
        sMd.respawnTimer--;

    if (sMd.respawnTimer == 0)
    {
        if (Md_LivingEnemyCount() < MD_MAX_ENEMIES)
            Md_SpawnEnemy();
        sMd.respawnTimer = MD_RESPAWN_TURNS;
    }

    sMd.state = MD_STATE_INPUT;
}

static void Md_HandleInput(void)
{
    struct ObjectEvent *playerObj = Md_PlayerObj();
    u8 dir = DIR_NONE;
    s16 dx, dy, targetX, targetY;

    if (JOY_NEW(START_BUTTON))
    {
        PlaySE(SE_SELECT);
        sMd.showControls = !sMd.showControls;
        sMd.uiSignature = 0;
        return;
    }

    if (JOY_NEW(L_BUTTON))
    {
        Md_End(FALSE);
        return;
    }

    if (JOY_HELD(R_BUTTON) && JOY_NEW(A_BUTTON))
    {
        Md_PlayerUseMove(1); // Bubble
        return;
    }

    if (JOY_HELD(R_BUTTON) && JOY_NEW(B_BUTTON))
    {
        Md_PlayerUseMove(2); // Tackle
        return;
    }

    if (JOY_NEW(A_BUTTON))
    {
        Md_PlayerUseMove(0); // Attack
        return;
    }

    if (JOY_HELD(DPAD_DOWN))
        dir = DIR_SOUTH;
    else if (JOY_HELD(DPAD_UP))
        dir = DIR_NORTH;
    else if (JOY_HELD(DPAD_LEFT))
        dir = DIR_WEST;
    else if (JOY_HELD(DPAD_RIGHT))
        dir = DIR_EAST;

    if (dir == DIR_NONE)
        return;

    // Holding B (without R) turns on the spot without spending the turn.
    if (JOY_HELD(B_BUTTON) && !JOY_HELD(R_BUTTON))
    {
        ObjectEventTurn(playerObj, dir);
        return;
    }

    ObjectEventTurn(playerObj, dir);

    Md_DirToDelta(dir, &dx, &dy);
    targetX = playerObj->currentCoords.x + dx;
    targetY = playerObj->currentCoords.y + dy;

    if (Md_IsStairsAt(targetX, targetY))
    {
        Md_End(TRUE);
        return;
    }

    // A body in the way only turns you toward it. Attacks are buttons.
    if (Md_EnemyAt(targetX, targetY) >= 0)
        return;

    if (GetCollisionAtCoords(playerObj, targetX, targetY, dir) != 0)
        return; // Bumped a wall: the turn is not spent.

    ObjectEventClearHeldMovementIfFinished(playerObj);
    ObjectEventSetHeldMovement(playerObj, MD_WALK_ACTION(dir));
    sMd.state = MD_STATE_PLAYER_MOVING;
}

static void Md_HandleEnemyTurn(void)
{
    struct MdEnemy *enemy;
    bool8 anyMoving;
    u8 i;

    // Attacks stay in order so a message can show for each one.
    // Walking does not: every enemy who is only moving steps on the same frame.
    while (sMd.enemyIndex < MD_MAX_ENEMIES)
    {
        enemy = &sMd.enemies[sMd.enemyIndex];

        if (!enemy->active || enemy->objEventId >= OBJECT_EVENTS_COUNT
         || !gObjectEvents[enemy->objEventId].active)
        {
            sMd.enemyIndex++;
            continue;
        }

        if (Md_EnemyTryAttack(sMd.enemyIndex))
        {
            sMd.enemyActed |= (1u << sMd.enemyIndex);
            sMd.enemyIndex++;
            Md_SetMessage(MD_STATE_ENEMY_TURN);
            return;
        }

        sMd.enemyIndex++;
    }

    // The player stands still for the whole enemy phase, so one flood serves
    // every walker this round.
    anyMoving = FALSE;
    sMdClaimedCount = 0;
    Md_BuildChaseMap();
    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        enemy = &sMd.enemies[i];
        if (!enemy->active || (sMd.enemyActed & (1u << i)))
            continue;
        if (enemy->objEventId >= OBJECT_EVENTS_COUNT || !gObjectEvents[enemy->objEventId].active)
            continue;
        if (Md_EnemyTryMove(i))
            anyMoving = TRUE;
    }

    if (anyMoving)
        sMd.state = MD_STATE_ENEMY_MOVING;
    else
        Md_EndRound();
}

static void Md_HandleEnemyMoving(void)
{
    u8 i;

    for (i = 0; i < MD_MAX_ENEMIES; i++)
    {
        struct MdEnemy *enemy = &sMd.enemies[i];

        if (!enemy->active || enemy->objEventId >= OBJECT_EVENTS_COUNT)
            continue;
        if (!gObjectEvents[enemy->objEventId].active)
            continue;
        if (!ObjectEventClearHeldMovementIfFinished(&gObjectEvents[enemy->objEventId]))
            return;
    }

    Md_EndRound();
}

// --- Main task --------------------------------------------------------------

static void Task_MysteryDungeon(u8 taskId)
{
    // Give the script a frame to reach `waitstate` before anything can finish.
    if (gTasks[taskId].data[0]++ < 1)
        return;

    Md_KeepFloorLoaded();
    Md_UpdateFocusEnemy();

    switch (sMd.state)
    {
    case MD_STATE_INPUT:
        Md_HandleInput();
        break;

    case MD_STATE_PLAYER_MOVING:
        if (ObjectEventClearHeldMovementIfFinished(Md_PlayerObj()))
        {
            Md_OnPlayerStep();
            Md_BeginEnemyTurn();
        }
        break;

    case MD_STATE_MESSAGE:
        if (sMd.messageTimer != 0)
            sMd.messageTimer--;

        // Damage text times out on its own. A level-up stays until a button,
        // since it is the only record of what the stats became.
        if ((sMd.messageTimer == 0 && !sMd.messageHold) || JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
        {
            sMd.messageHold = FALSE;

            if (sMd.levelsGained != 0)
            {
                Md_BuildLevelUpMessage();
                Md_SetMessage(sMd.nextState);
                sMd.messageHold = TRUE;
                sMd.levelsGained = 0;
                PlayFanfare(MUS_LEVEL_UP);
                return;
            }

            if (sMd.pendingEnd)
            {
                Md_End(sMd.reachedStairs);
                return;
            }
            sMd.state = sMd.nextState;
        }
        break;

    case MD_STATE_ENEMY_TURN:
        Md_HandleEnemyTurn();
        break;

    case MD_STATE_ENEMY_MOVING:
        Md_HandleEnemyMoving();
        break;
    }

    if (sMd.active && !sMd.ending)
        Md_DrawUi();
}

// --- Start / end ------------------------------------------------------------

void StartMysteryDungeon(void)
{
    u8 i;

    if (sMd.active)
        return;

    sMd = (struct MdSession){0};
    sMd.active = TRUE;
    sMd.taskId = TASK_NONE;
    sMd.playerWindowId = WINDOW_NONE;
    sMd.enemyWindowId = WINDOW_NONE;
    sMd.messageWindowId = WINDOW_NONE;
    sMd.fadeTextWindowId = WINDOW_NONE;
    sMd.focusEnemy = MD_MAX_ENEMIES;
    sMd.respawnTimer = MD_RESPAWN_TURNS;
    sMd.showControls = TRUE;
    sMd.state = MD_STATE_INPUT;

    // The guide's multichoice leaves the pick in VAR_0x8004.
    sMd.starter = (gSpecialVar_0x8004 < MD_STARTER_COUNT) ? gSpecialVar_0x8004 : MD_STARTER_SQUIRTLE;

    sMd.playerLevel = Md_BaseStats()->level;
    sMd.playerMaxHp = Md_BaseStats()->maxHp;
    sMd.playerHp = sMd.playerMaxHp;
    sMd.playerExp = 0;
    sMd.expToNext = Md_ExpForLevel(sMd.playerLevel);
    sMd.stepsUntilRegen = MD_REGEN_STEPS;
    for (i = 0; i < MD_PLAYER_MOVE_COUNT; i++)
        sMd.playerPp[i] = sMdMoves[Md_PlayerMoveId(i)].maxPp;

    gSpecialVar_Result = FALSE;

    // Keep the starter Pikachu out of the floor. Removing it makes the map
    // spawn it again, so it stays loaded and invisible until the run ends.
    Md_HideStartNpc();

    // The calling script's `lock` froze every object, which also pauses their
    // walk animations. Held movements still run while frozen, but they would
    // slide instead of animate, so clear it. Field controls stay locked, and
    // everything on the floor uses MOVEMENT_TYPE_NONE, so nothing moves on its own.
    LockPlayerFieldControls();
    UnfreezeObjectEvents();

    // Put the follower away for the whole run.
    {
        struct ObjectEvent *followerObj = GetFollowerObject();
        if (followerObj != NULL && followerObj->active)
        {
            ClearObjectEventMovement(followerObj, &gSprites[followerObj->spriteId]);
            ObjectEventSetHeldMovement(followerObj, MOVEMENT_ACTION_ENTER_POKEBALL);
            sMd.followerWasHidden = TRUE;
        }
    }

    sMd.originalPlayerGraphicsId = Md_PlayerObj()->graphicsId;

    sMd.fadeState = MD_FADE_OUT_START;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_MysteryDungeonFade, 0);
}

static void Md_End(bool8 reachedStairs)
{
    u8 taskId = sMd.taskId;
    u8 fadeTaskId;
    bool8 foundFadeTask = FALSE;

    if (sMd.ending)
        return;

    LockPlayerFieldControls();

    sMd.ending = TRUE;
    sMd.reachedStairs = reachedStairs;
    gSpecialVar_Result = reachedStairs;

    Md_DestroyUi();

    if (taskId != TASK_NONE && gTasks[taskId].isActive)
        DestroyTask(taskId);
    sMd.taskId = TASK_NONE;

    sMd.fadeState = MD_FADE_OUT_END;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);

    for (fadeTaskId = 0; fadeTaskId < NUM_TASKS; fadeTaskId++)
    {
        if (gTasks[fadeTaskId].isActive && gTasks[fadeTaskId].func == Task_MysteryDungeonFade)
        {
            foundFadeTask = TRUE;
            break;
        }
    }
    if (!foundFadeTask)
        CreateTask(Task_MysteryDungeonFade, 0);
}

static void Md_HideAllObjectSprites(bool8 hide)
{
    u32 i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *obj = &gObjectEvents[i];

        if (!obj->active)
            continue;

        if (hide)
        {
            if (!obj->invisible)
                sMd.objWasVisible |= (1u << i);
            obj->invisible = TRUE;
            if (obj->spriteId < MAX_SPRITES)
                gSprites[obj->spriteId].invisible = TRUE;
        }
        else if (!Md_IsStartNpc(obj) && (sMd.objWasVisible & (1u << i)))
        {
            obj->invisible = FALSE;
            if (obj->spriteId < MAX_SPRITES)
                gSprites[obj->spriteId].invisible = FALSE;
        }
    }
}

// Redraw the three map layers from the map buffer. Cheap insurance for the
// frame the black card goes away, and it keeps the text layer behind nothing.
static void Md_RedrawField(void)
{
    SetBgAttribute(MD_WINDOW_BG, BG_ATTR_PRIORITY, 0);
    ShowBg(MD_WINDOW_BG);
    DrawWholeMapView();
}

static void Md_ShowFullscreenText(const u8 *text)
{
    struct WindowTemplate template =
    {
        .bg = 0, .tilemapLeft = 0, .tilemapTop = 8, .width = 30, .height = 4,
        .paletteNum = 15, .baseBlock = MD_FADE_WIN_BASE,
    };

    Menu_LoadStdPalAt(BG_PLTT_ID(15));
    sMd.fadeTextWindowId = AddWindow(&template);
    PutWindowTilemap(sMd.fadeTextWindowId);
    FillWindowPixelBuffer(sMd.fadeTextWindowId, PIXEL_FILL(0));
    AddTextPrinterParameterized3(sMd.fadeTextWindowId, FONT_NORMAL,
                                 GetStringCenterAlignXOffset(FONT_NORMAL, text, 240), 8,
                                 sMdFadeTextColor, TEXT_SKIP_DRAW, text);
    CopyWindowToVram(sMd.fadeTextWindowId, COPYWIN_FULL);
}

static void Md_HideFullscreenText(void)
{
    if (sMd.fadeTextWindowId == WINDOW_NONE)
        return;

    ClearWindowTilemap(sMd.fadeTextWindowId);
    RemoveWindow(sMd.fadeTextWindowId);
    sMd.fadeTextWindowId = WINDOW_NONE;
    CopyBgTilemapBufferToVram(0);
}

static void Task_MysteryDungeonFade(u8 taskId)
{
    struct ObjectEvent *playerObj = Md_PlayerObj();

    switch (sMd.fadeState)
    {
    case MD_FADE_OUT_START:
        if (gPaletteFade.active)
            break;

        Md_HideAllObjectSprites(TRUE);
        ObjectEventSetGraphicsId(playerObj, OBJ_EVENT_MON + Md_BaseStats()->species);
        ObjectEventTurn(playerObj, playerObj->facingDirection);

        Md_HideStartNpc();
        Md_SpawnStairs();
        {
            u8 i;
            for (i = 0; i < MD_MAX_ENEMIES; i++)
                Md_SpawnEnemy();
        }

        Md_KeepFloorLoaded();
        Md_HideAllObjectSprites(TRUE);
        Md_ShowFullscreenText(COMPOUND_STRING("Mystery Dungeon  B1F"));
        PlaySE(SE_SUPER_EFFECTIVE);

        sMd.fadeState = MD_FADE_SHOW_START;
        sMd.fadeTextTimer = MD_INTRO_FRAMES;
        break;

    case MD_FADE_SHOW_START:
        Md_HideAllObjectSprites(TRUE);
        if (--sMd.fadeTextTimer != 0)
            break;

        Md_HideFullscreenText();
        Md_RedrawField();
        sMd.fadeState = MD_FADE_IN_START;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        Md_HideAllObjectSprites(FALSE);
        break;

    case MD_FADE_IN_START:
        if (gPaletteFade.active)
            break;

        sMd.fadeState = MD_FADE_NONE;
        sMd.taskId = CreateTask(Task_MysteryDungeon, 0);
        Md_DrawUi();
        DestroyTask(taskId);
        break;

    case MD_FADE_OUT_END:
        if (gPaletteFade.active)
            break;

        Md_DespawnAll();
        Md_HideAllObjectSprites(TRUE);
        ObjectEventSetGraphicsId(playerObj, sMd.originalPlayerGraphicsId);
        ObjectEventTurn(playerObj, playerObj->facingDirection);

        if (sMd.followerWasHidden)
            UpdateFollowingPokemon();

        Md_ShowStartNpc();
        Md_HideAllObjectSprites(TRUE);
        Md_ShowFullscreenText(sMd.reachedStairs
            ? COMPOUND_STRING("You climbed to the next floor!")
            : COMPOUND_STRING("You were knocked out..."));
        PlaySE(sMd.reachedStairs ? SE_EXP_MAX : SE_FAILURE);

        sMd.fadeState = MD_FADE_SHOW_END;
        sMd.fadeTextTimer = MD_INTRO_FRAMES;
        break;

    case MD_FADE_SHOW_END:
        Md_HideAllObjectSprites(TRUE);
        if (--sMd.fadeTextTimer != 0)
            break;

        Md_HideFullscreenText();
        Md_RedrawField();
        sMd.fadeState = MD_FADE_IN_END;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        Md_HideAllObjectSprites(FALSE);
        break;

    case MD_FADE_IN_END:
        if (gPaletteFade.active)
            break;

        // The calling script's `release` restores field control.
        sMd = (struct MdSession){0};
        ScriptContext_Enable();
        DestroyTask(taskId);
        break;
    }
}

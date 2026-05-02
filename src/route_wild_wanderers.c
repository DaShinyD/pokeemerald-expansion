#include "global.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/map_types.h"
#include "constants/pokemon.h"
#include "constants/trainer_types.h"
#include "config/item.h"
#include "config/overworld.h"
#include "config/pokemon.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_player_avatar.h"
#include "item.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "pokemon.h"
#include "random.h"
#include "route_wild_wanderers.h"
#include "script.h"
#include "sound.h"
#include "wild_encounter.h"
#include "constants/songs.h"
#include "battle_pyramid.h"
#include "trainer_hill.h"
#include "constants/metatile_behaviors.h"
#include "constants/wild_encounter.h"

#if OW_POKEMON_OBJECT_EVENTS

extern const u8 RouteWildWandering_EventScript[];

#define HEADER_NONE 0xFFFF

/* Minimum Chebyshev distance between wanderer spawn tiles; try 4→1 so they don’t clump. */
#define ROUTE_WILD_MAX_SPAWN_SEPARATION 4

/* ROM NPC density: skip tiles with too many nearby map objects (script clusters / palette load). */
#define ROUTE_WILD_CLUSTER_RADIUS 3
#define ROUTE_WILD_CLUSTER_MAX_ROM 4

/* Recycle wanderer templates that drift far from the player (tiles, Chebyshev). */
#define ROUTE_WILD_RELOCATE_DISTANCE 20

#define ROUTE_WILD_CAMERA_UPDATE_INTERVAL 10

/* Rough cap: ~1 wanderer per N grass tiles when grass exists (e.g. 20 tiles → ~4 max). */
#define ROUTE_WILD_GRASS_TILES_PER_MON 5

/* If land is ≥ this % of land+water tiles, reserve one surf wanderer when surf water exists. */
#define ROUTE_WILD_FORCE_WATER_LANDPCT 60

/*
 * If surf water tiles are at least this multiple of grass-eligible tiles, treat the route as
 * "water-dominated" for the no-Surf land skew (see RouteWildEffectiveLandRollPercent).
 */
#define ROUTE_WILD_NO_SURF_WATER_VS_GRASS_RATIO 3

/* Min land slot probability when water >> grass and HM Surf not received yet (actual tile % unchanged). */
#define ROUTE_WILD_NO_SURF_MIN_LAND_ROLL_PCT 78

/* Extra pull toward rarest encounter slots (see RouteWildChooseLandIndexForWanderer). */
#define ROUTE_WILD_RARE_LAND_BIAS_PERCENT 20
#define ROUTE_WILD_RARE_WATER_BIAS_PERCENT 25

static const u16 sRouteWildFlags[ROUTE_WILD_MON_MAX] = {
    FLAG_TEMP_5,
    FLAG_TEMP_6,
    FLAG_TEMP_7,
    FLAG_TEMP_8,
    FLAG_TEMP_9,
    FLAG_TEMP_A,
};

static const u8 sRouteWildLocalIds[ROUTE_WILD_MON_MAX] = {
    LOCALID_ROUTE_WILD_FIRST + 0,
    LOCALID_ROUTE_WILD_FIRST + 1,
    LOCALID_ROUTE_WILD_FIRST + 2,
    LOCALID_ROUTE_WILD_FIRST + 3,
    LOCALID_ROUTE_WILD_FIRST + 4,
    LOCALID_ROUTE_WILD_FIRST + 5,
};

static const s8 sSpawnOffsets[ROUTE_WILD_MON_MAX][2] = {
    { 2, 0 },
    { -2, 0 },
    { 0, 2 },
    { 0, -2 },
    { 3, 1 },
    { -3, -1 },
};

static EWRAM_DATA bool8 sRouteWildWanderingActive = FALSE;
static EWRAM_DATA u8 sRouteWildActiveCount = 0;
static EWRAM_DATA u16 sRouteWildSpecies[ROUTE_WILD_MON_MAX];
static EWRAM_DATA u8 sRouteWildLevels[ROUTE_WILD_MON_MAX];
static EWRAM_DATA u32 sRouteWildPersonality[ROUTE_WILD_MON_MAX];
static EWRAM_DATA bool8 sRouteWildIsShiny[ROUTE_WILD_MON_MAX];
static EWRAM_DATA u8 sRouteWildSlotKind[ROUTE_WILD_MON_MAX];
static EWRAM_DATA u8 sRouteWildPrevOverlapObjId = 0;
static EWRAM_DATA bool8 sRouteWildDeferredSetupPending = FALSE;
static EWRAM_DATA bool8 sRouteWildHiddenForScriptMovement = FALSE;

static u16 RouteWildMapKey(void)
{
    return (u16)((gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum);
}

static void RouteWildPersistClearSlot(u16 mapKey, u8 slotIndex)
{
    s32 j;
    struct RouteWildPersistedShiny *e;

    for (j = 0; j < ROUTE_WILD_PERSISTED_SHINY_COUNT; j++)
    {
        e = &gSaveBlock3Ptr->routeWildPersistedShinies[j];
        if (e->species != SPECIES_NONE && e->mapKey == mapKey && e->slotIndex == slotIndex)
        {
            e->species = SPECIES_NONE;
            return;
        }
    }
}

static void RouteWildPersistSaveShiny(u8 slotIndex, u16 species, u32 personality, u8 slotKind, u8 level)
{
    u16 mapKey = RouteWildMapKey();
    s32 i, idx;
    struct RouteWildPersistedShiny *e;

    idx = -1;
    for (i = 0; i < ROUTE_WILD_PERSISTED_SHINY_COUNT; i++)
    {
        e = &gSaveBlock3Ptr->routeWildPersistedShinies[i];
        if (e->species != SPECIES_NONE && e->mapKey == mapKey && e->slotIndex == slotIndex)
        {
            idx = i;
            break;
        }
    }

    if (idx < 0)
    {
        for (i = 0; i < ROUTE_WILD_PERSISTED_SHINY_COUNT; i++)
        {
            e = &gSaveBlock3Ptr->routeWildPersistedShinies[i];
            if (e->species == SPECIES_NONE)
            {
                idx = i;
                break;
            }
        }
    }

    if (idx < 0)
    {
        idx = gSaveBlock3Ptr->routeWildPersistEvictCursor % ROUTE_WILD_PERSISTED_SHINY_COUNT;
        gSaveBlock3Ptr->routeWildPersistEvictCursor++;
    }

    e = &gSaveBlock3Ptr->routeWildPersistedShinies[idx];
    e->mapKey = mapKey;
    e->slotIndex = slotIndex;
    e->slotKind = slotKind;
    e->species = species;
    e->personality = personality;
    e->level = level;
    e->unused = 0;
}

static bool8 RouteWildPersistTryLoadShinySlot(u8 slotIndex)
{
    u16 mapKey = RouteWildMapKey();
    u32 value;
    s32 i;
    struct RouteWildPersistedShiny *e;

    value = gSaveBlock2Ptr->playerTrainerId[0]
          | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
          | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
          | (gSaveBlock2Ptr->playerTrainerId[3] << 24);

    for (i = 0; i < ROUTE_WILD_PERSISTED_SHINY_COUNT; i++)
    {
        e = &gSaveBlock3Ptr->routeWildPersistedShinies[i];
        if (e->species == SPECIES_NONE || e->mapKey != mapKey || e->slotIndex != slotIndex)
            continue;

        if (GET_SHINY_VALUE(value, e->personality) >= SHINY_ODDS)
        {
            e->species = SPECIES_NONE;
            return FALSE;
        }

        sRouteWildSpecies[slotIndex] = e->species;
        sRouteWildLevels[slotIndex] = e->level;
        sRouteWildPersonality[slotIndex] = e->personality;
        sRouteWildSlotKind[slotIndex] = e->slotKind;
        sRouteWildIsShiny[slotIndex] = TRUE;
        return TRUE;
    }
    return FALSE;
}

static void RouteWildTryPlayShinySparklesSound(void)
{
    u8 i;

    if (!RouteWildWanderingSystemIsEnabled())
        return;

    for (i = 0; i < sRouteWildActiveCount; i++)
    {
        if (sRouteWildIsShiny[i])
        {
            PlaySE(SE_SHINY);
            return;
        }
    }
}

bool8 RouteWildWanderingSystemIsEnabled(void)
{
    return !FlagGet(FLAG_SYS_ROUTE_WILD_WANDERERS_DISABLED);
}

static bool8 MapQualifiesForRouteWildWandering(void)
{
    u16 headerId;
    const struct WildPokemonHeader *header;

    if (!RouteWildWanderingSystemIsEnabled())
        return FALSE;
    if (IsOverworldLinkActive())
        return FALSE;
    if (InBattlePyramid() || InTrainerHill())
        return FALSE;
    if (gMapHeader.events == NULL)
        return FALSE;
    if (gMapHeader.mapType == MAP_TYPE_NONE)
        return FALSE;

    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return FALSE;

    header = &gWildMonHeaders[headerId];
    if (header->landMonsInfo == NULL && header->waterMonsInfo == NULL && header->fishingMonsInfo == NULL)
        return FALSE;

    return TRUE;
}

static s16 RouteWildMaxAbsS16(s16 a, s16 b)
{
    s16 aa = a < 0 ? (s16)-a : a;
    s16 bb = b < 0 ? (s16)-b : b;
    return aa > bb ? aa : bb;
}

static bool8 IsGrassSpawnTile(u8 metatileBehavior)
{
    if (MetatileBehavior_IsTallGrass(metatileBehavior)
     || MetatileBehavior_IsLongGrass(metatileBehavior)
     || MetatileBehavior_IsShortGrass(metatileBehavior)
     || MetatileBehavior_IsPokeGrass(metatileBehavior)
     || MetatileBehavior_IsAshGrass(metatileBehavior))
        return TRUE;
    return FALSE;
}

static u8 RouteWildCountRomObjectsNear(s16 tx, s16 ty, u8 romTemplateCount)
{
    u8 n = 0;
    u8 j;

    for (j = 0; j < romTemplateCount; j++)
    {
        s16 ox = gSaveBlock1Ptr->objectEventTemplates[j].x;
        s16 oy = gSaveBlock1Ptr->objectEventTemplates[j].y;

        if (RouteWildMaxAbsS16(tx - ox, ty - oy) <= ROUTE_WILD_CLUSTER_RADIUS)
            n++;
    }
    return n;
}

static bool8 RouteWildTileOccupiedByOtherObjectEvent(s16 tx, s16 ty, u8 mapTemplateCount,
                                                     const s16 *placedX, const s16 *placedY, u8 placedCount)
{
    u8 j;

    for (j = 0; j < mapTemplateCount; j++)
    {
        if (gSaveBlock1Ptr->objectEventTemplates[j].x == tx
         && gSaveBlock1Ptr->objectEventTemplates[j].y == ty)
            return TRUE;
    }
    for (j = 0; j < placedCount; j++)
    {
        if (placedX[j] == tx && placedY[j] == ty)
            return TRUE;
    }
    return FALSE;
}

static bool8 RouteWildTileOkForLandSpawn(s16 tx, s16 ty, u8 behavior, u8 romTemplateCount,
                                         const s16 *placedX, const s16 *placedY, u8 placedCount)
{
    if (MetatileBehavior_IsSurfableWaterOrUnderwater(behavior))
        return FALSE;
    if (MapGridGetCollisionAt(tx + MAP_OFFSET, ty + MAP_OFFSET))
        return FALSE;
    if (RouteWildTileOccupiedByOtherObjectEvent(tx, ty, romTemplateCount, placedX, placedY, placedCount))
        return FALSE;
    if (RouteWildCountRomObjectsNear(tx, ty, romTemplateCount) >= ROUTE_WILD_CLUSTER_MAX_ROM)
        return FALSE;
    return TRUE;
}

static bool8 RouteWildTileOkForWaterSpawn(s16 tx, s16 ty, u8 behavior, u8 romTemplateCount,
                                          const s16 *placedX, const s16 *placedY, u8 placedCount)
{
    if (!MetatileBehavior_IsSurfableWaterOrUnderwater(behavior))
        return FALSE;
    if (MapGridGetCollisionAt(tx + MAP_OFFSET, ty + MAP_OFFSET))
        return FALSE;
    if (RouteWildTileOccupiedByOtherObjectEvent(tx, ty, romTemplateCount, placedX, placedY, placedCount))
        return FALSE;
    if (RouteWildCountRomObjectsNear(tx, ty, romTemplateCount) >= ROUTE_WILD_CLUSTER_MAX_ROM)
        return FALSE;
    return TRUE;
}

static bool8 RouteWildIsWaterSpawnTile(u8 metatileBehavior)
{
    return MetatileBehavior_IsSurfableWaterOrUnderwater(metatileBehavior);
}

/* Single full-map pass — Setup previously ran three separate scans (land, water, grass). */
static void RouteWildCountEligibleTiles(u8 romTemplateCount, u32 *outLand, u32 *outWater, u32 *outGrass)
{
    s16 width = (s16)gMapHeader.mapLayout->width;
    s16 height = (s16)gMapHeader.mapLayout->height;
    s16 tx, ty;
    u32 land = 0, water = 0, grass = 0;
    u8 behavior;

    for (ty = 1; ty < height - 1; ty++)
    {
        for (tx = 1; tx < width - 1; tx++)
        {
            behavior = MapGridGetMetatileBehaviorAt(tx + MAP_OFFSET, ty + MAP_OFFSET);
            if (RouteWildTileOkForLandSpawn(tx, ty, behavior, romTemplateCount, NULL, NULL, 0))
            {
                land++;
                if (IsGrassSpawnTile(behavior))
                    grass++;
            }
            if (RouteWildTileOkForWaterSpawn(tx, ty, behavior, romTemplateCount, NULL, NULL, 0))
                water++;
        }
    }

    *outLand = land;
    *outWater = water;
    *outGrass = grass;
}

/*
 * Re-entering the same map repeats Setup; tile eligibility depends only on layout + ROM object count.
 * Cache counts per (map, wild header, romCount) to skip a full grid walk when unchanged.
 */
static u16 sRouteWildTileCacheMapKey;
static u16 sRouteWildTileCacheWildHeaderId;
static u8 sRouteWildTileCacheRomCount;
static u32 sRouteWildTileCacheLand;
static u32 sRouteWildTileCacheWater;
static u32 sRouteWildTileCacheGrass;

static void RouteWildCountEligibleTilesCached(u8 romTemplateCount, u32 *outLand, u32 *outWater, u32 *outGrass)
{
    u16 mapKey = RouteWildMapKey();
    u16 headerId = GetCurrentMapWildMonHeaderId();

    if (mapKey == sRouteWildTileCacheMapKey
     && headerId == sRouteWildTileCacheWildHeaderId
     && headerId != HEADER_NONE
     && romTemplateCount == sRouteWildTileCacheRomCount)
    {
        *outLand = sRouteWildTileCacheLand;
        *outWater = sRouteWildTileCacheWater;
        *outGrass = sRouteWildTileCacheGrass;
        return;
    }

    RouteWildCountEligibleTiles(romTemplateCount, outLand, outWater, outGrass);
    sRouteWildTileCacheMapKey = mapKey;
    sRouteWildTileCacheWildHeaderId = headerId;
    sRouteWildTileCacheRomCount = romTemplateCount;
    sRouteWildTileCacheLand = *outLand;
    sRouteWildTileCacheWater = *outWater;
    sRouteWildTileCacheGrass = *outGrass;
}

static bool8 RouteWildMeetsWandererSeparation(s16 tx, s16 ty,
                                              const s16 *placedX, const s16 *placedY, u8 placedCount,
                                              u8 minSep)
{
    u8 j;
    s16 dx, dy, ax, ay;

    if (minSep <= 1)
        return TRUE;

    for (j = 0; j < placedCount; j++)
    {
        dx = tx - placedX[j];
        dy = ty - placedY[j];
        ax = dx < 0 ? (s16)-dx : dx;
        ay = dy < 0 ? (s16)-dy : dy;
        if (RouteWildMaxAbsS16(ax, ay) < minSep)
            return FALSE;
    }
    return TRUE;
}

typedef bool8 (*RouteWildTilePredicate)(u8 metatileBehavior);

static bool8 RouteWildAcceptAnyLandTile(u8 metatileBehavior)
{
    (void)metatileBehavior;
    return TRUE;
}

static bool8 RouteWildAcceptCaveTile(u8 metatileBehavior)
{
    return metatileBehavior == MB_CAVE ? TRUE : FALSE;
}

static u16 RouteWildProximityHalfSpan(s16 width, s16 height)
{
    u16 interiorW = width > 2 ? (u16)(width - 2) : 1;
    u16 interiorH = height > 2 ? (u16)(height - 2) : 1;
    u16 maxDim = interiorW > interiorH ? interiorW : interiorH;

    if (maxDim <= 24)
        return maxDim;
    if (maxDim <= 40)
        return 14;
    return (u16)(12 + maxDim / 10);
}

static u8 RouteWildComputeDesiredCount(u8 romTemplateCount, u32 grassTiles)
{
    u8 room = (u8)(OBJECT_EVENT_TEMPLATES_COUNT - romTemplateCount);
    u8 want;
    u8 grassCap;

    if (room == 0 || romTemplateCount + ROUTE_WILD_MON_MAX > OBJECT_EVENT_TEMPLATES_COUNT)
        return 0;

    /* Few ROM objects → more wanderers; busy maps → fewer (fewer OBJ slots / palettes / tile scans). */
    if (romTemplateCount <= 8)
        want = (u8)(4 + (Random() % 3)); /* up to ROUTE_WILD_MON_MAX */
    else if (romTemplateCount <= 18)
        want = (u8)(3 + (Random() % 2));
    else
        want = 2;

    if (grassTiles > 0)
    {
        grassCap = (u8)(grassTiles / ROUTE_WILD_GRASS_TILES_PER_MON);
        if (grassCap < 1)
            grassCap = 1;
        if (grassCap > ROUTE_WILD_MON_MAX)
            grassCap = ROUTE_WILD_MON_MAX;
        if (want > grassCap)
            want = grassCap;
    }

    if (want > room)
        want = room;
    if (want > ROUTE_WILD_MON_MAX)
        want = ROUTE_WILD_MON_MAX;
    if (want == 0)
        want = 1;
    return want;
}

/*
 * Scans valid tiles; optionally restricted to a square around (centerX, centerY).
 * Picks one uniform random valid candidate.
 */
static bool8 TryPickSpawnOnLoadedMap(s16 *outX, s16 *outY,
                                     RouteWildTilePredicate tileWanted,
                                     u8 romTemplateCount,
                                     const s16 *placedX, const s16 *placedY, u8 placedCount,
                                     u8 minChebyshevSep,
                                     bool8 limitToRegion, s16 centerX, s16 centerY, u16 halfSpan,
                                     bool8 waterTiles)
{
    s16 width = (s16)gMapHeader.mapLayout->width;
    s16 height = (s16)gMapHeader.mapLayout->height;
    s16 tx, ty;
    u8 behavior;
    u32 count;
    u32 pick;
    s16 minX, maxX, minY, maxY;

    if (limitToRegion)
    {
        minX = centerX - (s16)halfSpan;
        maxX = centerX + (s16)halfSpan;
        minY = centerY - (s16)halfSpan;
        maxY = centerY + (s16)halfSpan;
        if (minX < 1)
            minX = 1;
        if (minY < 1)
            minY = 1;
        if (maxX > width - 2)
            maxX = width - 2;
        if (maxY > height - 2)
            maxY = height - 2;
    }
    else
    {
        minX = 1;
        minY = 1;
        maxX = width - 2;
        maxY = height - 2;
    }

    count = 0;
    for (ty = minY; ty <= maxY; ty++)
    {
        for (tx = minX; tx <= maxX; tx++)
        {
            behavior = MapGridGetMetatileBehaviorAt(tx + MAP_OFFSET, ty + MAP_OFFSET);
            if (!tileWanted(behavior))
                continue;
            if (waterTiles)
            {
                if (!RouteWildTileOkForWaterSpawn(tx, ty, behavior, romTemplateCount, placedX, placedY, placedCount))
                    continue;
            }
            else
            {
                if (!RouteWildTileOkForLandSpawn(tx, ty, behavior, romTemplateCount, placedX, placedY, placedCount))
                    continue;
            }
            if (!RouteWildMeetsWandererSeparation(tx, ty, placedX, placedY, placedCount, minChebyshevSep))
                continue;
            count++;
        }
    }

    if (count == 0)
        return FALSE;

    pick = Random() % count;

    for (ty = minY; ty <= maxY; ty++)
    {
        for (tx = minX; tx <= maxX; tx++)
        {
            behavior = MapGridGetMetatileBehaviorAt(tx + MAP_OFFSET, ty + MAP_OFFSET);
            if (!tileWanted(behavior))
                continue;
            if (waterTiles)
            {
                if (!RouteWildTileOkForWaterSpawn(tx, ty, behavior, romTemplateCount, placedX, placedY, placedCount))
                    continue;
            }
            else
            {
                if (!RouteWildTileOkForLandSpawn(tx, ty, behavior, romTemplateCount, placedX, placedY, placedCount))
                    continue;
            }
            if (!RouteWildMeetsWandererSeparation(tx, ty, placedX, placedY, placedCount, minChebyshevSep))
                continue;
            if (pick == 0)
            {
                *outX = tx;
                *outY = ty;
                return TRUE;
            }
            pick--;
        }
    }

    return FALSE;
}

static bool8 TryPickSpawnCategoryWithSpacing(s16 *outX, s16 *outY,
                                             RouteWildTilePredicate tileWanted,
                                             u8 romTemplateCount,
                                             const s16 *placedX, const s16 *placedY, u8 placedCount,
                                             bool8 limitToRegion, s16 centerX, s16 centerY, u16 halfSpan,
                                             bool8 waterTiles)
{
    u8 sep;

    for (sep = ROUTE_WILD_MAX_SPAWN_SEPARATION; sep >= 1; sep--)
    {
        if (TryPickSpawnOnLoadedMap(outX, outY, tileWanted, romTemplateCount, placedX, placedY, placedCount, sep,
                                      limitToRegion, centerX, centerY, halfSpan, waterTiles))
            return TRUE;
    }
    return FALSE;
}

static bool8 RouteWildTrySpawnNearPlayer(s16 *outX, s16 *outY,
                                        RouteWildTilePredicate tileWanted,
                                        u8 romTemplateCount,
                                        const s16 *placedX, const s16 *placedY, u8 placedCount,
                                        s16 px, s16 py,
                                        bool8 waterTiles)
{
    u16 half = RouteWildProximityHalfSpan((s16)gMapHeader.mapLayout->width, (s16)gMapHeader.mapLayout->height);
    u16 expand;

    for (expand = 0; expand <= (u16)(half + 24); expand += 6)
    {
        u16 span = half + expand;

        if (TryPickSpawnCategoryWithSpacing(outX, outY, tileWanted, romTemplateCount, placedX, placedY, placedCount,
                                            TRUE, px, py, span, waterTiles))
            return TRUE;
    }
    return FALSE;
}

static bool8 RouteWildPickLandSpawnForSlot(s16 *outX, s16 *outY,
                                           u8 romTemplateCount,
                                           const s16 *placedX, const s16 *placedY, u8 placedCount,
                                           s16 px, s16 py)
{
    if (RouteWildTrySpawnNearPlayer(outX, outY, IsGrassSpawnTile, romTemplateCount, placedX, placedY, placedCount, px, py, FALSE))
        return TRUE;
    if (RouteWildTrySpawnNearPlayer(outX, outY, RouteWildAcceptCaveTile, romTemplateCount, placedX, placedY, placedCount, px, py, FALSE))
        return TRUE;
    if (RouteWildTrySpawnNearPlayer(outX, outY, RouteWildAcceptAnyLandTile, romTemplateCount, placedX, placedY, placedCount, px, py, FALSE))
        return TRUE;

    if (TryPickSpawnCategoryWithSpacing(outX, outY, IsGrassSpawnTile, romTemplateCount, placedX, placedY, placedCount,
                                        FALSE, 0, 0, 0, FALSE))
        return TRUE;
    if (TryPickSpawnCategoryWithSpacing(outX, outY, RouteWildAcceptCaveTile, romTemplateCount, placedX, placedY, placedCount,
                                        FALSE, 0, 0, 0, FALSE))
        return TRUE;
    if (TryPickSpawnCategoryWithSpacing(outX, outY, RouteWildAcceptAnyLandTile, romTemplateCount, placedX, placedY, placedCount,
                                        FALSE, 0, 0, 0, FALSE))
        return TRUE;
    return FALSE;
}

static bool8 RouteWildPickWaterSpawnForSlot(s16 *outX, s16 *outY,
                                            u8 romTemplateCount,
                                            const s16 *placedX, const s16 *placedY, u8 placedCount,
                                            s16 px, s16 py)
{
    if (RouteWildTrySpawnNearPlayer(outX, outY, RouteWildIsWaterSpawnTile, romTemplateCount, placedX, placedY, placedCount, px, py, TRUE))
        return TRUE;

    if (TryPickSpawnCategoryWithSpacing(outX, outY, RouteWildIsWaterSpawnTile, romTemplateCount, placedX, placedY, placedCount,
                                        FALSE, 0, 0, 0, TRUE))
        return TRUE;
    return FALSE;
}

static bool8 RouteWildPickSpawnForSlotKind(u8 slotIdx,
                                           s16 *outX, s16 *outY,
                                           u8 romTemplateCount,
                                           const s16 *placedX, const s16 *placedY, u8 placedCount,
                                           s16 px, s16 py)
{
    if (sRouteWildSlotKind[slotIdx] == ROUTE_WILD_KIND_LAND)
        return RouteWildPickLandSpawnForSlot(outX, outY, romTemplateCount, placedX, placedY, placedCount, px, py);
    return RouteWildPickWaterSpawnForSlot(outX, outY, romTemplateCount, placedX, placedY, placedCount, px, py);
}

static u8 RouteWildElevationAt(s16 templateX, s16 templateY)
{
    u8 elevation;

    elevation = MapGridGetElevationAt(templateX + MAP_OFFSET, templateY + MAP_OFFSET);
    if (elevation != 0)
        return elevation;

    if (gPlayerAvatar.objectEventId < OBJECT_EVENTS_COUNT
        && gObjectEvents[gPlayerAvatar.objectEventId].active)
        return PlayerGetElevation();

    return 3;
}

static void RouteWildResetOverlapLatch(void)
{
    sRouteWildPrevOverlapObjId = OBJECT_EVENTS_COUNT;
}

/* Match engine object-object elevation rules (see AreElevationsCompatible). */
static bool8 RouteWildElevationsCompatible(u8 a, u8 b)
{
    if (a == 0 || b == 0)
        return TRUE;
    return a == b;
}

static u8 RouteWildGetOverlapObjectEventId(void)
{
    u8 i;
    s16 px = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x;
    s16 py = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *obj = &gObjectEvents[i];

        if (!obj->active || obj->isPlayer)
            continue;
        if (obj->localId < LOCALID_ROUTE_WILD_FIRST || obj->localId > LOCALID_ROUTE_WILD_LAST)
            continue;
        if (obj->currentCoords.x != px || obj->currentCoords.y != py)
            continue;
        /*
         * PlayerGetElevation() uses previousElevation; after a step it can disagree with
         * currentElevation for a frame. Accept overlap if either matches the wild mon.
         */
        if (!RouteWildElevationsCompatible(obj->currentElevation, player->currentElevation)
         && !RouteWildElevationsCompatible(obj->currentElevation, player->previousElevation))
            continue;
        return i;
    }
    return OBJECT_EVENTS_COUNT;
}

bool8 TryRouteWildWanderingOverlapBattle(void)
{
    u8 objEventId;
    struct ObjectEvent *obj;

    if (!RouteWildWanderingSystemIsEnabled())
        return FALSE;
    if (!sRouteWildWanderingActive)
        return FALSE;
    if (ScriptContext_IsEnabled())
        return FALSE;
    if (gPlayerAvatar.objectEventId >= OBJECT_EVENTS_COUNT)
        return FALSE;

    objEventId = RouteWildGetOverlapObjectEventId();
    if (objEventId == OBJECT_EVENTS_COUNT)
    {
        sRouteWildPrevOverlapObjId = OBJECT_EVENTS_COUNT;
        return FALSE;
    }

    if (objEventId == sRouteWildPrevOverlapObjId)
        return FALSE;

    obj = &gObjectEvents[objEventId];

    sRouteWildPrevOverlapObjId = objEventId;
    gSelectedObjectEvent = objEventId;
    gSpecialVar_LastTalked = obj->localId;
    ScriptContext_SetupScript(RouteWildWandering_EventScript);
    return TRUE;
}

static u8 RouteWildPickWetKindOnly(bool8 hasW, bool8 hasF)
{
    if (hasW && hasF)
        return (Random() & 1) ? ROUTE_WILD_KIND_WATER : ROUTE_WILD_KIND_FISHING;
    if (hasW)
        return ROUTE_WILD_KIND_WATER;
    return ROUTE_WILD_KIND_FISHING;
}

/* Biases land vs wet slot rolls when Surf is not obtained but water tiles dwarf grass (player still on land). */
static u8 RouteWildEffectiveLandRollPercent(u8 geographicLandPercent, u32 waterTiles, u32 grassTiles)
{
    if (FlagGet(FLAG_RECEIVED_HM_SURF))
        return geographicLandPercent;
    if (grassTiles == 0)
        return geographicLandPercent;
    if (waterTiles < grassTiles * ROUTE_WILD_NO_SURF_WATER_VS_GRASS_RATIO)
        return geographicLandPercent;

    if (geographicLandPercent < ROUTE_WILD_NO_SURF_MIN_LAND_ROLL_PCT)
        return ROUTE_WILD_NO_SURF_MIN_LAND_ROLL_PCT;
    return geographicLandPercent;
}

static void RouteWildAssignSlotKinds(u16 headerId, u8 count, u8 romTemplateCount, u32 landTiles, u32 waterTiles, u32 grassTiles)
{
    const struct WildPokemonHeader *h = &gWildMonHeaders[headerId];
    bool8 hasL = h->landMonsInfo != NULL;
    bool8 hasW = h->waterMonsInfo != NULL;
    bool8 hasF = h->fishingMonsInfo != NULL;
    u32 totalTiles = landTiles + waterTiles;
    u8 landPercent;
    u8 rollLandPercent;
    u8 i;

    if (totalTiles == 0)
        landPercent = 50;
    else
        landPercent = (u8)((landTiles * 100) / totalTiles);

    rollLandPercent = RouteWildEffectiveLandRollPercent(landPercent, waterTiles, grassTiles);

    for (i = 0; i < count; i++)
    {
        u8 roll = (u8)(Random() % 100);

        if (hasL && hasW && hasF)
        {
            if (landTiles == 0 && waterTiles > 0)
                sRouteWildSlotKind[i] = RouteWildPickWetKindOnly(hasW, hasF);
            else if (waterTiles == 0 && landTiles > 0)
                sRouteWildSlotKind[i] = ROUTE_WILD_KIND_LAND;
            else if (roll < rollLandPercent)
                sRouteWildSlotKind[i] = ROUTE_WILD_KIND_LAND;
            else
                sRouteWildSlotKind[i] = RouteWildPickWetKindOnly(hasW, hasF);
        }
        else if (hasL && hasW)
        {
            if (landTiles == 0 && waterTiles > 0)
                sRouteWildSlotKind[i] = ROUTE_WILD_KIND_WATER;
            else if (waterTiles == 0 && landTiles > 0)
                sRouteWildSlotKind[i] = ROUTE_WILD_KIND_LAND;
            else
                sRouteWildSlotKind[i] = (roll < rollLandPercent) ? ROUTE_WILD_KIND_LAND : ROUTE_WILD_KIND_WATER;
        }
        else if (hasL && hasF)
        {
            if (landTiles == 0 && waterTiles > 0)
                sRouteWildSlotKind[i] = ROUTE_WILD_KIND_FISHING;
            else if (waterTiles == 0 && landTiles > 0)
                sRouteWildSlotKind[i] = ROUTE_WILD_KIND_LAND;
            else
                sRouteWildSlotKind[i] = (roll < rollLandPercent) ? ROUTE_WILD_KIND_LAND : ROUTE_WILD_KIND_FISHING;
        }
        else if (hasW && hasF)
        {
            sRouteWildSlotKind[i] = RouteWildPickWetKindOnly(TRUE, TRUE);
        }
        else if (hasL)
        {
            sRouteWildSlotKind[i] = ROUTE_WILD_KIND_LAND;
        }
        else if (hasW)
        {
            sRouteWildSlotKind[i] = ROUTE_WILD_KIND_WATER;
        }
        else
        {
            sRouteWildSlotKind[i] = ROUTE_WILD_KIND_FISHING;
        }
    }

    if (hasW && waterTiles > 0 && landPercent >= ROUTE_WILD_FORCE_WATER_LANDPCT)
    {
        bool8 hasWaterSlot = FALSE;

        for (i = 0; i < count; i++)
        {
            if (sRouteWildSlotKind[i] == ROUTE_WILD_KIND_WATER)
            {
                hasWaterSlot = TRUE;
                break;
            }
        }
        if (!hasWaterSlot && count > 0)
            sRouteWildSlotKind[Random() % count] = ROUTE_WILD_KIND_WATER;
    }
}

static u8 RouteWildChooseLandIndexForWanderer(void)
{
    if (LAND_WILD_COUNT >= 3 && (Random() % 100) < ROUTE_WILD_RARE_LAND_BIAS_PERCENT)
        return (u8)(LAND_WILD_COUNT - 3 + (Random() % 3));
    return ChooseWildMonIndex_Land();
}

static u8 RouteWildChooseWaterIndexForWanderer(void)
{
    if (WATER_WILD_COUNT >= 1 && (Random() % 100) < ROUTE_WILD_RARE_WATER_BIAS_PERCENT)
        return (u8)(WATER_WILD_COUNT - 1);
    return ChooseWildMonIndex_WaterRock();
}

/* Matches CreateBoxMon player-OT shiny logic; rerolls only Shiny Charm (not lure / fishing / DexNav). */
static void RouteWildRollEncounterPersonality(u8 slotIdx)
{
    u32 value;
    u32 personality;
    u32 totalRerolls;
    bool32 isShiny;

    value = gSaveBlock2Ptr->playerTrainerId[0]
          | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
          | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
          | (gSaveBlock2Ptr->playerTrainerId[3] << 24);

    personality = Random32();

#if P_FLAG_FORCE_NO_SHINY != 0
    if (FlagGet(P_FLAG_FORCE_NO_SHINY))
    {
        sRouteWildPersonality[slotIdx] = personality;
        sRouteWildIsShiny[slotIdx] = FALSE;
        return;
    }
#endif
#if P_FLAG_FORCE_SHINY != 0
    if (FlagGet(P_FLAG_FORCE_SHINY))
    {
        sRouteWildPersonality[slotIdx] = personality;
        sRouteWildIsShiny[slotIdx] = TRUE;
        return;
    }
#endif
    if (P_ONLY_OBTAINABLE_SHINIES && InBattlePyramid())
    {
        sRouteWildPersonality[slotIdx] = personality;
        sRouteWildIsShiny[slotIdx] = FALSE;
        return;
    }
    if (P_NO_SHINIES_WITHOUT_POKEBALLS && !HasAtLeastOnePokeBall())
    {
        sRouteWildPersonality[slotIdx] = personality;
        sRouteWildIsShiny[slotIdx] = FALSE;
        return;
    }

    totalRerolls = 0;
    if (CheckBagHasItem(ITEM_SHINY_CHARM, 1))
        totalRerolls += I_SHINY_CHARM_ADDITIONAL_ROLLS;

    while (GET_SHINY_VALUE(value, personality) >= SHINY_ODDS && totalRerolls > 0)
    {
        personality = Random32();
        totalRerolls--;
    }

    isShiny = GET_SHINY_VALUE(value, personality) < SHINY_ODDS;
    sRouteWildPersonality[slotIdx] = personality;
    sRouteWildIsShiny[slotIdx] = isShiny;
}

static u16 RouteWildGraphicsIdForSlot(u8 i)
{
    u16 gfx = (u16)(sRouteWildSpecies[i] + OBJ_EVENT_MON);

    if (sRouteWildIsShiny[i])
        gfx |= OBJ_EVENT_MON_SHINY;
    return gfx;
}

static void RouteWildCreateEnemyMonForBattle(u8 idx)
{
    u32 value = gSaveBlock2Ptr->playerTrainerId[0]
              | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
              | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
              | (gSaveBlock2Ptr->playerTrainerId[3] << 24);
    u32 personality;
    u16 attempt;

    ZeroEnemyPartyMons();

#if OW_SYNCHRONIZE_NATURE > GEN_3
    {
        u8 nature = PickWildMonNature();

        for (attempt = 0; attempt < 512; attempt++)
        {
            personality = Random32();
            if (GetNatureFromPersonality(personality) != nature)
                continue;
            if (!!(GET_SHINY_VALUE(value, personality) < SHINY_ODDS) != !!sRouteWildIsShiny[idx])
                continue;

            CreateMon(&gEnemyParty[0], sRouteWildSpecies[idx], sRouteWildLevels[idx], USE_RANDOM_IVS, TRUE, personality, OT_ID_PLAYER_ID, 0);
            return;
        }
        CreateMon(&gEnemyParty[0], sRouteWildSpecies[idx], sRouteWildLevels[idx], USE_RANDOM_IVS, TRUE, sRouteWildPersonality[idx], OT_ID_PLAYER_ID, 0);
    }
#else
    CreateMon(&gEnemyParty[0], sRouteWildSpecies[idx], sRouteWildLevels[idx], USE_RANDOM_IVS, TRUE, sRouteWildPersonality[idx], OT_ID_PLAYER_ID, 0);
#endif
}

static void RouteWildRollSpeciesAndLevelForSlot(u8 i, u16 headerId)
{
    const struct WildPokemonHeader *h = &gWildMonHeaders[headerId];
    u8 idx;
    const struct WildPokemon *wp;
    const struct WildPokemonInfo *info;

    switch (sRouteWildSlotKind[i])
    {
    default:
    case ROUTE_WILD_KIND_LAND:
        info = h->landMonsInfo;
        idx = RouteWildChooseLandIndexForWanderer();
        wp = &info->wildPokemon[idx];
        sRouteWildSpecies[i] = wp->species;
        sRouteWildLevels[i] = GetLandWildMonEncounterLevel(info->wildPokemon, idx);
        break;
    case ROUTE_WILD_KIND_WATER:
        info = h->waterMonsInfo;
        idx = RouteWildChooseWaterIndexForWanderer();
        wp = &info->wildPokemon[idx];
        sRouteWildSpecies[i] = wp->species;
        sRouteWildLevels[i] = GetWaterWildMonEncounterLevel(info->wildPokemon, idx);
        break;
    case ROUTE_WILD_KIND_FISHING:
        info = h->fishingMonsInfo;
        idx = ChooseWildMonIndex_FishingRandomRod();
        wp = &info->wildPokemon[idx];
        sRouteWildSpecies[i] = wp->species;
        sRouteWildLevels[i] = GetFishingWildMonEncounterLevel(info->wildPokemon, idx);
        break;
    }

    RouteWildRollEncounterPersonality(i);

    if (sRouteWildIsShiny[i])
        RouteWildPersistSaveShiny(i, sRouteWildSpecies[i], sRouteWildPersonality[i], sRouteWildSlotKind[i], sRouteWildLevels[i]);
}

void SetupRouteWildWanderingMons(void)
{
    u8 i;
    u8 romCount;
    u16 headerId;
    s16 chosenX[ROUTE_WILD_MON_MAX];
    s16 chosenY[ROUTE_WILD_MON_MAX];
    s16 px, py;
    u32 landTiles, waterTiles, grassTiles;

    sRouteWildHiddenForScriptMovement = FALSE;
    RouteWildResetOverlapLatch();
    sRouteWildWanderingActive = FALSE;
    sRouteWildActiveCount = 0;

    if (!MapQualifiesForRouteWildWandering())
        return;

    romCount = gMapHeader.events->objectEventCount;
    RouteWildCountEligibleTilesCached(romCount, &landTiles, &waterTiles, &grassTiles);
    sRouteWildActiveCount = RouteWildComputeDesiredCount(romCount, grassTiles);
    if (sRouteWildActiveCount == 0)
        return;
    if ((u16)romCount + sRouteWildActiveCount > OBJECT_EVENT_TEMPLATES_COUNT)
        return;

    headerId = GetCurrentMapWildMonHeaderId();

    RouteWildAssignSlotKinds(headerId, sRouteWildActiveCount, romCount, landTiles, waterTiles, grassTiles);

    for (i = 0; i < sRouteWildActiveCount; i++)
    {
        if (!RouteWildPersistTryLoadShinySlot(i))
            RouteWildRollSpeciesAndLevelForSlot(i, headerId);
    }

    RouteWildTryPlayShinySparklesSound();

    if (gPlayerAvatar.objectEventId < OBJECT_EVENTS_COUNT && gObjectEvents[gPlayerAvatar.objectEventId].active)
    {
        px = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x - MAP_OFFSET;
        py = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y - MAP_OFFSET;
    }
    else
    {
        px = gSaveBlock1Ptr->pos.x;
        py = gSaveBlock1Ptr->pos.y;
    }

    for (i = 0; i < sRouteWildActiveCount; i++)
    {
        struct ObjectEventTemplate *t = &gSaveBlock1Ptr->objectEventTemplates[romCount + i];
        s16 tx, ty;

        if (RouteWildPickSpawnForSlotKind(i, &chosenX[i], &chosenY[i], romCount, chosenX, chosenY, i, px, py))
        {
            tx = chosenX[i];
            ty = chosenY[i];
        }
        else if (sRouteWildSlotKind[i] == ROUTE_WILD_KIND_LAND)
        {
            tx = px + sSpawnOffsets[i][0];
            ty = py + sSpawnOffsets[i][1];

            if (tx < 1)
                tx = 1;
            if (ty < 1)
                ty = 1;
            if (tx >= gMapHeader.mapLayout->width - 1)
                tx = gMapHeader.mapLayout->width - 2;
            if (ty >= gMapHeader.mapLayout->height - 1)
                ty = gMapHeader.mapLayout->height - 2;
            chosenX[i] = tx;
            chosenY[i] = ty;
        }
        else
        {
            tx = px;
            ty = py;
            if (tx < 1)
                tx = 1;
            if (ty < 1)
                ty = 1;
            if (tx >= gMapHeader.mapLayout->width - 1)
                tx = gMapHeader.mapLayout->width - 2;
            if (ty >= gMapHeader.mapLayout->height - 1)
                ty = gMapHeader.mapLayout->height - 2;
            chosenX[i] = tx;
            chosenY[i] = ty;
        }

        t->localId = sRouteWildLocalIds[i];
        t->graphicsId = RouteWildGraphicsIdForSlot(i);
        t->kind = OBJ_KIND_NORMAL;
        t->x = tx;
        t->y = ty;
        t->elevation = RouteWildElevationAt(tx, ty);
        t->movementType = MOVEMENT_TYPE_WANDER_AROUND;
        t->movementRangeX = 3;
        t->movementRangeY = 3;
        t->trainerType = TRAINER_TYPE_NONE;
        t->trainerRange_berryTreeId = 0;
        t->script = RouteWildWandering_EventScript;
        t->flagId = sRouteWildFlags[i];
        t->filler = 0;
    }

    sRouteWildWanderingActive = TRUE;
}

void RouteWildScheduleDeferredSetup(void)
{
    if (!RouteWildWanderingSystemIsEnabled())
        return;
    sRouteWildDeferredSetupPending = TRUE;
}

void RouteWildProcessDeferredSetup(void)
{
    if (!sRouteWildDeferredSetupPending)
        return;

    /* Do not build or spawn wanderers while the player is locked (events, warps, etc.). */
    if (ArePlayerFieldControlsLocked())
        return;

    sRouteWildDeferredSetupPending = FALSE;
    SetupRouteWildWanderingMons();
    TrySpawnObjectEvents(0, 0);
}

static void RouteWildHideForScript(void)
{
    u8 i;
    u8 mapNum = gSaveBlock1Ptr->location.mapNum;
    u8 mapGroup = gSaveBlock1Ptr->location.mapGroup;

    if (!sRouteWildWanderingActive || sRouteWildActiveCount == 0)
        return;

    /* Hide without RemoveObjectEvent — CB2 runs this before scripts; removal broke Prepare (slot cleared). */
    for (i = 0; i < sRouteWildActiveCount; i++)
    {
        FlagSet(sRouteWildFlags[i]);
        SetObjectInvisibility(sRouteWildLocalIds[i], mapNum, mapGroup, TRUE);
    }
}

static void RouteWildShowAfterScript(void)
{
    u8 i;
    u8 mapNum = gSaveBlock1Ptr->location.mapNum;
    u8 mapGroup = gSaveBlock1Ptr->location.mapGroup;

    if (sRouteWildWanderingActive && sRouteWildActiveCount != 0)
    {
        for (i = 0; i < sRouteWildActiveCount; i++)
        {
            FlagClear(sRouteWildFlags[i]);
            SetObjectInvisibility(sRouteWildLocalIds[i], mapNum, mapGroup, FALSE);
        }
    }
    TrySpawnObjectEvents(0, 0);
}

void RouteWildNotifyScriptMovementBegin(void)
{
    if (!RouteWildWanderingSystemIsEnabled())
        return;
    if (!sRouteWildWanderingActive || sRouteWildActiveCount == 0)
        return;
    if (sRouteWildHiddenForScriptMovement)
        return;

    RouteWildHideForScript();
    sRouteWildHiddenForScriptMovement = TRUE;
}

void RouteWildNotifyScriptMovementEnd(void)
{
    if (!sRouteWildHiddenForScriptMovement)
        return;
    sRouteWildHiddenForScriptMovement = FALSE;
    RouteWildShowAfterScript();
}

/*
 * Vanilla NPC collision alone lets wanderers onto shallow shore water (no TILE_FLAG_SURFABLE),
 * thin decorations without collision bits, etc. Match spawn rules + directional tree edges.
 */
static bool8 RouteWildLandMetatileBlocksWanderStep(u8 behavior)
{
    if (MetatileBehavior_IsSurfableWaterOrUnderwater(behavior))
        return TRUE;
    if (MetatileBehavior_IsShallowFlowingWater(behavior))
        return TRUE;
    if (MetatileBehavior_IsDeepOrOceanWater(behavior))
        return TRUE;
    if (MetatileBehavior_IsWaterfall(behavior))
        return TRUE;
    if (MetatileBehavior_IsDiveable(behavior))
        return TRUE;
    if (MetatileBehavior_IsMountain(behavior))
        return TRUE;
    if (MetatileBehavior_IsSecretBaseTree(behavior))
        return TRUE;
    if (MetatileBehavior_IsSecretBaseImpassable(behavior))
        return TRUE;
    if (MetatileBehavior_IsJumpEast(behavior)
        || MetatileBehavior_IsJumpWest(behavior)
        || MetatileBehavior_IsJumpNorth(behavior)
        || MetatileBehavior_IsJumpSouth(behavior))
        return TRUE;
    if (MetatileBehavior_HasRipples(behavior) && !MetatileBehavior_IsPuddle(behavior))
        return TRUE;
    if (MetatileBehavior_IsNorthwardCurrent(behavior)
        || MetatileBehavior_IsSouthwardCurrent(behavior)
        || MetatileBehavior_IsWestwardCurrent(behavior)
        || MetatileBehavior_IsEastwardCurrent(behavior))
        return TRUE;
    return FALSE;
}

static bool8 RouteWildWaterMetatileBlocksWanderStep(u8 behavior)
{
    if (!MetatileBehavior_IsSurfableWaterOrUnderwater(behavior))
        return TRUE;
    if (MetatileBehavior_IsMountain(behavior))
        return TRUE;
    return FALSE;
}

bool8 RouteWildObjectEventCanTakeWanderStep(struct ObjectEvent *objectEvent, u8 direction)
{
    u8 slot;
    s16 x, y;
    u8 behavior;

    if (!sRouteWildWanderingActive)
        return TRUE;
    if (!IS_OW_MON_OBJ(objectEvent))
        return TRUE;
    if (objectEvent->localId < LOCALID_ROUTE_WILD_FIRST || objectEvent->localId > LOCALID_ROUTE_WILD_LAST)
        return TRUE;

    slot = objectEvent->localId - LOCALID_ROUTE_WILD_FIRST;
    if (slot >= sRouteWildActiveCount)
        return TRUE;

    x = objectEvent->currentCoords.x;
    y = objectEvent->currentCoords.y;
    MoveCoords(direction, &x, &y);

    if (IsMetatileDirectionallyImpassable(objectEvent, x, y, direction))
        return FALSE;

    behavior = MapGridGetMetatileBehaviorAt(x, y);

    switch (sRouteWildSlotKind[slot])
    {
    case ROUTE_WILD_KIND_LAND:
        if (MapGridGetCollisionAt(x, y))
            return FALSE;
        if (RouteWildLandMetatileBlocksWanderStep(behavior))
            return FALSE;
        return TRUE;
    case ROUTE_WILD_KIND_WATER:
    case ROUTE_WILD_KIND_FISHING:
        if (MapGridGetCollisionAt(x, y))
            return FALSE;
        if (RouteWildWaterMetatileBlocksWanderStep(behavior))
            return FALSE;
        return TRUE;
    default:
        return TRUE;
    }
}

u8 GetCurrentMapObjectEventTemplateCount(void)
{
    if (gMapHeader.events == NULL)
        return 0;

    return gMapHeader.events->objectEventCount + (sRouteWildWanderingActive ? sRouteWildActiveCount : 0);
}

void RouteWildWandering_OnCameraUpdate(void)
{
    static u8 sThrottle;
    u8 i;
    u8 romCount;
    u16 headerId;
    s16 px, py;
    u8 mapNum = gSaveBlock1Ptr->location.mapNum;
    u8 mapGroup = gSaveBlock1Ptr->location.mapGroup;

    if (!RouteWildWanderingSystemIsEnabled())
        return;
    if (!sRouteWildWanderingActive || sRouteWildActiveCount == 0)
        return;
    if (!MapQualifiesForRouteWildWandering())
        return;

    if (++sThrottle < ROUTE_WILD_CAMERA_UPDATE_INTERVAL)
        return;
    sThrottle = 0;

    if (gPlayerAvatar.objectEventId >= OBJECT_EVENTS_COUNT || !gObjectEvents[gPlayerAvatar.objectEventId].active)
        return;

    px = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x - MAP_OFFSET;
    py = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y - MAP_OFFSET;

    romCount = gMapHeader.events->objectEventCount;
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return;

    for (i = 0; i < sRouteWildActiveCount; i++)
    {
        struct ObjectEventTemplate *t = &gSaveBlock1Ptr->objectEventTemplates[romCount + i];
        u8 localId = sRouteWildLocalIds[i];
        u8 objId;
        s16 wx, wy;
        s16 dist;
        s16 newX, newY;
        s16 placedX[ROUTE_WILD_MON_MAX];
        s16 placedY[ROUTE_WILD_MON_MAX];
        u8 j, placedCount;

        if (!TryGetObjectEventIdByLocalIdAndMap(localId, mapNum, mapGroup, &objId))
        {
            wx = gObjectEvents[objId].currentCoords.x - MAP_OFFSET;
            wy = gObjectEvents[objId].currentCoords.y - MAP_OFFSET;
        }
        else
        {
            wx = t->x;
            wy = t->y;
        }

        dist = RouteWildMaxAbsS16(wx - px, wy - py);
        if (dist <= ROUTE_WILD_RELOCATE_DISTANCE)
            continue;

        if (sRouteWildIsShiny[i])
            continue;

        RouteWildRollSpeciesAndLevelForSlot(i, headerId);

        placedCount = 0;
        for (j = 0; j < sRouteWildActiveCount; j++)
        {
            if (j == i)
                continue;
            placedX[placedCount] = gSaveBlock1Ptr->objectEventTemplates[romCount + j].x;
            placedY[placedCount] = gSaveBlock1Ptr->objectEventTemplates[romCount + j].y;
            placedCount++;
        }

        if (!RouteWildPickSpawnForSlotKind(i, &newX, &newY, romCount, placedX, placedY, placedCount, px, py))
            continue;

        t->graphicsId = RouteWildGraphicsIdForSlot(i);
        t->x = newX;
        t->y = newY;
        t->elevation = RouteWildElevationAt(newX, newY);

        FlagClear(sRouteWildFlags[i]);
        RemoveObjectEventByLocalIdNoFlag(localId, mapNum, mapGroup);
        TrySpawnObjectEvent(localId, mapNum, mapGroup);
    }
}

void Special_PrepareRouteWildScriptedMon(void)
{
    struct ObjectEvent *obj;
    u8 idx;
    u8 localId = gSpecialVar_LastTalked;

    if (!RouteWildWanderingSystemIsEnabled())
        return;
    if (!sRouteWildWanderingActive)
        return;

    if (localId >= LOCALID_ROUTE_WILD_FIRST && localId <= LOCALID_ROUTE_WILD_LAST)
    {
        idx = localId - LOCALID_ROUTE_WILD_FIRST;
        if (idx < sRouteWildActiveCount)
        {
            RouteWildCreateEnemyMonForBattle(idx);
            return;
        }
    }

    obj = &gObjectEvents[gSelectedObjectEvent];
    idx = obj->localId - LOCALID_ROUTE_WILD_FIRST;
    if (idx >= sRouteWildActiveCount)
        return;
    if (!IS_OW_MON_OBJ(obj))
        return;

    RouteWildCreateEnemyMonForBattle(idx);
}

void Special_RemoveRouteWildMonAfterBattle(void)
{
    u8 localId = gSpecialVar_LastTalked;

    if (localId < LOCALID_ROUTE_WILD_FIRST || localId > LOCALID_ROUTE_WILD_LAST)
        localId = gObjectEvents[gSelectedObjectEvent].localId;

    RouteWildPersistClearByLocalId(localId);
    RemoveObjectEventByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
}

bool8 RouteWildObjectEventIsProtectedShiny(struct ObjectEvent *objectEvent)
{
    if (!RouteWildWanderingSystemIsEnabled())
        return FALSE;
    if (!IS_OW_MON_OBJ(objectEvent))
        return FALSE;
    if (!OW_SHINY(objectEvent))
        return FALSE;
    if (objectEvent->localId < LOCALID_ROUTE_WILD_FIRST || objectEvent->localId > LOCALID_ROUTE_WILD_LAST)
        return FALSE;
    if (objectEvent->mapNum != gSaveBlock1Ptr->location.mapNum
     || objectEvent->mapGroup != gSaveBlock1Ptr->location.mapGroup)
        return FALSE;
    return TRUE;
}

void RouteWildPersistClearByLocalId(u8 localId)
{
    u8 slot;

    if (!RouteWildWanderingSystemIsEnabled())
        return;
    if (localId < LOCALID_ROUTE_WILD_FIRST || localId > LOCALID_ROUTE_WILD_LAST)
        return;

    slot = localId - LOCALID_ROUTE_WILD_FIRST;
    RouteWildPersistClearSlot(RouteWildMapKey(), slot);
}

bool8 TryStartRouteWildWanderingBattleOnBump(u8 direction)
{
    s16 x, y;
    u8 objEventId;
    struct ObjectEvent *obj;

    if (!RouteWildWanderingSystemIsEnabled())
        return FALSE;
    if (!sRouteWildWanderingActive)
        return FALSE;

    x = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x;
    y = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;
    MoveCoords(direction, &x, &y);

    objEventId = GetObjectEventIdByPosition(x, y, PlayerGetElevation());
    if (objEventId == OBJECT_EVENTS_COUNT)
        return FALSE;

    obj = &gObjectEvents[objEventId];
    if (obj->localId < LOCALID_ROUTE_WILD_FIRST || obj->localId > LOCALID_ROUTE_WILD_LAST)
        return FALSE;
    if (!IS_OW_MON_OBJ(obj))
        return FALSE;

    gSelectedObjectEvent = objEventId;
    gSpecialVar_LastTalked = obj->localId;
    ScriptContext_SetupScript(RouteWildWandering_EventScript);
    return TRUE;
}

#else // !OW_POKEMON_OBJECT_EVENTS

bool8 RouteWildWanderingSystemIsEnabled(void)
{
    return FALSE;
}

void RouteWildScheduleDeferredSetup(void)
{
}

void RouteWildProcessDeferredSetup(void)
{
}

void RouteWildNotifyScriptMovementBegin(void)
{
}

void RouteWildNotifyScriptMovementEnd(void)
{
}

void SetupRouteWildWanderingMons(void)
{
}

void RouteWildWandering_OnCameraUpdate(void)
{
}

u8 GetCurrentMapObjectEventTemplateCount(void)
{
    if (gMapHeader.events == NULL)
        return 0;
    return gMapHeader.events->objectEventCount;
}

void Special_PrepareRouteWildScriptedMon(void)
{
}

void Special_RemoveRouteWildMonAfterBattle(void)
{
}

bool8 TryStartRouteWildWanderingBattleOnBump(u8 direction)
{
    return FALSE;
}

bool8 TryRouteWildWanderingOverlapBattle(void)
{
    return FALSE;
}

bool8 RouteWildObjectEventIsProtectedShiny(struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
    return FALSE;
}

void RouteWildPersistClearByLocalId(u8 localId)
{
    (void)localId;
}

#endif // OW_POKEMON_OBJECT_EVENTS

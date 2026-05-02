#ifndef GUARD_ROUTE_WILD_WANDERERS_H
#define GUARD_ROUTE_WILD_WANDERERS_H

#include "global.h"
#include "config/overworld.h"

/*
 * Route wandering wild Pokémon append ObjectEventTemplate slots in the *save block only*
 * after the map’s ROM templates (indices [0, objectEventCount)); they do not modify map JSON,
 * ROM header pointers, or existing template fields (local IDs, scripts, flags) for NPCs.
 *
 * Encounter pools: land (tall grass / cave / land), surf water (waterMons on surfable water only),
 * fishing (fishingMons on surfable water only, rod weights via ChooseWildMonIndex_FishingRandomRod).
 * Map type is unrestricted if the map has wild data and at least one of land / water / fishing.
 *
 * Reserved local IDs 242–247 (up to ROUTE_WILD_MON_MAX) — map objects normally use low IDs;
 * do not assign these in map data. LOAD_PATH: LoadSaveblockObjEventScripts only re-syncs scripts
 * for ROM object count, so appended script pointers are preserved on Continue.
 *
 * Toggle: FLAG_SYS_ROUTE_WILD_WANDERERS_DISABLED — setflag disables the system (clearflag enables).
 * Default is enabled (flag clear after InitEventData / existing saves).
 */

#define ROUTE_WILD_MON_MAX 6
#define LOCALID_ROUTE_WILD_FIRST 242
#define LOCALID_ROUTE_WILD_LAST (LOCALID_ROUTE_WILD_FIRST + ROUTE_WILD_MON_MAX - 1)

/* Backward-compatible aliases for the original three wanderers */
#define LOCALID_ROUTE_WILD_A (LOCALID_ROUTE_WILD_FIRST + 0)
#define LOCALID_ROUTE_WILD_B (LOCALID_ROUTE_WILD_FIRST + 1)
#define LOCALID_ROUTE_WILD_C (LOCALID_ROUTE_WILD_FIRST + 2)

enum RouteWildSlotKind {
    ROUTE_WILD_KIND_LAND = 0,
    ROUTE_WILD_KIND_WATER,
    ROUTE_WILD_KIND_FISHING,
};

bool8 RouteWildWanderingSystemIsEnabled(void);

void RouteWildScheduleDeferredSetup(void);
void RouteWildProcessDeferredSetup(void);
void RouteWildNotifyScriptMovementBegin(void);
void RouteWildNotifyScriptMovementEnd(void);

void SetupRouteWildWanderingMons(void);
void RouteWildWandering_OnCameraUpdate(void);
u8 GetCurrentMapObjectEventTemplateCount(void);

void Special_PrepareRouteWildScriptedMon(void);
void Special_RemoveRouteWildMonAfterBattle(void);

bool8 TryStartRouteWildWanderingBattleOnBump(u8 direction);
bool8 TryRouteWildWanderingOverlapBattle(void);

struct ObjectEvent;
#if OW_POKEMON_OBJECT_EVENTS
bool8 RouteWildObjectEventCanTakeWanderStep(struct ObjectEvent *objectEvent, u8 direction);
bool8 RouteWildObjectEventIsProtectedShiny(struct ObjectEvent *objectEvent);
void RouteWildPersistClearByLocalId(u8 localId);
#endif

#endif // GUARD_ROUTE_WILD_WANDERERS_H

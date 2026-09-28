#ifndef GUARD_FOLLOWER_NPC_H
#define GUARD_FOLLOWER_NPC_H

// The NPC follower feature arrived after this branch point, so the screens ported
// from it see a world where the player never has an NPC following them. Every
// query answers "no follower", and the movement flags answer "nothing blocked"
// so field moves behave exactly as they did before.

#include "global.h"
#include "constants/follower_npc.h"

static inline bool32 PlayerHasFollowerNPC(void)
{
    return FALSE;
}

static inline u8 GetFollowerNPCObjectId(void)
{
    return 0;
}

static inline bool32 FollowerNPCIsBattlePartner(void)
{
    return FALSE;
}

static inline bool32 CheckFollowerNPCFlag(u32 flag)
{
    return TRUE;
}

static inline void FollowerNPCWalkIntoPlayerForLeaveMap(void)
{
}

static inline void FollowerNPCHideForLeaveMap(struct ObjectEvent *follower)
{
}

#endif // GUARD_FOLLOWER_NPC_H

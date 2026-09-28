#ifndef GUARD_FIELD_MOVE_H
#define GUARD_FIELD_MOVE_H

#include "global.h"
#include "constants/field_move.h"

struct FieldMoveUnlock
{
    bool32 (*isUnlockedFunc)(enum FieldMove);
    const u8 *lockedMessage;
};

enum FieldMoveUnlockType
{
    CANT_UNLOCK,
    ALWAYS_UNLOCKED,
    BADGE_UNLOCK,
    FIELD_MOVE_UNLOCK_COUNT
};

struct FieldMoveInfo
{
    bool32 (*fieldMoveFunc)(void);
    enum FieldMoveUnlockType unlockType:3;
    u32 moveID:11;
    u32 partyMsgID:7;
    u32 arg:8;
    u32 hideIfLocked:1;
    u32 padding:2;
};

extern const struct FieldMoveInfo gFieldMoveInfo[];
extern const struct FieldMoveUnlock gFieldMoveUnlocks[];

static inline bool32 SetUpFieldMove(enum FieldMove fieldMove)
{
    return gFieldMoveInfo[fieldMove].fieldMoveFunc();
}

static inline bool32 IsFieldMoveUnlocked(enum FieldMove fieldMove)
{
    return gFieldMoveUnlocks[gFieldMoveInfo[fieldMove].unlockType].isUnlockedFunc(fieldMove);
}

static inline const u8 *FieldMove_GetLockedMessage(enum FieldMove fieldMove)
{
    return gFieldMoveUnlocks[gFieldMoveInfo[fieldMove].unlockType].lockedMessage;
}

static inline u16 FieldMove_GetMoveId(enum FieldMove fieldMove)
{
    return gFieldMoveInfo[fieldMove].moveID;
}

static inline u32 FieldMove_GetPartyMsgID(enum FieldMove fieldMove)
{
    return gFieldMoveInfo[fieldMove].partyMsgID;
}

static inline bool32 FieldMove_IsVisible(enum FieldMove fieldMove)
{
    return !gFieldMoveInfo[fieldMove].hideIfLocked || IsFieldMoveUnlocked(fieldMove);
}

// Defined by whichever party menu implementation is compiled in.
bool32 SetUpFieldMove_Surf(void);
bool32 SetUpFieldMove_Fly(void);
bool32 SetUpFieldMove_Waterfall(void);
bool32 SetUpFieldMove_Dive(void);

#endif //GUARD_FIELD_MOVE_H

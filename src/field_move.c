#include "global.h"
#include "event_data.h"
#include "field_move.h"
#include "fldeff.h"
#include "fldeff_misc.h"
#include "party_menu.h"
#include "strings.h"
#include "constants/field_move.h"
#include "constants/moves.h"
#include "constants/party_menu.h"

#if SWSH_PARTY_MENU

static bool32 IsAlwaysFalse(enum FieldMove fieldMove)
{
    return FALSE;
}

static bool32 IsAlwaysTrue(enum FieldMove fieldMove)
{
    return TRUE;
}

static bool32 HasBadgeForFieldMove(enum FieldMove fieldMove)
{
    return FlagGet(gFieldMoveInfo[fieldMove].arg + FLAG_BADGE01_GET);
}

// The fldeff setup routines still return bool8 here, so they are called through
// wrappers rather than cast to the bool32 signature the table uses.
static bool32 FieldMove_Cut(void)         { return SetUpFieldMove_Cut(); }
static bool32 FieldMove_Flash(void)       { return SetUpFieldMove_Flash(); }
static bool32 FieldMove_RockSmash(void)   { return SetUpFieldMove_RockSmash(); }
static bool32 FieldMove_Strength(void)    { return SetUpFieldMove_Strength(); }
static bool32 FieldMove_Teleport(void)    { return SetUpFieldMove_Teleport(); }
static bool32 FieldMove_Dig(void)         { return SetUpFieldMove_Dig(); }
static bool32 FieldMove_SecretPower(void) { return SetUpFieldMove_SecretPower(); }
static bool32 FieldMove_SoftBoiled(void)  { return SetUpFieldMove_SoftBoiled(); }
static bool32 FieldMove_SweetScent(void)  { return SetUpFieldMove_SweetScent(); }

const struct FieldMoveUnlock gFieldMoveUnlocks[FIELD_MOVE_UNLOCK_COUNT] =
{
    [CANT_UNLOCK] =
    {
        .isUnlockedFunc = IsAlwaysFalse,
        .lockedMessage = gText_EmptyString2,
    },
    [ALWAYS_UNLOCKED] =
    {
        .isUnlockedFunc = IsAlwaysTrue,
        .lockedMessage = gText_EmptyString2,
    },
    [BADGE_UNLOCK] =
    {
        .isUnlockedFunc = HasBadgeForFieldMove,
        .lockedMessage = gText_CantUseUntilNewBadge,
    },
};

#define FLAG_TO_BADGE(flag) ((flag) - FLAG_BADGE01_GET)

const struct FieldMoveInfo gFieldMoveInfo[FIELD_MOVES_COUNT] =
{
    [FIELD_MOVE_CUT] =
    {
        .fieldMoveFunc = FieldMove_Cut,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_CUT,
        .partyMsgID = PARTY_MSG_NOTHING_TO_CUT,
        .arg = FLAG_TO_BADGE(FLAG_BADGE01_GET),
    },

    [FIELD_MOVE_FLASH] =
    {
        .fieldMoveFunc = FieldMove_Flash,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_FLASH,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE02_GET),
    },

    [FIELD_MOVE_ROCK_SMASH] =
    {
        .fieldMoveFunc = FieldMove_RockSmash,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_ROCK_SMASH,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE03_GET),
    },

    [FIELD_MOVE_STRENGTH] =
    {
        .fieldMoveFunc = FieldMove_Strength,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_STRENGTH,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE04_GET),
    },

    [FIELD_MOVE_SURF] =
    {
        .fieldMoveFunc = SetUpFieldMove_Surf,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_SURF,
        .partyMsgID = PARTY_MSG_CANT_SURF_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE05_GET),
    },

    [FIELD_MOVE_FLY] =
    {
        .fieldMoveFunc = SetUpFieldMove_Fly,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_FLY,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE06_GET),
    },

    [FIELD_MOVE_DIVE] =
    {
        .fieldMoveFunc = SetUpFieldMove_Dive,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_DIVE,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE07_GET),
    },

    [FIELD_MOVE_WATERFALL] =
    {
        .fieldMoveFunc = SetUpFieldMove_Waterfall,
        .unlockType = BADGE_UNLOCK,
        .moveID = MOVE_WATERFALL,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .arg = FLAG_TO_BADGE(FLAG_BADGE08_GET),
    },

    [FIELD_MOVE_TELEPORT] =
    {
        .fieldMoveFunc = FieldMove_Teleport,
        .unlockType = ALWAYS_UNLOCKED,
        .moveID = MOVE_TELEPORT,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
    },

    [FIELD_MOVE_DIG] =
    {
        .fieldMoveFunc = FieldMove_Dig,
        .unlockType = ALWAYS_UNLOCKED,
        .moveID = MOVE_DIG,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
    },

    [FIELD_MOVE_SECRET_POWER] =
    {
        .fieldMoveFunc = FieldMove_SecretPower,
        .unlockType = ALWAYS_UNLOCKED,
        .moveID = MOVE_SECRET_POWER,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
    },

    [FIELD_MOVE_MILK_DRINK] =
    {
        .fieldMoveFunc = FieldMove_SoftBoiled,
        .unlockType = ALWAYS_UNLOCKED,
        .moveID = MOVE_MILK_DRINK,
        .partyMsgID = PARTY_MSG_NOT_ENOUGH_HP,
    },

    [FIELD_MOVE_SOFT_BOILED] =
    {
        .fieldMoveFunc = FieldMove_SoftBoiled,
        .unlockType = ALWAYS_UNLOCKED,
        .moveID = MOVE_SOFT_BOILED,
        .partyMsgID = PARTY_MSG_NOT_ENOUGH_HP,
    },

    [FIELD_MOVE_SWEET_SCENT] =
    {
        .fieldMoveFunc = FieldMove_SweetScent,
        .unlockType = ALWAYS_UNLOCKED,
        .moveID = MOVE_SWEET_SCENT,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
    },

    // The overworld has no rock climb or defog interaction here, so both stay hidden.
    [FIELD_MOVE_ROCK_CLIMB] =
    {
        .fieldMoveFunc = NULL,
        .unlockType = CANT_UNLOCK,
        .moveID = MOVE_ROCK_CLIMB,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .hideIfLocked = TRUE,
    },

    [FIELD_MOVE_DEFOG] =
    {
        .fieldMoveFunc = NULL,
        .unlockType = CANT_UNLOCK,
        .moveID = MOVE_DEFOG,
        .partyMsgID = PARTY_MSG_CANT_USE_HERE,
        .hideIfLocked = TRUE,
    },
};

#endif // SWSH_PARTY_MENU

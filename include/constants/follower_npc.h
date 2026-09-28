#ifndef GUARD_CONSTANTS_FOLLOWER_NPC_H
#define GUARD_CONSTANTS_FOLLOWER_NPC_H

// NPC Follower Flags
#define FOLLOWER_NPC_FLAG_CAN_BIKE              0x2     // Player is allowed to use the Bike. Part of FOLLOWER_NPC_FLAG_ALL_LAND.
#define FOLLOWER_NPC_FLAG_CAN_LEAVE_ROUTE       0x4     // Player is allowed to use Fly/Teleport/EscapeRope/etc. Part of FOLLOWER_NPC_FLAG_ALL_LAND.
#define FOLLOWER_NPC_FLAG_CAN_SURF              0x8     // Player is allowed to Surf. Part of FOLLOWER_NPC_FLAG_ALL_WATER.
#define FOLLOWER_NPC_FLAG_CAN_WATERFALL         0x10    // Player is allowed to use Waterfall. Part of FOLLOWER_NPC_FLAG_ALL_WATER.
#define FOLLOWER_NPC_FLAG_CAN_DIVE              0x20    // Player is allowed to use Dive. Part of FOLLOWER_NPC_FLAG_ALL_WATER.
#define FOLLOWER_NPC_FLAG_CLEAR_ON_WHITE_OUT    0x80    // The NPC follower will be destroyed if the player whites out.

#define FOLLOWER_NPC_FLAG_ALL_LAND              (FOLLOWER_NPC_FLAG_CAN_BIKE | FOLLOWER_NPC_FLAG_CAN_LEAVE_ROUTE)
#define FOLLOWER_NPC_FLAG_ALL_WATER             (FOLLOWER_NPC_FLAG_CAN_SURF | FOLLOWER_NPC_FLAG_CAN_WATERFALL | FOLLOWER_NPC_FLAG_CAN_DIVE)
#define FOLLOWER_NPC_FLAG_ALL                   (FOLLOWER_NPC_FLAG_ALL_LAND | FOLLOWER_NPC_FLAG_ALL_WATER | FOLLOWER_NPC_FLAG_CLEAR_ON_WHITE_OUT)

#endif // GUARD_CONSTANTS_FOLLOWER_NPC_H

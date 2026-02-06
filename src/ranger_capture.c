#include "global.h"
#include "bg.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_effect.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"

#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "gba/io_reg.h"

// Script API (recommended):
// - VAR_0x8004: target localId (event object)
// - VAR_0x8005: near range in tiles (0 = default)
// - After `special StartRangerCapture` + `waitstate`, read VAR_RESULT (gSpecialVar_Result):
//   - 1: captured, 0: failed/canceled

#define RC_PROGRESS_MAX          1000
#define RC_DEFAULT_NEAR_RANGE    3
#define RC_FILL_PER_ORBIT        220 // capture progress per full circle
#define RC_DECAY_RATE            2   // per frame when not filling
#define RC_DECAY_RATE_DAMAGED    6   // per frame while recently damaged
#define RC_DAMAGED_FRAMES        60  // pause/penalty duration
#define RC_ATTACK_COOLDOWN       60  // frames between attacks
#define RC_ATTACK_RANGE_TILES    6

#define RC_WANDER_INTERVAL_MIN   35  // frames between random steps (slightly less movement)
#define RC_WANDER_INTERVAL_MAX   95
#define RC_WANDER_RADIUS_MAX     4   // target cannot move more than 4 tiles from start
#define RC_JUMP_CHANCE           25  // % chance to jump instead of walk
#define RC_FAST_WALK_CHANCE      30  // % chance to walk fast

#define RC_FADE_STATE_NONE         0
#define RC_FADE_STATE_OUT_START    1
#define RC_FADE_STATE_SHOW_START   2  // Show "Capture ON!" text
#define RC_FADE_STATE_IN_START     3
#define RC_FADE_STATE_OUT_END      4
#define RC_FADE_STATE_SHOW_END     5  // Show result text
#define RC_FADE_STATE_IN_END       6

#define RC_TEXT_DELAY_FRAMES     120  // 2 seconds at 60fps

// UI on BG0 so we don't touch BG3 (map). Raise BG0 priority during minigame.
#define RC_WINDOW_BG                 0
// Capture bar: top middle.
#define RC_BAR_WINDOW_LEFT            10
#define RC_BAR_WINDOW_TOP             0
#define RC_BAR_WINDOW_WIDTH           10
#define RC_BAR_WINDOW_HEIGHT          2
#define RC_BAR_WINDOW_PAL              STD_WINDOW_PALETTE_NUM
// "Hold A" + "L cancel": bottom left.
#define RC_HINT_WINDOW_LEFT           1
#define RC_HINT_WINDOW_TOP            15
#define RC_HINT_WINDOW_WIDTH          10
#define RC_HINT_WINDOW_HEIGHT         4
#define RC_HINT_WINDOW_PAL             STD_WINDOW_PALETTE_NUM
// Bar graphic inside bar window.
#define RC_BAR_X                      4
#define RC_BAR_Y                      4
#define RC_BAR_W                      72
#define RC_BAR_H                      6

// Border uses 9 tiles; bar 10*2=20 tiles; hint 6*2=12. Use distinct baseBlocks so
// CopyWindowToVram doesn't overwrite the border or the other window.
#define RC_BAR_BASE_BLOCK              (STD_WINDOW_BASE_TILE_NUM + 9)   // after border
#define RC_HINT_BASE_BLOCK             (RC_BAR_BASE_BLOCK + 10 * 2)     // after bar

#define RC_SPRITE_MARKER         0x52
#define RC_TRAIL_MAX             48  // max trail positions
#define RC_TRAIL_SPRITES         12  // fixed pool of sprites for trail

extern const struct SpriteTemplate gFieldEffectObjectTemplate_SmallSparkle;
extern const struct SpritePalette gSpritePalette_SmallSparkle;

struct RangerCaptureSession
{
    bool8 active;
    u8 taskId;
    u8 barWindowId;
    u8 hintWindowId;

    u8 targetLocalId;
    u8 targetMapGroup;
    u8 targetMapNum;
    u8 targetObjEventId;

    u16 progress; // 0..RC_PROGRESS_MAX
    u8 nearRangeTiles;

    u16 damagedTimer;
    u16 attackCooldown;

    // Orbit tracking (for walking circles).
    bool8 haveSector;
    u8 currentSector;   // 0-3 (quadrants around target)
    u8 sectorMask;      // bitmask of visited sectors in this orbit

    // Random wandering timer for the target.
    u16 wanderTimer;
    // Target's starting position (for max movement radius).
    s16 targetStartX;
    s16 targetStartY;

    // Follower state.
    bool8 followerWasHidden;

    // Player sprite state.
    u16 originalPlayerGraphicsId;
    u8 fadeState;
    u16 fadeTextTimer;  // For "Capture ON!" / result text delay
    u8 fadeTextWindowId;
    bool8 captureSuccess;  // TRUE if captured, FALSE if failed

    // Trail from orbit start to current position (cleared when orbit completes).
    s16 trailX[RC_TRAIL_MAX];
    s16 trailY[RC_TRAIL_MAX];
    u8 trailCount;
    u8 trailSpriteIds[RC_TRAIL_SPRITES];
};

static EWRAM_DATA struct RangerCaptureSession sRangerCapture = {0};

static const struct WindowTemplate sRangerCaptureBarWindowTemplate =
{
    .bg = RC_WINDOW_BG,
    .tilemapLeft = RC_BAR_WINDOW_LEFT,
    .tilemapTop = RC_BAR_WINDOW_TOP,
    .width = RC_BAR_WINDOW_WIDTH,
    .height = RC_BAR_WINDOW_HEIGHT,
    .paletteNum = RC_BAR_WINDOW_PAL,
    .baseBlock = RC_BAR_BASE_BLOCK,
};

static const struct WindowTemplate sRangerCaptureHintWindowTemplate =
{
    .bg = RC_WINDOW_BG,
    .tilemapLeft = RC_HINT_WINDOW_LEFT,
    .tilemapTop = RC_HINT_WINDOW_TOP,
    .width = RC_HINT_WINDOW_WIDTH,
    .height = RC_HINT_WINDOW_HEIGHT,
    .paletteNum = RC_HINT_WINDOW_PAL,
    .baseBlock = RC_HINT_BASE_BLOCK,
};

static void Task_RangerCapture(u8 taskId);
static void RangerCapture_DrawUi(void);
static void RangerCapture_DestroyUi(void);
static bool8 RangerCapture_GetPlayerAndTargetCoords(s16 *playerX, s16 *playerY, s16 *targetX, s16 *targetY);
static u16 RangerCapture_GetManhattanDistanceToTarget(void);
static u8 RangerCapture_GetSectorAroundTarget(s16 px, s16 py, s16 tx, s16 ty);
static void RangerCapture_ResetOrbit(void);
static void RangerCapture_UpdateOrbitAndProgress(void);
static void RangerCapture_End(bool8 captured);

static void RangerCapture_SpawnAttackIfReady(void);
static void SpriteCB_RangerProjectile(struct Sprite *sprite);
static void RangerCapture_OnPlayerDamaged(void);
static void RangerCapture_DestroyAllProjectiles(void);
static void RangerCapture_ResetWanderTimer(void);
static void RangerCapture_UpdateWander(void);
static void Task_RangerCaptureFade(u8 taskId);
static void RangerCapture_ClearTrail(void);
static void RangerCapture_UpdateTrail(s16 playerMapX, s16 playerMapY);
static void RangerCapture_HideAllObjectSprites(void);
static void RangerCapture_UnhideAllObjectSprites(void);

u16 StartRangerCapture(void)
{
    u8 taskId;

    // If already running, ignore.
    if (sRangerCapture.active)
        return 0;

    sRangerCapture.active = TRUE;
    sRangerCapture.taskId = TASK_NONE;
    sRangerCapture.barWindowId = WINDOW_NONE;
    sRangerCapture.hintWindowId = WINDOW_NONE;
    sRangerCapture.progress = 0;
    sRangerCapture.damagedTimer = 0;
    sRangerCapture.attackCooldown = 0;
    sRangerCapture.haveSector = FALSE;
    sRangerCapture.currentSector = 0;
    sRangerCapture.sectorMask = 0;
    sRangerCapture.wanderTimer = 0;
    sRangerCapture.followerWasHidden = FALSE;
    sRangerCapture.fadeState = RC_FADE_STATE_NONE;
    sRangerCapture.originalPlayerGraphicsId = 0;
    sRangerCapture.fadeTextTimer = 0;
    sRangerCapture.fadeTextWindowId = WINDOW_NONE;
    sRangerCapture.captureSuccess = FALSE;
    sRangerCapture.trailCount = 0;
    {
        u8 i;
        for (i = 0; i < RC_TRAIL_SPRITES; i++)
            sRangerCapture.trailSpriteIds[i] = MAX_SPRITES;
    }

    sRangerCapture.targetLocalId = gSpecialVar_0x8004;
    sRangerCapture.targetMapGroup = gSaveBlock1Ptr->location.mapGroup;
    sRangerCapture.targetMapNum = gSaveBlock1Ptr->location.mapNum;

    sRangerCapture.nearRangeTiles = gSpecialVar_0x8005;
    if (sRangerCapture.nearRangeTiles == 0)
        sRangerCapture.nearRangeTiles = RC_DEFAULT_NEAR_RANGE;

    sRangerCapture.targetObjEventId = GetObjectEventIdByLocalIdAndMap(
        sRangerCapture.targetLocalId,
        sRangerCapture.targetMapNum,
        sRangerCapture.targetMapGroup
    );
    if (sRangerCapture.targetObjEventId < OBJECT_EVENTS_COUNT && gObjectEvents[sRangerCapture.targetObjEventId].active)
    {
        sRangerCapture.targetStartX = gObjectEvents[sRangerCapture.targetObjEventId].currentCoords.x;
        sRangerCapture.targetStartY = gObjectEvents[sRangerCapture.targetObjEventId].currentCoords.y;
    }
    else
    {
        sRangerCapture.targetStartX = 0;
        sRangerCapture.targetStartY = 0;
    }

    RangerCapture_ResetOrbit();
    RangerCapture_ResetWanderTimer();

    // Default to failure until we succeed.
    gSpecialVar_Result = FALSE;

    // Lock controls during fade transitions.
    LockPlayerFieldControls();

    // Hide follower Pokemon if present.
    {
        struct ObjectEvent *followerObj = GetFollowerObject();
        if (followerObj != NULL && followerObj->active)
        {
            ClearObjectEventMovement(followerObj, &gSprites[followerObj->spriteId]);
            ObjectEventSetHeldMovement(followerObj, MOVEMENT_ACTION_ENTER_POKEBALL);
            sRangerCapture.followerWasHidden = TRUE;
        }
    }

    // Load palette used by projectile/trail sprites.
    LoadSpritePalette(&gSpritePalette_SmallSparkle);

    // Store original player graphics ID.
    {
        struct ObjectEvent *playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
        sRangerCapture.originalPlayerGraphicsId = playerObj->graphicsId;
    }

    // Start fade transition: fade out → change sprite → fade in → start minigame.
    sRangerCapture.fadeState = RC_FADE_STATE_OUT_START;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    taskId = CreateTask(Task_RangerCaptureFade, 0);
    return 0;
}

static void Task_RangerCapture(u8 taskId)
{
    // Ensure we never finish before the script reaches `waitstate`.
    if (gTasks[taskId].data[0]++ < 1)
        return;

    // If target doesn't exist anymore, cancel.
    if (sRangerCapture.targetObjEventId >= OBJECT_EVENTS_COUNT
     || !gObjectEvents[sRangerCapture.targetObjEventId].active)
    {
        RangerCapture_End(FALSE);
        return;
    }

    // B button allows running. L button cancels.
    if (JOY_NEW(L_BUTTON))
    {
        RangerCapture_End(FALSE);
        return;
    }

    if (sRangerCapture.damagedTimer != 0)
        sRangerCapture.damagedTimer--;

    if (sRangerCapture.attackCooldown != 0)
        sRangerCapture.attackCooldown--;

    // Pokémon random wandering.
    RangerCapture_UpdateWander();

    // Spawn attacks.
    RangerCapture_SpawnAttackIfReady();

    // Progress logic based on walking full circles around the target.
    RangerCapture_UpdateOrbitAndProgress();
    if (sRangerCapture.progress >= RC_PROGRESS_MAX)
    {
        RangerCapture_End(TRUE);
        return;
    }

    RangerCapture_DrawUi();

    // Update trail after UI so sparkle palette (loaded in UpdateTrail) isn't overwritten.
    {
        s16 px, py, tx, ty;
        if (RangerCapture_GetPlayerAndTargetCoords(&px, &py, &tx, &ty))
            RangerCapture_UpdateTrail(px, py);
    }
}

static void RangerCapture_DrawUi(void)
{
    u16 filled;
    static const u8 sText_HoldA[] = _("Hold {A_BUTTON}\n{L_BUTTON} Cancel");
    static const u8 sText_Damaged[] = _("HIT!");
    static const u8 sColor_Normal[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
    static const u8 sColor_Warn[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED, TEXT_COLOR_DARK_GRAY};

    if (sRangerCapture.barWindowId == WINDOW_NONE)
    {
        SetBgAttribute(RC_WINDOW_BG, BG_ATTR_PRIORITY, 3);
        sRangerCapture.barWindowId = AddWindow(&sRangerCaptureBarWindowTemplate);
        LoadUserWindowBorderGfx(sRangerCapture.barWindowId, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
        Menu_LoadStdPal();  // After border load so interior (PIXEL_FILL 1/2) is light, not frame's dark
        PutWindowTilemap(sRangerCapture.barWindowId);
        DrawTextBorderOuter(sRangerCapture.barWindowId, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM);
        LoadSpritePalette(&gSpritePalette_SmallSparkle);
        ScheduleBgCopyTilemapToVram(RC_WINDOW_BG);
    }
    if (sRangerCapture.hintWindowId == WINDOW_NONE)
    {
        sRangerCapture.hintWindowId = AddWindow(&sRangerCaptureHintWindowTemplate);
        PutWindowTilemap(sRangerCapture.hintWindowId);
        DrawTextBorderOuter(sRangerCapture.hintWindowId, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM);
        LoadSpritePalette(&gSpritePalette_SmallSparkle);  // Reload for trail/projectiles
        ScheduleBgCopyTilemapToVram(RC_WINDOW_BG);
    }

    // Top middle: capture bar (+ HIT! when damaged).
    {
        u8 windowId = sRangerCapture.barWindowId;
        FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
        FillWindowPixelRect(windowId, PIXEL_FILL(2), RC_BAR_X, RC_BAR_Y, RC_BAR_W, RC_BAR_H);
        filled = (u32)RC_BAR_W * sRangerCapture.progress / RC_PROGRESS_MAX;
        if (filled != 0)
            FillWindowPixelRect(windowId, PIXEL_FILL(6), RC_BAR_X, RC_BAR_Y, filled, RC_BAR_H);  // Green progress
        if (sRangerCapture.damagedTimer != 0)
            AddTextPrinterParameterized3(windowId, FONT_SMALL, 44, 0, sColor_Warn, TEXT_SKIP_DRAW, sText_Damaged);
        CopyWindowToVram(windowId, COPYWIN_GFX);
    }

    // Bottom left: "Hold A" and "L cancel" (y=2 avoids top clip, height 4 gives room for 2 lines).
    {
        u8 windowId = sRangerCapture.hintWindowId;
        FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
        AddTextPrinterParameterized3(windowId, FONT_SMALL, 0, 2, sColor_Normal, TEXT_SKIP_DRAW, sText_HoldA);
        CopyWindowToVram(windowId, COPYWIN_GFX);
    }

    // Reload sparkle palette last so trail/projectiles stay visible (UI can overwrite it).
    LoadSpritePalette(&gSpritePalette_SmallSparkle);
}

static void RangerCapture_DestroyUi(void)
{
    u8 left, top, width, height;

    // Clear full window area including the 1-tile border (DrawTextBorderOuter drew at left-1, top-1, etc.).
    if (sRangerCapture.barWindowId != WINDOW_NONE)
    {
        left = GetWindowAttribute(sRangerCapture.barWindowId, WINDOW_TILEMAP_LEFT);
        top = GetWindowAttribute(sRangerCapture.barWindowId, WINDOW_TILEMAP_TOP);
        width = GetWindowAttribute(sRangerCapture.barWindowId, WINDOW_WIDTH);
        height = GetWindowAttribute(sRangerCapture.barWindowId, WINDOW_HEIGHT);
        FillBgTilemapBufferRect(RC_WINDOW_BG, 0, (u8)(left - 1), (u8)(top - 1), (u8)(width + 2), (u8)(height + 2), STD_WINDOW_PALETTE_NUM);
        RemoveWindow(sRangerCapture.barWindowId);
        sRangerCapture.barWindowId = WINDOW_NONE;
    }
    if (sRangerCapture.hintWindowId != WINDOW_NONE)
    {
        left = GetWindowAttribute(sRangerCapture.hintWindowId, WINDOW_TILEMAP_LEFT);
        top = GetWindowAttribute(sRangerCapture.hintWindowId, WINDOW_TILEMAP_TOP);
        width = GetWindowAttribute(sRangerCapture.hintWindowId, WINDOW_WIDTH);
        height = GetWindowAttribute(sRangerCapture.hintWindowId, WINDOW_HEIGHT);
        FillBgTilemapBufferRect(RC_WINDOW_BG, 0, (u8)(left - 1), (u8)(top - 1), (u8)(width + 2), (u8)(height + 2), STD_WINDOW_PALETTE_NUM);
        RemoveWindow(sRangerCapture.hintWindowId);
        sRangerCapture.hintWindowId = WINDOW_NONE;
    }
    ScheduleBgCopyTilemapToVram(RC_WINDOW_BG);
    SetBgAttribute(RC_WINDOW_BG, BG_ATTR_PRIORITY, 0);
}

static bool8 RangerCapture_GetPlayerAndTargetCoords(s16 *playerX, s16 *playerY, s16 *targetX, s16 *targetY)
{
    u8 playerObjId = gPlayerAvatar.objectEventId;
    u8 targetObjId = sRangerCapture.targetObjEventId;

    if (playerObjId >= OBJECT_EVENTS_COUNT || targetObjId >= OBJECT_EVENTS_COUNT)
        return FALSE;
    if (!gObjectEvents[playerObjId].active || !gObjectEvents[targetObjId].active)
        return FALSE;

    *playerX = gObjectEvents[playerObjId].currentCoords.x;
    *playerY = gObjectEvents[playerObjId].currentCoords.y;
    *targetX = gObjectEvents[targetObjId].currentCoords.x;
    *targetY = gObjectEvents[targetObjId].currentCoords.y;
    return TRUE;
}

static u16 RangerCapture_GetManhattanDistanceToTarget(void)
{
    s16 px, py, tx, ty;
    if (!RangerCapture_GetPlayerAndTargetCoords(&px, &py, &tx, &ty))
        return 0xFFFF;
    return abs(px - tx) + abs(py - ty);
}

static void RangerCapture_End(bool8 captured)
{
    u8 taskId = sRangerCapture.taskId;

    // Lock controls during end transition.
    LockPlayerFieldControls();

    sRangerCapture.captureSuccess = captured;  // Store for result text

    if (captured)
    {
        RemoveObjectEventByLocalIdAndMap(sRangerCapture.targetLocalId, sRangerCapture.targetMapNum, sRangerCapture.targetMapGroup);
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
    /* Success/failure SE plays when result text is shown */

    RangerCapture_DestroyAllProjectiles();
    RangerCapture_ClearTrail();
    RangerCapture_DestroyUi();
    FreeSpritePaletteByTag(FLDEFF_PAL_TAG_SMALL_SPARKLE);

    // Stop the main capture task.
    if (taskId != TASK_NONE && gTasks[taskId].isActive)
        DestroyTask(taskId);

    // Start fade out to restore player sprite.
    sRangerCapture.fadeState = RC_FADE_STATE_OUT_END;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    
    // Create or reuse fade task for the end transition.
    {
        u8 fadeTaskId;
        bool8 foundFadeTask = FALSE;
        for (fadeTaskId = 0; fadeTaskId < NUM_TASKS; fadeTaskId++)
        {
            if (gTasks[fadeTaskId].isActive && gTasks[fadeTaskId].func == Task_RangerCaptureFade)
            {
                foundFadeTask = TRUE;
                break;
            }
        }
        if (!foundFadeTask)
            CreateTask(Task_RangerCaptureFade, 0);
    }
}

static void RangerCapture_SpawnAttackIfReady(void)
{
    u16 dist;
    u8 spriteId;
    struct Sprite *sprite;
    s16 playerX, playerY, targetX, targetY;

    if (sRangerCapture.attackCooldown != 0)
        return;

    dist = RangerCapture_GetManhattanDistanceToTarget();
    if (dist > RC_ATTACK_RANGE_TILES)
        return;

    if (!RangerCapture_GetPlayerAndTargetCoords(&playerX, &playerY, &targetX, &targetY))
        return;

    // Spawn from the target sprite position (pixel coords).
    if (gObjectEvents[sRangerCapture.targetObjEventId].spriteId >= MAX_SPRITES)
        return;

    spriteId = CreateSpriteAtEnd(&gFieldEffectObjectTemplate_SmallSparkle,
        gSprites[gObjectEvents[sRangerCapture.targetObjEventId].spriteId].x,
        gSprites[gObjectEvents[sRangerCapture.targetObjEventId].spriteId].y,
        0
    );
    if (spriteId == MAX_SPRITES)
        return;

    sprite = &gSprites[spriteId];
    sprite->coordOffsetEnabled = TRUE;
    sprite->callback = SpriteCB_RangerProjectile;
    sprite->data[7] = RC_SPRITE_MARKER;

    // Store a simple velocity toward the player's current sprite position.
    {
        s16 dx = gSprites[gPlayerAvatar.spriteId].x - sprite->x;
        s16 dy = gSprites[gPlayerAvatar.spriteId].y - sprite->y;
        s16 adx = abs(dx);
        s16 ady = abs(dy);

        if (adx == 0 && ady == 0)
        {
            sprite->data[1] = 0;
            sprite->data[2] = 0;
        }
        else if (adx >= ady)
        {
            sprite->data[1] = (dx < 0) ? -2 : 2;
            sprite->data[2] = (adx == 0) ? 0 : (dy * 2) / adx;
        }
        else
        {
            sprite->data[2] = (dy < 0) ? -2 : 2;
            sprite->data[1] = (ady == 0) ? 0 : (dx * 2) / ady;
        }
    }

    sRangerCapture.attackCooldown = RC_ATTACK_COOLDOWN;
}

// data[0] life timer
// data[1] velX
// data[2] velY
static void SpriteCB_RangerProjectile(struct Sprite *sprite)
{
    s16 playerX, playerY;

    sprite->x += sprite->data[1];
    sprite->y += sprite->data[2];

    if (++sprite->data[0] > 120)
    {
        DestroySprite(sprite);
        return;
    }

    playerX = gSprites[gPlayerAvatar.spriteId].x;
    playerY = gSprites[gPlayerAvatar.spriteId].y;

    if (abs(sprite->x - playerX) <= 8 && abs(sprite->y - playerY) <= 8)
    {
        RangerCapture_OnPlayerDamaged();
        DestroySprite(sprite);
        return;
    }
}

static void RangerCapture_OnPlayerDamaged(void)
{
    if (!sRangerCapture.active)
        return;

    sRangerCapture.damagedTimer = RC_DAMAGED_FRAMES;

    // Quick feedback sparkle at the player.
    if (gPlayerAvatar.spriteId < MAX_SPRITES)
    {
        u8 spriteId = CreateSpriteAtEnd(&gFieldEffectObjectTemplate_SmallSparkle,
            gSprites[gPlayerAvatar.spriteId].x,
            gSprites[gPlayerAvatar.spriteId].y,
            0
        );
        if (spriteId != MAX_SPRITES)
            gSprites[spriteId].coordOffsetEnabled = TRUE;
    }

    PlaySE(SE_BALL_BOUNCE_1);
}

static void RangerCapture_DestroyAllProjectiles(void)
{
    u32 i;
    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse && gSprites[i].data[7] == RC_SPRITE_MARKER)
            DestroySprite(&gSprites[i]);
    }
}

// Divide the tiles around the target into 4 rough sectors:
//  0: east, 1: south, 2: west, 3: north.
static u8 RangerCapture_GetSectorAroundTarget(s16 px, s16 py, s16 tx, s16 ty)
{
    s16 dx = px - tx;
    s16 dy = py - ty;
    s16 adx = abs(dx);
    s16 ady = abs(dy);

    if (adx >= ady)
        return (dx >= 0) ? 0 : 2; // east / west
    else
        return (dy >= 0) ? 1 : 3; // south / north
}

static void RangerCapture_ResetOrbit(void)
{
    sRangerCapture.haveSector = FALSE;
    sRangerCapture.currentSector = 0;
    sRangerCapture.sectorMask = 0;
    RangerCapture_ClearTrail();
}

// Require the player to walk a full loop around the target while holding A.
static void RangerCapture_UpdateOrbitAndProgress(void)
{
    u16 dist = RangerCapture_GetManhattanDistanceToTarget();
    bool8 holdingA = JOY_HELD(A_BUTTON);
    bool8 damaged = (sRangerCapture.damagedTimer != 0);

    if (holdingA && !damaged && dist <= sRangerCapture.nearRangeTiles)
    {
        s16 px, py, tx, ty;
        u8 sector;

        if (!RangerCapture_GetPlayerAndTargetCoords(&px, &py, &tx, &ty))
            return;

        sector = RangerCapture_GetSectorAroundTarget(px, py, tx, ty);

        if (!sRangerCapture.haveSector)
        {
            sRangerCapture.haveSector = TRUE;
            sRangerCapture.currentSector = sector;
            sRangerCapture.sectorMask = (1 << sector);
            // Start trail at first A-press position (orbit start marker).
            sRangerCapture.trailX[0] = px;
            sRangerCapture.trailY[0] = py;
            sRangerCapture.trailCount = 1;
        }
        else if (sector != sRangerCapture.currentSector)
        {
            sRangerCapture.currentSector = sector;
            sRangerCapture.sectorMask |= (1 << sector);

            // Completed a full orbit once all 4 quadrants have been visited.
            if (sRangerCapture.sectorMask == 0x0F)
            {
                if (sRangerCapture.progress + RC_FILL_PER_ORBIT >= RC_PROGRESS_MAX)
                    sRangerCapture.progress = RC_PROGRESS_MAX;
                else
                    sRangerCapture.progress += RC_FILL_PER_ORBIT;

                // Clear trail and start tracking the next orbit from the current quadrant.
                RangerCapture_ClearTrail();
                sRangerCapture.sectorMask = (1 << sector);
                // Re-seed trail so the line reappears for the next orbit.
                sRangerCapture.trailX[0] = px;
                sRangerCapture.trailY[0] = py;
                sRangerCapture.trailCount = 1;
            }
        }

        // Append current position to trail when moved. Use sliding window so we never run out.
        if (sRangerCapture.trailCount > 0
            && (px != sRangerCapture.trailX[sRangerCapture.trailCount - 1]
                || py != sRangerCapture.trailY[sRangerCapture.trailCount - 1]))
        {
            if (sRangerCapture.trailCount < RC_TRAIL_MAX)
            {
                sRangerCapture.trailX[sRangerCapture.trailCount] = px;
                sRangerCapture.trailY[sRangerCapture.trailCount] = py;
                sRangerCapture.trailCount++;
            }
            else
            {
                // Sliding window: keep orbit start (index 0), shift rest, add current at end.
                u8 i;
                for (i = 1; i < RC_TRAIL_MAX - 1; i++)
                {
                    sRangerCapture.trailX[i] = sRangerCapture.trailX[i + 1];
                    sRangerCapture.trailY[i] = sRangerCapture.trailY[i + 1];
                }
                sRangerCapture.trailX[RC_TRAIL_MAX - 1] = px;
                sRangerCapture.trailY[RC_TRAIL_MAX - 1] = py;
            }
        }
    }
    else
    {
        // Not orbiting correctly: slowly decay progress.
        u16 decay = damaged ? RC_DECAY_RATE_DAMAGED : RC_DECAY_RATE;
        if (sRangerCapture.progress <= decay)
            sRangerCapture.progress = 0;
        else
            sRangerCapture.progress -= decay;

        // Reset orbit tracking so the player needs to start a new full circle.
        RangerCapture_ResetOrbit();
    }
}

static void RangerCapture_ResetWanderTimer(void)
{
    sRangerCapture.wanderTimer = RC_WANDER_INTERVAL_MIN + Random() % (RC_WANDER_INTERVAL_MAX - RC_WANDER_INTERVAL_MIN);
}

static void RangerCapture_UpdateWander(void)
{
    struct ObjectEvent *targetObj;
    s16 curX, curY, nextX, nextY;
    s16 dx, dy;
    u8 direction, movementAction;
    u8 moveType;

    if (sRangerCapture.targetObjEventId >= OBJECT_EVENTS_COUNT)
        return;

    targetObj = &gObjectEvents[sRangerCapture.targetObjEventId];
    if (!targetObj->active)
        return;

    // Clear finished movement so we can queue a new one (engine keeps "active" until we clear).
    ObjectEventClearHeldMovementIfFinished(targetObj);

    if (sRangerCapture.wanderTimer > 0)
    {
        sRangerCapture.wanderTimer--;
        return;
    }

    // Take a random action if not already moving.
    if (!ObjectEventIsMovementOverridden(targetObj) && !ObjectEventIsHeldMovementActive(targetObj))
    {
        curX = targetObj->currentCoords.x;
        curY = targetObj->currentCoords.y;
        direction = Random() % 4;
        /* One step: 0=down, 1=up, 2=left, 3=right */
        dx = (direction == 2) ? -1 : (direction == 3) ? 1 : 0;
        dy = (direction == 0) ? 1 : (direction == 1) ? -1 : 0;
        nextX = curX + dx;
        nextY = curY + dy;
        /* Enforce max radius of RC_WANDER_RADIUS_MAX from start (Chebyshev distance) */
        if (abs(nextX - sRangerCapture.targetStartX) > RC_WANDER_RADIUS_MAX
         || abs(nextY - sRangerCapture.targetStartY) > RC_WANDER_RADIUS_MAX)
        {
            RangerCapture_ResetWanderTimer();
            return;
        }

        moveType = Random() % 100;
        if (moveType < RC_JUMP_CHANCE)
        {
            movementAction = MOVEMENT_ACTION_JUMP_DOWN + direction;
        }
        else if (moveType < RC_JUMP_CHANCE + RC_FAST_WALK_CHANCE)
        {
            movementAction = MOVEMENT_ACTION_WALK_FAST_DOWN + direction;
        }
        else
        {
            movementAction = MOVEMENT_ACTION_WALK_NORMAL_DOWN + direction;
        }

        ObjectEventSetHeldMovement(targetObj, movementAction);
        RangerCapture_ResetWanderTimer();
    }
    /* When still moving: don't reset timer (stay at 0) so we retry every frame. */
}

static void Task_RangerCaptureFade(u8 taskId)
{
    struct ObjectEvent *playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    static const u8 sText_CaptureOn[] = _("Capture ON!");
    static const u8 sText_CaptureComplete[] = _("Capture Complete!");
    static const u8 sText_CaptureFailed[] = _("Capture Failed!");
    static const u8 sColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};

    switch (sRangerCapture.fadeState)
    {
    case RC_FADE_STATE_OUT_START:
        // Wait for fade out to complete.
        if (!gPaletteFade.active)
        {
            // Fade out complete - change sprite to rangertop.
            ObjectEventSetGraphicsId(playerObj, OBJ_EVENT_GFX_RANGERTOP);
            ObjectEventTurn(playerObj, playerObj->facingDirection);
            
            // Show "Capture ON!" text on black background (like whiteout screen).
            sRangerCapture.fadeState = RC_FADE_STATE_SHOW_START;
            sRangerCapture.fadeTextTimer = RC_TEXT_DELAY_FRAMES;
            RangerCapture_HideAllObjectSprites();
            PlaySE(SE_SUPER_EFFECTIVE);
            
            // Create centered window for text (no border, transparent bg).
            {
                struct WindowTemplate template = {
                    .bg = 0,
                    .tilemapLeft = 0,
                    .tilemapTop = 8,
                    .width = 30,
                    .height = 4,
                    .paletteNum = 15,
                    .baseBlock = 1
                };
                Menu_LoadStdPalAt(BG_PLTT_ID(15));
                sRangerCapture.fadeTextWindowId = AddWindow(&template);
                PutWindowTilemap(sRangerCapture.fadeTextWindowId);
                FillWindowPixelBuffer(sRangerCapture.fadeTextWindowId, PIXEL_FILL(0));  // Transparent (black shows through)
                AddTextPrinterParameterized3(sRangerCapture.fadeTextWindowId, FONT_NORMAL, GetStringCenterAlignXOffset(FONT_NORMAL, sText_CaptureOn, 240), 8, sColor, TEXT_SKIP_DRAW, sText_CaptureOn);
                CopyWindowToVram(sRangerCapture.fadeTextWindowId, COPYWIN_FULL);
            }
        }
        break;

    case RC_FADE_STATE_SHOW_START:
        // Wait for text delay.
        if (--sRangerCapture.fadeTextTimer == 0)
        {
            RangerCapture_UnhideAllObjectSprites();
            ClearWindowTilemap(sRangerCapture.fadeTextWindowId);
            RemoveWindow(sRangerCapture.fadeTextWindowId);
            sRangerCapture.fadeTextWindowId = WINDOW_NONE;
            CopyBgTilemapBufferToVram(0);
            
            sRangerCapture.fadeState = RC_FADE_STATE_IN_START;
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        }
        break;

    case RC_FADE_STATE_IN_START:
        // Wait for fade in to complete.
        if (!gPaletteFade.active)
        {
            // Fade in complete - start the actual minigame.
            sRangerCapture.fadeState = RC_FADE_STATE_NONE;
            sRangerCapture.taskId = CreateTask(Task_RangerCapture, 0);
            RangerCapture_DrawUi();
            UnlockPlayerFieldControls();  // Now allow player to move
            DestroyTask(taskId);
        }
        break;

    case RC_FADE_STATE_OUT_END:
        // Wait for fade out to complete.
        if (!gPaletteFade.active)
        {
            // Fade out complete - restore original sprite.
            ObjectEventSetGraphicsId(playerObj, sRangerCapture.originalPlayerGraphicsId);
            ObjectEventTurn(playerObj, playerObj->facingDirection);
            
            // Restore follower if we hid it.
            if (sRangerCapture.followerWasHidden)
            {
                struct ObjectEvent *followerObj = GetFollowerObject();
                if (followerObj != NULL)
                {
                    ObjectEventClearHeldMovementIfFinished(followerObj);
                    followerObj->invisible = FALSE;
                }
            }
            
            // Show result text on black background (like whiteout screen).
            sRangerCapture.fadeState = RC_FADE_STATE_SHOW_END;
            sRangerCapture.fadeTextTimer = RC_TEXT_DELAY_FRAMES;
            RangerCapture_HideAllObjectSprites();
            PlaySE(sRangerCapture.captureSuccess ? SE_EXP_MAX : SE_FAILURE);
            
            // Create centered window for result text (no border, transparent bg).
            {
                struct WindowTemplate template = {
                    .bg = 0,
                    .tilemapLeft = 0,
                    .tilemapTop = 8,
                    .width = 30,
                    .height = 4,
                    .paletteNum = 15,
                    .baseBlock = 1
                };
                const u8 *text = sRangerCapture.captureSuccess ? sText_CaptureComplete : sText_CaptureFailed;
                Menu_LoadStdPalAt(BG_PLTT_ID(15));
                sRangerCapture.fadeTextWindowId = AddWindow(&template);
                PutWindowTilemap(sRangerCapture.fadeTextWindowId);
                FillWindowPixelBuffer(sRangerCapture.fadeTextWindowId, PIXEL_FILL(0));  // Transparent (black shows through)
                AddTextPrinterParameterized3(sRangerCapture.fadeTextWindowId, FONT_NORMAL, GetStringCenterAlignXOffset(FONT_NORMAL, text, 240), 8, sColor, TEXT_SKIP_DRAW, text);
                CopyWindowToVram(sRangerCapture.fadeTextWindowId, COPYWIN_FULL);
            }
        }
        break;

    case RC_FADE_STATE_SHOW_END:
        // Wait for text delay.
        if (--sRangerCapture.fadeTextTimer == 0)
        {
            RangerCapture_UnhideAllObjectSprites();
            ClearWindowTilemap(sRangerCapture.fadeTextWindowId);
            RemoveWindow(sRangerCapture.fadeTextWindowId);
            sRangerCapture.fadeTextWindowId = WINDOW_NONE;
            CopyBgTilemapBufferToVram(0);
            
            sRangerCapture.fadeState = RC_FADE_STATE_IN_END;
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        }
        break;

    case RC_FADE_STATE_IN_END:
        // Wait for fade in to complete.
        if (!gPaletteFade.active)
        {
            // All done - clean up and resume script.
            sRangerCapture = (struct RangerCaptureSession){0};
            ScriptContext_Enable();
            DestroyTask(taskId);
        }
        break;
    }
}

static void RangerCapture_ClearTrail(void)
{
    u8 i;
    for (i = 0; i < RC_TRAIL_SPRITES; i++)
    {
        if (sRangerCapture.trailSpriteIds[i] < MAX_SPRITES)
        {
            DestroySprite(&gSprites[sRangerCapture.trailSpriteIds[i]]);
            sRangerCapture.trailSpriteIds[i] = MAX_SPRITES;
        }
    }
    sRangerCapture.trailCount = 0;
}

static void RangerCapture_UpdateTrail(s16 playerMapX, s16 playerMapY)
{
    u8 i;
    u8 spriteIdx;
    u32 trailStep;
    u32 pathLen;  // trail points + current position
    s16 mapX, mapY;  // map coords to convert
    s16 x, y;

    if (sRangerCapture.trailCount < 1)
        return;

    // Path = stored trail [0..trailCount-1] plus current (px, py). Always 2+ points when orbiting.
    pathLen = sRangerCapture.trailCount + 1;

    LoadSpritePalette(&gSpritePalette_SmallSparkle);

    for (i = 0; i < RC_TRAIL_SPRITES; i++)
    {
        trailStep = (i * (pathLen - 1)) / RC_TRAIL_SPRITES;
        if (trailStep >= pathLen)
            trailStep = pathLen - 1;

        if (trailStep < sRangerCapture.trailCount)
        {
            mapX = sRangerCapture.trailX[trailStep];
            mapY = sRangerCapture.trailY[trailStep];
        }
        else
        {
            mapX = playerMapX;
            mapY = playerMapY;
        }

        /* Convert map coords to sprite coords (same as field effect sprites). */
        x = mapX;
        y = mapY;
        SetSpritePosToOffsetMapCoords(&x, &y, 8, 8);

        if (sRangerCapture.trailSpriteIds[i] >= MAX_SPRITES)
        {
            spriteIdx = CreateSpriteAtEnd(&gFieldEffectObjectTemplate_SmallSparkle, x, y, 0);
            if (spriteIdx < MAX_SPRITES)
            {
                sRangerCapture.trailSpriteIds[i] = spriteIdx;
                gSprites[spriteIdx].coordOffsetEnabled = TRUE;
                gSprites[spriteIdx].oam.priority = 0;  // Draw on top of overworld
                gSprites[spriteIdx].callback = SpriteCallbackDummy;  // Override: don't hide/destroy (default callback does)
            }
        }

        if (sRangerCapture.trailSpriteIds[i] < MAX_SPRITES)
        {
            gSprites[sRangerCapture.trailSpriteIds[i]].x = x;
            gSprites[sRangerCapture.trailSpriteIds[i]].y = y;
        }
    }
}

static void RangerCapture_HideAllObjectSprites(void)
{
    u32 i;
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].active && gObjectEvents[i].spriteId < MAX_SPRITES)
            gSprites[gObjectEvents[i].spriteId].invisible = TRUE;
    }
}

static void RangerCapture_UnhideAllObjectSprites(void)
{
    u32 i;
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].active && gObjectEvents[i].spriteId < MAX_SPRITES)
            gSprites[gObjectEvents[i].spriteId].invisible = FALSE;
    }
}

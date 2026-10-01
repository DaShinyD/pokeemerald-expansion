#include "global.h"
#include "battle.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "shadow_pokemon.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/rgb.h"
#include "constants/species.h"

bool32 IsShadowMon(struct Pokemon *mon)
{
    if (mon == NULL)
        return FALSE;
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE)
        return FALSE;
    if (GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
    return GetMonData(mon, MON_DATA_IS_SHADOW);
}

static struct Pokemon *GetBattlerPartyMon(u32 battler)
{
    if (GetBattlerSide(battler) == B_SIDE_PLAYER)
        return &gPlayerParty[gBattlerPartyIndexes[battler]];
    return &gEnemyParty[gBattlerPartyIndexes[battler]];
}

bool32 IsBattlerShadowMon(u32 battler)
{
    return IsShadowMon(GetBattlerPartyMon(battler));
}

u8 GetShadowHeart(struct Pokemon *mon)
{
    if (!IsShadowMon(mon))
        return 0;
    return GetMonData(mon, MON_DATA_SHADOW_HEART);
}

u8 GetShadowBattlesToPurify(struct Pokemon *mon)
{
    u8 heart;
    u8 remaining;

    if (!IsShadowMon(mon))
        return 0;

    heart = GetMonData(mon, MON_DATA_SHADOW_HEART);
    if (heart >= SHADOW_HEART_MAX)
        return 0;

    remaining = SHADOW_HEART_MAX - heart;
    return (remaining + SHADOW_HEART_GAIN_BATTLE - 1) / SHADOW_HEART_GAIN_BATTLE;
}

u8 TryAddShadowHeart(struct Pokemon *mon, u8 amount)
{
    u8 heart;

    if (!IsShadowMon(mon) || amount == 0)
        return GetMonData(mon, MON_DATA_SHADOW_HEART);

    heart = GetMonData(mon, MON_DATA_SHADOW_HEART);
    if (heart >= SHADOW_HEART_MAX)
        return heart;

    if (heart + amount > SHADOW_HEART_MAX)
        heart = SHADOW_HEART_MAX;
    else
        heart += amount;

    SetMonData(mon, MON_DATA_SHADOW_HEART, &heart);
    return heart;
}

void SetMonAsShadow(struct Pokemon *mon)
{
    u8 heart = 0;
    u8 level = GetMonData(mon, MON_DATA_LEVEL);
    u8 flag = TRUE;

    SetMonData(mon, MON_DATA_IS_SHADOW, &flag);
    SetMonData(mon, MON_DATA_SHEEN, &level);
    SetMonData(mon, MON_DATA_SHADOW_HEART, &heart);
}

bool32 PurifyShadowMon(struct Pokemon *mon)
{
    u8 zero = 0;
    u8 flag = FALSE;
    u8 ribbon = TRUE;
    u32 maxHP;

    if (!IsShadowMon(mon))
        return FALSE;
    if (GetMonData(mon, MON_DATA_SHADOW_HEART) < SHADOW_HEART_MAX)
        return FALSE;

    SetMonData(mon, MON_DATA_IS_SHADOW, &flag);
    SetMonData(mon, MON_DATA_SHADOW_HEART, &zero);
    SetMonData(mon, MON_DATA_SHEEN, &zero);
    SetMonData(mon, MON_DATA_NATIONAL_RIBBON, &ribbon);
    CalculateMonStats(mon);
    maxHP = GetMonData(mon, MON_DATA_MAX_HP);
    SetMonData(mon, MON_DATA_HP, &maxHP);
    return TRUE;
}

void ApplyShadowTintToPaletteBuffer(u16 *pal)
{
    u32 i;

    if (pal == NULL)
        return;

    for (i = 1; i < 16; i++)
    {
        u16 color = pal[i];
        u32 r = GET_R(color) * 6 / 16;
        u32 g = GET_G(color) * 4 / 16;
        u32 b = GET_B(color) * 10 / 16;
        pal[i] = RGB(r, g, b);
    }
}

static u32 GiveOneShadowLegendary(u16 species, u8 heart)
{
    struct Pokemon mon;
    u32 exp;

    CreateMon(&mon, species, 50, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
    SetMonAsShadow(&mon);
    SetMonData(&mon, MON_DATA_SHADOW_HEART, &heart);
    exp = gExperienceTables[gSpeciesInfo[species].growthRate][50];
    SetMonData(&mon, MON_DATA_EXP, &exp);
    return GiveMonToPlayer(&mon);
}

void GiveShadowLegendaries(void)
{
    static const u16 sShadowLegendaries[] = {
        SPECIES_DARKRAI,
        SPECIES_SHAYMIN_LAND,
        SPECIES_CRESSELIA,
        SPECIES_UXIE,
        SPECIES_MESPRIT,
        SPECIES_AZELF,
    };
    u32 i;
    u32 result;
    u32 firstOk = MON_CANT_GIVE;
    bool32 gaveAny = FALSE;

    for (i = 0; i < ARRAY_COUNT(sShadowLegendaries); i++)
    {
        // Darkrai is ready to purify; the rest start with an empty Heart Gauge.
        result = GiveOneShadowLegendary(sShadowLegendaries[i], (i == 0) ? SHADOW_HEART_MAX : 0);
        if (result != MON_CANT_GIVE)
        {
            if (!gaveAny)
                firstOk = result;
            gaveAny = TRUE;
        }
    }

    if (!gaveAny)
    {
        gSpecialVar_Result = MON_CANT_GIVE;
        return;
    }

    gSpecialVar_Result = firstOk;
    AddBagItem(ITEM_JOY_SCENT, 1);
}

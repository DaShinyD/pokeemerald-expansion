#ifndef GUARD_SHADOW_POKEMON_H
#define GUARD_SHADOW_POKEMON_H

#include "global.h"

struct Pokemon;

bool32 IsShadowMon(struct Pokemon *mon);
bool32 IsBattlerShadowMon(u32 battler);
u8 GetShadowHeart(struct Pokemon *mon);
u8 GetShadowBattlesToPurify(struct Pokemon *mon);
u8 TryAddShadowHeart(struct Pokemon *mon, u8 amount);
void SetMonAsShadow(struct Pokemon *mon);
bool32 PurifyShadowMon(struct Pokemon *mon);
void ApplyShadowTintToPaletteBuffer(u16 *pal);
void GiveShadowLegendaries(void);

#endif // GUARD_SHADOW_POKEMON_H

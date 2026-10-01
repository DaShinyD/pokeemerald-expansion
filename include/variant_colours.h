#ifndef GUARD_VARIANT_COLOURS_H
#define GUARD_VARIANT_COLOURS_H

#include "global.h"

struct PaletteVariant
{
  u8 start : 4;        // Start index of palette customisation range
  u8 length : 4;       // Length of customisation range
  u8 hue_amount : 3;   // Index into hue table [0,10,20,30,45,60,90,180]
  u8 chr_amount : 2;   // Index into chroma table [0,5,10,25]
  u8 lum_amount : 2;   // Index into luma table [0,5,10,25]
  u8 sv_down_only : 1; // If set, switch from +/- to "down only" for both C & L (C: -2*chr, L: -2*lum)
};

struct SpeciesVariant
{
  struct PaletteVariant pv1;
  struct PaletteVariant pv2;
};

const struct SpeciesVariant *GetSpeciesVariants(u32 species);

void ApplyPaletteVariantToPaletteBuffer(u16 pal16[16], const struct PaletteVariant *pv, u16 prn16);
void ApplyCustomRestrictionToPaletteBuffer(u8 hMin, u8 hMax, u8 cMin, u8 cMax, u8 lMin, u8 lMax, u16 pal16[16]);
void ApplyMonSpeciesVariantToPaletteBuffer(u32 species, bool8 shiny, u32 PID, u16 pal16[16]);
bool8 AreColorVariantsEnabled(void);
void LoadMonPaletteWithVariants(u16 species, bool32 isShiny, u32 personality, u16 offset, bool32 isShadow);
void LoadMonSpritePaletteTagWithVariants(u16 species, bool32 isShiny, u32 personality, u16 tag, bool32 isShadow);

#endif // GUARD_VARIANT_COLOURS_H

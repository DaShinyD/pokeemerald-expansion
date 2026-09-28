#ifndef GUARD_POKERUS_H
#define GUARD_POKERUS_H

// Pokerus lives in pokemon.c here rather than its own module, so this maps the
// single-mon queries the ported screens use onto the party-wide helpers.

#include "global.h"
#include "pokemon.h"

static inline bool32 CheckMonPokerus(struct Pokemon *mon)
{
    return CheckPartyPokerus(mon, 0) != 0;
}

static inline bool32 CheckMonHasHadPokerus(struct Pokemon *mon)
{
    return CheckPartyHasHadPokerus(mon, 0) != 0;
}

static inline bool32 ShouldPokemonShowActivePokerus(struct Pokemon *mon)
{
    return CheckMonPokerus(mon);
}

static inline bool32 ShouldPokemonShowCuredPokerus(struct Pokemon *mon)
{
    return !CheckMonPokerus(mon) && CheckMonHasHadPokerus(mon);
}

#endif // GUARD_POKERUS_H

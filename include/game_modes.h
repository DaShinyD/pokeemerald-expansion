#ifndef GUARD_GAME_MODES_H
#define GUARD_GAME_MODES_H

#include "global.h"

void GameMode_BeginNewGameOptions(void);
void GameMode_CancelNewGameOptions(void);
bool8 GameMode_IsNewGameOptions(void);
void GameMode_ApplyNewGame(void);

void GameModeOptions_Load(void);
void GameModeOptions_Commit(void);
u8 GameModeOptions_PageCount(void);
u8 GameModeOptions_ItemCount(u8 page);
const u8 *GameModeOptions_ItemName(u8 page, u8 index);
const u8 *GameModeOptions_ChoiceOn(u8 page, u8 index);
const u8 *GameModeOptions_ChoiceOff(u8 page, u8 index);
bool8 GameModeOptions_Get(u8 page, u8 index);
bool8 GameModeOptions_IsLocked(u8 page, u8 index);
void GameModeOptions_Toggle(u8 page, u8 index);

bool8 AreLevelCapsEnabled(void);
bool8 AreScaledLevelsEnabled(void);
bool8 IsNuzlockeEnabled(void);
bool8 IsNuzlockeActive(void);

void GameMode_AdjustEnemyMon(struct Pokemon *mon, u32 salt);
u16 GameMode_RandomWildSpecies(u16 species, u32 salt);
u16 GameMode_RandomStaticSpecies(u16 species);
u16 GameMode_RandomizeFoundItem(u16 itemId, u32 salt);

void GameMode_NoteWildEncounter(u16 species);
void GameMode_FinishWildEncounter(void);
bool8 Nuzlocke_ShouldBlockCatch(void);
void Nuzlocke_BuryFaintedParty(void);
bool8 Nuzlocke_OnWhiteOut(void);
bool8 Nuzlocke_IsGraveyardBox(u8 boxId);

u16 Special_AreLevelCapsEnabled(void);
u16 Special_ActivateNuzlockeRules(void);

#endif // GUARD_GAME_MODES_H

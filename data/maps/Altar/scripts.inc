.set LOCALID_MOLTRES, 1
.set LOCALID_ZAPDOS, 2
.set LOCALID_ARTICUNO, 3

Altar_MapScripts::
	.byte 0

Altar_EventScript_Moltres_Galarian::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_MOLTRES_GALAR, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_MOLTRES_GALAR, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Altar_EventScript_Moltres_GalarianCaught
	release
	end

Altar_EventScript_Moltres_GalarianCaught::
	setflag FLAG_MOLTRES_G
	removeobject LOCALID_MOLTRES
	release
	end

Altar_EventScript_Zapdos_Galarian::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_ZAPDOS_GALAR, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ZAPDOS_GALAR, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Altar_EventScript_Zapdos_GalarianCaught
	release
	end

Altar_EventScript_Zapdos_GalarianCaught::
	setflag FLAG_ZAPDOS_G
	removeobject LOCALID_ZAPDOS
	release
	end

Altar_EventScript_Articuno_Galarian::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_ARTICUNO_GALAR, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ARTICUNO_GALAR, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Altar_EventScript_Articuno_GalarianCaught
	release
	end

Altar_EventScript_Articuno_GalarianCaught::
	setflag FLAG_ARTICUNO_G
	removeobject LOCALID_ARTICUNO
	release
	end

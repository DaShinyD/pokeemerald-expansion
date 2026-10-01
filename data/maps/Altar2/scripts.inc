.set LOCALID_MOLTRES, 1
.set LOCALID_ZAPDOS, 2
.set LOCALID_ARTICUNO, 3

Altar2_MapScripts::
	.byte 0

Altar2_EventScript_Moltres::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_MOLTRES, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Altar2_EventScript_MoltresCaught
	release
	end

Altar2_EventScript_MoltresCaught::
	setflag FLAG_MOLTRES
	removeobject LOCALID_MOLTRES
	release
	end

Altar2_EventScript_Zapdos::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ZAPDOS, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Altar2_EventScript_ZapdosCaught
	release
	end

Altar2_EventScript_ZapdosCaught::
	setflag FLAG_ZAPDOS
	removeobject LOCALID_ZAPDOS
	release
	end

Altar2_EventScript_Articuno::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_ARTICUNO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ARTICUNO, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Altar2_EventScript_ArticunoCaught
	release
	end

Altar2_EventScript_ArticunoCaught::
	setflag FLAG_ARTICUNO
	removeobject LOCALID_ARTICUNO
	release
	end

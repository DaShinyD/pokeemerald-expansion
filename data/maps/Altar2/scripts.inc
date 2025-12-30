.set LOCALID_MOLTRES, 1
.set LOCALID_ZAPDOS, 2
.set LOCALID_ARTICUNO, 3

Altar2_MapScripts::
	.byte 0

Altar2_EventScript_Moltres::
	lock
	faceplayer
	playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_MOLTRES, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Altar2_EventScript_MoltresGone
	release
	end

Altar2_EventScript_MoltresGone:
	setflag FLAG_MOLTRES
	removeobject LOCALID_MOLTRES
	release
	end

Altar2_EventScript_Zapdos::
	lock
	faceplayer
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ZAPDOS, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Altar2_EventScript_ZapdosGone
	release
	end

Altar2_EventScript_ZapdosGone:
	setflag FLAG_ZAPDOS
	removeobject LOCALID_ZAPDOS
	release
	end

Altar2_EventScript_Articuno::
	lock
	faceplayer
	playmoncry SPECIES_ARTICUNO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ARTICUNO, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Altar2_EventScript_ArticunoGone
	release
	end

Altar2_EventScript_ArticunoGone:
	setflag FLAG_ARTICUNO
	removeobject LOCALID_ARTICUNO
	release
	end


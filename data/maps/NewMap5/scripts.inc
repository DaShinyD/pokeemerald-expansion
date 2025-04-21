.set LOCALID_MOLTRES, 1
.set LOCALID_ZAPDOS, 2
.set LOCALID_ARTICUNO, 3

NewMap5_MapScripts::
	.byte 0

NewMap5_EventScript_Moltres_Galarian::
	lock
	faceplayer
	playmoncry SPECIES_MOLTRES_GALARIAN, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_MOLTRES_GALARIAN, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NewMap5_EventScript_Moltres_GalarianGone
	release
	end

NewMap5_EventScript_Moltres_GalarianGone:
	setflag FLAG_MOLTRES_G
	removeobject LOCALID_MOLTRES
	release
	end

NewMap5_EventScript_Zapdos_Galarian::
	lock
	faceplayer
	playmoncry SPECIES_ZAPDOS_GALARIAN, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ZAPDOS_GALARIAN, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NewMap5_EventScript_Zapdos_GalarianGone
	release
	end

NewMap5_EventScript_Zapdos_GalarianGone:
	setflag FLAG_ZAPDOS_G
	removeobject LOCALID_ZAPDOS
	release
	end

NewMap5_EventScript_Articuno_Galarian::
	lock
	faceplayer
	playmoncry SPECIES_ARTICUNO_GALARIAN, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ARTICUNO_GALARIAN, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NewMap5_EventScript_Articuno_GalarianGone
	release
	end

NewMap5_EventScript_Articuno_GalarianGone:
	setflag FLAG_ARTICUNO_G
	removeobject LOCALID_ARTICUNO
	release
	end


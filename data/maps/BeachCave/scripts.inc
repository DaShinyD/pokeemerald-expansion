.set LOCALID_SUICUNE, 1
.set LOCALID_SUICUNE_P, 2

BeachCave_MapScripts::
	.byte 0

BeachCave_EventScript_Suicune::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_SUICUNE, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BeachCave_EventScript_SuicuneCaught
	release
	end

BeachCave_EventScript_SuicuneCaught::
	setflag FLAG_SUICUNE
	removeobject LOCALID_SUICUNE
	release
	end

BeachCave_EventScript_SuicuneP::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_WALKING_WAKE, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_WALKING_WAKE, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BeachCave_EventScript_SuicunePCaught
	release
	end

BeachCave_EventScript_SuicunePCaught::
	setflag FLAG_SUICUNE_P
	removeobject LOCALID_SUICUNE_P
	release
	end

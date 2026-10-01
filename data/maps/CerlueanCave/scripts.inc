.set LOCALID_MEWTWO, 1
.set LOCALID_MEW, 2

CerlueanCave_MapScripts::
	.byte 0

CerlueanCave_EventScript_Mewtwo::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_MEWTWO, 90
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CerlueanCave_EventScript_MewtwoCaught
	release
	end

CerlueanCave_EventScript_MewtwoCaught::
	setflag FLAG_MEWTWO
	removeobject LOCALID_MEWTWO
	release
	end

CerlueanCave_EventScript_Mew::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_MEW, 90
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CerlueanCave_EventScript_MewCaught
	release
	end

CerlueanCave_EventScript_MewCaught::
	setflag FLAG_MEW
	removeobject LOCALID_MEW
	release
	end

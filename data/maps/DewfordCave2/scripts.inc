.set LOCALID_TERRAKION, 1
.set LOCALID_KELDEO, 2
.set LOCALID_COBALION, 3
.set LOCALID_VIRIZION, 4

DewfordCave2_MapScripts::
	.byte 0

DewfordCave2_EventScript_Terrakion::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_TERRAKION, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_TERRAKION, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DewfordCave2_EventScript_TerrakionCaught
	release
	end

DewfordCave2_EventScript_TerrakionCaught::
	setflag FLAG_TERRAKION
	removeobject LOCALID_TERRAKION
	release
	end

DewfordCave2_EventScript_Keldeo::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_KELDEO, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_KELDEO, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DewfordCave2_EventScript_KeldeoCaught
	release
	end

DewfordCave2_EventScript_KeldeoCaught::
	setflag FLAG_KELDEO
	removeobject LOCALID_KELDEO
	release
	end

DewfordCave2_EventScript_Cobalion::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_COBALION, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_COBALION, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DewfordCave2_EventScript_CobalionCaught
	release
	end

DewfordCave2_EventScript_CobalionCaught::
	setflag FLAG_COBALION
	removeobject LOCALID_COBALION
	release
	end

DewfordCave2_EventScript_Virizion::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_VIRIZION, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_VIRIZION, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DewfordCave2_EventScript_VirizionCaught
	release
	end

DewfordCave2_EventScript_VirizionCaught::
	setflag FLAG_VIRIZION
	removeobject LOCALID_VIRIZION
	release
	end

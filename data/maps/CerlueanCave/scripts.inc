.set LOCALID_MEWTWO, 1
.set LOCALID_MEW, 2

CerlueanCave_MapScripts::
	.byte 0

CerlueanCave_EventScript_Mewtwo::
	lock
	faceplayer
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
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_MEWTWO
	setflag FLAG_MEWTWO
	release
	end

CerlueanCave_EventScript_Mew::
	lock
	faceplayer
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
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_MEW
	setflag FLAG_MEW
	release
	end


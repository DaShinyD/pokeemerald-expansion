.set LOCALID_GIRATINA, 1

NewMap3_MapScripts::
	.byte 0

NewMap3_EventScript_Giratina::
	lock
	faceplayer
	playmoncry SPECIES_GIRATINA, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	delay 50
	setwildbattle SPECIES_GIRATINA, 70
	special BattleSetup_StartLegendaryBattle
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_GIRATINA
	setflag FLAG_GIRATINA
	clearflag FLAG_DIALGA
	clearflag FLAG_PALKIA
	release
	end


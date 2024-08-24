.set LOCALID_TERRAKION, 1
.set LOCALID_KELDEO, 2
.set LOCALID_COBALION, 3
.set LOCALID_VIRIZION, 4

DewfordCave2_MapScripts::
	.byte 0

DewfordCave2_EventScript_Terrakion::
	lock
	faceplayer
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
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_TERRAKION
	setflag FLAG_TERRAKION
	release
	end

DewfordCave2_EventScript_Keldeo::
	lock
	faceplayer
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
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_KELDEO
	setflag FLAG_KELDEO
	release
	end

DewfordCave2_EventScript_Cobalion::
	lock
	faceplayer
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
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_COBALION
	setflag FLAG_COBALION
	release
	end

DewfordCave2_EventScript_Virizion::
	lock
	faceplayer
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
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	delay 50
	removeobject LOCALID_VIRIZION
	setflag FLAG_VIRIZION
	release
	end


.set LOCALID_GIRATINA, 1

DistortionWorld_MapScripts::
	.byte 0

DistortionWorld_EventScript_Giratina::
	lock
	faceplayer
	call Common_EventScript_SaveGame
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
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DistortionWorld_EventScript_GiratinaCaught
	release
	end

DistortionWorld_EventScript_GiratinaCaught::
	setflag FLAG_GIRATINA
	removeobject LOCALID_GIRATINA
	clearflag FLAG_DIALGA
	clearflag FLAG_PALKIA
	release
	end

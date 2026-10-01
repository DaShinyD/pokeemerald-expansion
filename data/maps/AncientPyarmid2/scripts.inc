.set LOCALID_ZYGARDE, 1

AncientPyarmid2_MapScripts::
	.byte 0

AncientPyarmid2_EventScript_Zygarde::
	lock
	faceplayer
	call Common_EventScript_SaveGame
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_ZYGARDE, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, AncientPyarmid2_EventScript_ZygardeCaught
	release
	end

AncientPyarmid2_EventScript_ZygardeCaught::
	setflag FLAG_ZYGARDE
	removeobject LOCALID_ZYGARDE
	release
	end

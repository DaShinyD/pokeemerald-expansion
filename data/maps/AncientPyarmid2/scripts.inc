.set LOCALID_ZYGARDE, 1

AncientPyarmid2_MapScripts::
	.byte 0

AncientPyarmid2_EventScript_Zygarde::
	lock
	faceplayer
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_ZYGARDE, 70
	special BattleSetup_StartLegendaryBattle
	removeobject LOCALID_ZYGARDE
	setflag FLAG_ZYGARDE
	waitstate
	release
	end


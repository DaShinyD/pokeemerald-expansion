.set LOCALID_LUNALA, 1
.set LOCALID_SOLGALEO, 2
.set LOCALID_KAIBA, 3

Space_MapScripts::
	.byte 0

Space_EventScript_Solgaleo::
	lock
	faceplayer
	playmoncry SPECIES_SOLGALEO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_SOLGALEO, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Space_EventScript_SolgaleoGone
	release
	end

Space_EventScript_SolgaleoGone:
	setflag FLAG_SOLGALEO
	removeobject LOCALID_SOLGALEO
	release
	end

Space_EventScript_Lunala::
	lock
	faceplayer
	playmoncry SPECIES_LUNALA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_LUNALA, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Space_EventScript_LunalaGone
	release
	end

Space_EventScript_LunalaGone:
	setflag FLAG_LUNALA
	removeobject LOCALID_LUNALA
	release
	end


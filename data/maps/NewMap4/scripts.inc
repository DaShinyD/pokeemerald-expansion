.set LOCALID_ENTEI, 1
.set LOCALID_ENTEI_P, 2

NewMap4_MapScripts::
	.byte 0

NewMap4_EventScript_Entei::
	lock
	faceplayer
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_ENTEI, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NewMap4_EventScript_EnteiGone
	release
	end

NewMap4_EventScript_EnteiGone:
	setflag FLAG_ENTEI
	removeobject LOCALID_ENTEI
	release
	end

NewMap4_EventScript_EnteiP::
	lock
	faceplayer
	playmoncry SPECIES_GOUGING_FIRE, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_GOUGING_FIRE, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, NewMap4_EventScript_EnteiPGone
	release
	end

NewMap4_EventScript_EnteiPGone:
	setflag FLAG_ENTEI_P
	removeobject LOCALID_ENTEI_P
	release
	end


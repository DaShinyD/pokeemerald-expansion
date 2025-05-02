.set LOCALID_UNOWN_1, 1
.set LOCALID_UNOWN_2, 2
.set LOCALID_UNOWN_3, 3
.set LOCALID_UNOWN_4, 4
.set LOCALID_UNOWN_5, 5

UnknownRuinsGodCave_MapScripts::
	.byte 0

UnknownRuinsGodCave_EventScript_Unone::
	lock
	goto_if_set FLAG_UNOWN_GOD_EVENT, UnknownRuinsGodCave_EventScript_UnoneNO
	applymovement LOCALID_UNOWN_1, UnknownRuinsGodCave_Movement_Approach
	applymovement LOCALID_UNOWN_2, UnknownRuinsGodCave_Movement_Approach
	applymovement LOCALID_UNOWN_3, UnknownRuinsGodCave_Movement_Approach
	applymovement LOCALID_UNOWN_4, UnknownRuinsGodCave_Movement_Approach
	applymovement LOCALID_UNOWN_5, UnknownRuinsGodCave_Movement_Approach
	waitmovement 0
	playmoncry SPECIES_UNOWN, CRY_MODE_ENCOUNTER
	waitmoncry
	setobjectxy LOCALID_UNOWN_1, 9, 1
	setobjectxy LOCALID_UNOWN_2, 7, 1
	setobjectxy LOCALID_UNOWN_3, 5, 1
	setobjectxy LOCALID_UNOWN_4, 3, 1
	setobjectxy LOCALID_UNOWN_5, 1, 1
	setwildbattle SPECIES_UNOWN_GOD, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, UnknownRuinsGodCave_EventScript_UnoneGone
	release
	end

UnknownRuinsGodCave_EventScript_UnoneGone::
	setflag FLAG_UNOWN_GOD_EVENT
	release
	end

UnknownRuinsGodCave_EventScript_UnoneNO::
	release
	end

UnknownRuinsGodCave_Movement_Approach:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end


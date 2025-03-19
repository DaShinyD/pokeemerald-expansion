.set LOCALID_REGIDRAGO, 1
.set LOCALID_REGIELEKI, 2

ThunderDragonCave_MapScripts::
	.byte 0

ThunderDragonCave_EventScript_BrailleHint::
	lockall
	braillemessage ThunderDragonCave_Braille_Clock
	waitbuttonpress
	closebraillemessage
	releaseall
	end

ThunderDragonCave_EventScript_Rock5::
	goto_if_ne VAR_REGI_CAVE_STATE, 0, ThunderDragonCave_EventScript_Reset
	setvar VAR_REGI_CAVE_STATE, 1
	end

ThunderDragonCave_EventScript_Rock7::
	goto_if_ne VAR_REGI_CAVE_STATE, 1, ThunderDragonCave_EventScript_Reset
	setvar VAR_REGI_CAVE_STATE, 2
	end

ThunderDragonCave_EventScript_Rock2::
	goto_if_ne VAR_REGI_CAVE_STATE, 2, ThunderDragonCave_EventScript_Reset
	setvar VAR_REGI_CAVE_STATE, 3
	end

ThunderDragonCave_EventScript_Rock3::
	goto_if_ne VAR_REGI_CAVE_STATE, 3, ThunderDragonCave_EventScript_Reset
	setvar VAR_REGI_CAVE_STATE, 4
	end

ThunderDragonCave_EventScript_Rock11::
	goto_if_ne VAR_REGI_CAVE_STATE, 4, ThunderDragonCave_EventScript_Reset
	setvar VAR_REGI_CAVE_STATE, 5
	end

ThunderDragonCave_EventScript_Rock8::
	goto_if_ne VAR_REGI_CAVE_STATE, 5, ThunderDragonCave_EventScript_Reset
	setmetatile 9, 13, METATILE_Cave_SealedChamberEntrance_TopLeft, TRUE
	setmetatile 10, 13, METATILE_Cave_SealedChamberEntrance_TopMid, TRUE
	setmetatile 11, 13, METATILE_Cave_SealedChamberEntrance_TopRight, TRUE
	setmetatile 9, 14, METATILE_Cave_SealedChamberEntrance_BottomLeft, TRUE
	setmetatile 10, 14, METATILE_Cave_SealedChamberEntrance_BottomMid, FALSE
	setmetatile 11, 14, METATILE_Cave_SealedChamberEntrance_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	end

ThunderDragonCave_EventScript_Reset::
	setvar VAR_REGI_CAVE_STATE, 0
	end

ThunderDragonCave_EventScript_Regidrago::
	lock
	faceplayer
	playmoncry SPECIES_REGIDRAGO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_REGIDRAGO, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ThunderDragonCave_EventScript_RegidragoGone
	release
	end

ThunderDragonCave_EventScript_RegidragoGone::
	fadescreen FADE_TO_BLACK
	setflag FLAG_REGIDRAGO
	removeobject LOCALID_REGIDRAGO
	fadescreen FADE_FROM_BLACK
	release
	end

ThunderDragonCave_EventScript_Regieleki::
	lock
	faceplayer
	playmoncry SPECIES_REGIELEKI, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_REGIELEKI, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ThunderDragonCave_EventScript_RegielekiGone
	release
	end

ThunderDragonCave_EventScript_RegielekiGone::
	fadescreen FADE_TO_BLACK
	setflag FLAG_REGIELEKI
	removeobject LOCALID_REGIELEKI
	fadescreen FADE_FROM_BLACK
	release
	end


	.set LOCALID_MATCH3_1, 1
	.set LOCALID_MATCH3_2, 2
	.set LOCALID_MATCH3_3, 3
	.set LOCALID_MATCH3_4, 4
	.set LOCALID_MATCH3_5, 5
	.set LOCALID_MATCH3_6, 6
	.set LOCALID_MATCH3_7, 7
	.set LOCALID_MATCH3_8, 8
	.set LOCALID_MATCH3_9, 9

	.set MATCH3_COST, 100
	.set MATCH3_WIN_PAYOUT, 1800

CeladonGameCorner_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CeladonGameCorner_OnFrame
	.byte 0

CeladonGameCorner_OnFrame::
	map_script_2 VAR_TEMP_3, 0, CeladonGameCorner_EventScript_Match3_InitVisibility
	.2byte 0

CeladonGameCorner_EventScript_Match3_InitVisibility::
	setvar VAR_0x8000, 2
	special StartMatch3Game
	compare VAR_RESULT, 1
	call_if_eq CeladonGameCorner_EventScript_Match3_SetInitDone
	end

CeladonGameCorner_EventScript_Match3_SetInitDone::
	setvar VAR_TEMP_3, 1
	return

CeladonGameCorner_EventScript_Match3::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, CeladonGameCorner_EventScript_Match3_NoCoinCase
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, MATCH3_COST, CeladonGameCorner_EventScript_Match3_NotEnough
	showcoinsbox 1, 1
	msgbox CeladonGameCorner_Text_Match3, MSGBOX_YESNO
	closemessage
	goto_if_eq VAR_RESULT, NO, CeladonGameCorner_EventScript_Match3_Cancel
	removecoins MATCH3_COST
	updatecoinsbox 1, 1
	playse SE_SHOP
	delay 40
	hidecoinsbox 0, 5
	@ Walk all 9 objects up one step (only 3 are visible initially: 1, 6, 8)
	applymovement LOCALID_MATCH3_1, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_2, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_3, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_4, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_5, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_6, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_7, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_8, Common_Movement_WalkUp
	applymovement LOCALID_MATCH3_9, Common_Movement_WalkUp
	waitmovement 0
	delay 20
	@ Hide all 9 so they "leave", then pause before new ones appear
	setvar VAR_0x8000, 3
	special StartMatch3Game
	delay 60
	@ Roll and show the three for this round, set VAR_0x8001/0x8002/0x8003
	setvar VAR_0x8000, 0
	special StartMatch3Game
	@ Face them down so they don't appear facing away when they walk out
	turnobject VAR_0x8001, DIR_SOUTH
	turnobject VAR_0x8002, DIR_SOUTH
	turnobject VAR_0x8003, DIR_SOUTH
	delay 20
	@ Walk all 9 down so every object is back at base position (otherwise the 6 hidden ones stay 1 tile high)
	applymovement LOCALID_MATCH3_1, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_2, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_3, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_4, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_5, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_6, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_7, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_8, Common_Movement_WalkDown
	applymovement LOCALID_MATCH3_9, Common_Movement_WalkDown
	waitmovement 0
	delay 60
	goto_if_eq VAR_RESULT, 1, CeladonGameCorner_EventScript_Match3_Win
	msgbox CeladonGameCorner_Text_Match3_Lose, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

CeladonGameCorner_EventScript_Match3_Win::
	showcoinsbox 1, 1
	setvar GAME_CORNER_VAR_WINNINGS, MATCH3_WIN_PAYOUT
	buffernumberstring STR_VAR_1, GAME_CORNER_VAR_WINNINGS
	msgbox CeladonGameCorner_Text_Match3_Win, MSGBOX_DEFAULT
	waitmessage
	addcoins GAME_CORNER_VAR_WINNINGS
	setvar GAME_CORNER_VAR_WINNINGS, 0
	updatecoinsbox 1, 1
	playse SE_SHOP
	delay 40
	hidecoinsbox 0, 5
	closemessage
	releaseall
	end

CeladonGameCorner_EventScript_Match3_NoCoinCase::
	msgbox CeladonGameCorner_Text_Match3_NoCoinCase, MSGBOX_DEFAULT
	releaseall
	end

CeladonGameCorner_EventScript_Match3_NotEnough::
	msgbox CeladonGameCorner_Text_Match3_NotEnough, MSGBOX_DEFAULT
	releaseall
	end

CeladonGameCorner_EventScript_Match3_Cancel::
	hidecoinsbox 0, 5
	closemessage
	releaseall
	end

CeladonGameCorner_Text_Match3:
	.string "MATCH 3!\p"
	.string "Match CHARMANDER, BULBASAUR, or\n"
	.string "SQUIRTLE across all three!\p"
	.string "It costs 100 COINS to play.$"

CeladonGameCorner_Text_Match3_Win:
	.string "You won\n"
	.string "{STR_VAR_1} COINS!$"

CeladonGameCorner_Text_Match3_Lose:
	.string "No match this time!$"

CeladonGameCorner_Text_Match3_NoCoinCase:
	.string "You can't play if you don't have\n"
	.string "a COIN CASE.$"

CeladonGameCorner_Text_Match3_NotEnough:
	.string "It's MATCH 3!\p"
	.string "You don't have enough COINS…$"

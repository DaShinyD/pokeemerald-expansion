.set LOCALID_TPP, 3

Route4_MapScripts::
	.byte 0

Route4_EventScript_Blocker::
	msgbox Route4_Text_Nope, MSGBOX_NPC
	end

Route4_Text_Nope:
	.string "Mt. Moon had a cave in and is\n"
	.string "currently impassable.\p"
	.string "Luckily, a path was made just\n"
	.string "south of here.$"

Route4_EventScript_Closed::
	msgbox Route4_Text_Closed, MSGBOX_SIGN
	end

Route4_Text_Closed:
	.string "There's a note written.\p"
	.string "Currently closed helping people\n"
	.string "in Mt. Moon. Sorry!$"

Route4_EventScript_TPP::
	lock
	addobject LOCALID_TPP
	applymovement LOCALID_TPP, Route4_Movement_MemeWalk
	waitmovement 0
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Route4_Text_MemeText, MSGBOX_AUTOCLOSE
	goto Route4_EventScript_TPPBattle
	end

Route4_Movement_MemeWalk:
	walk_right
	walk_right
	walk_down
	walk_up
	walk_down
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_right
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_right
	walk_in_place_right
	walk_down
	step_end

Route4_Text_MemeText:
.string "left left right a a b a b a a right\p"
.string "start a a b start\p"
.string "GUYS PRESS A WE MADE IT$"

Route4_EventScript_TPPBattle::
	trainerbattle_no_intro TRAINER_TPP, Route4_Text_TPPDefeat
	goto Route4_EventScript_TPPLeave
	end

Route4_EventScript_TPPLeave:
	msgbox Route4_Text_MemeText2, MSGBOX_AUTOCLOSE
	removeobject LOCALID_TPP
	setflag FLAG_HIDE_TPP
	setvar VAR_HELIX_FIGHT, 1
	release
	end

Route4_Text_MemeText2:
	.string "a a a b b left right left\p"
	.string "a b start select\p"
	.string "NO GUYS THATS THE RESE--$"

Route4_Text_TPPDefeat:
	.string "a a a a a a a$"


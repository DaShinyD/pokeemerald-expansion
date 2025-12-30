.set LOCALID_NUDE, 5
.set LOCALID_COP_1, 6
.set LOCALID_COP_2, 7

PokemonFanClub_MapScripts::
	.byte 0

PokemonFanClub_EventScript_MemeScene::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, PokemonFanClub_Movement_StepForward
	waitmovement 0
	applymovement LOCALID_NUDE, PokemonFanClub_Movement_BackAndForth
	waitmovement 0
	msgbox PokemonFanClub_Text_YeaGood, MSGBOX_AUTOCLOSE
	playse SE_DOOR
	addobject LOCALID_COP_1
	addobject LOCALID_COP_2
	playse SE_PIN
	applymovement LOCALID_NUDE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_NUDE, PokemonFanClub_Movement_FaceDown
	waitmovement 0
	msgbox PokemonFanClub_Text_WaitExplain, MSGBOX_AUTOCLOSE
	applymovement LOCALID_COP_1, PokemonFanClub_Movement_ApproachNude
	applymovement LOCALID_COP_2, PokemonFanClub_Movement_ApproachNude2
	waitmovement 0
	msgbox PokemonFanClub_Text_YeaYea, MSGBOX_AUTOCLOSE
	applymovement LOCALID_COP_1, PokemonFanClub_Movement_ArrestNude
	applymovement LOCALID_NUDE, PokemonFanClub_Movement_GetArrested
	applymovement LOCALID_COP_2, PokemonFanClub_Movement_ArrestNude2
	waitmovement 0
	playse SE_DOOR
	removeobject LOCALID_COP_1
	applymovement LOCALID_NUDE, PokemonFanClub_Movement_Continue
	applymovement LOCALID_COP_2, PokemonFanClub_Movement_Continue
	waitmovement 0
	playse SE_DOOR
	removeobject LOCALID_NUDE
	setflag FLAG_NUDE_GUY
	applymovement LOCALID_COP_2, PokemonFanClub_Movement_Continue
	waitmovement 0
	playse SE_DOOR
	removeobject LOCALID_COP_2
	setvar VAR_FAN_CLUB, 1
	release
	end

PokemonFanClub_Movement_StepForward:
	walk_up
	walk_up
	step_end

PokemonFanClub_Movement_BackAndForth:
	walk_left
	walk_left
	walk_right
	walk_right
	walk_right
	walk_left
	face_up
	step_end

PokemonFanClub_Text_YeaGood:
	.string "Yeah..yeah..that's good stuff.$"

PokemonFanClub_Movement_FaceDown:
	face_down
	step_end

PokemonFanClub_Text_WaitExplain:
	.string "Wait, I can explain!$"

PokemonFanClub_Movement_ApproachNude:
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	face_left
	step_end

PokemonFanClub_Movement_ApproachNude2:
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	step_end

PokemonFanClub_Text_YeaYea:
	.string "Guards: Yeah okay bud.\p"
	.string "You can explain it all down at the\n"
	.string "station.$"

PokemonFanClub_Movement_ArrestNude:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_down
	walk_down
	step_end

PokemonFanClub_Movement_GetArrested:
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_down
	step_end

PokemonFanClub_Movement_ArrestNude2:
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	step_end

PokemonFanClub_Movement_Continue:
	walk_down
	step_end


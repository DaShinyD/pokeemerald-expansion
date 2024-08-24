.set LOCALID_LADY, 1

DewfordRoute_MapScripts::
	.byte 0

DewfordRoute_EventScript_MBall::
	call Common_EventScript_FindItem
	end

DewfordRoute_EventScript_LadyBlocksCave::
	lock
	faceplayer
	goto_if_set FLAG_IS_CHAMPION, DewfordRoute_EventScript_LadyMovesFromCave
	msgbox DewfordRoute_Text_NotReady, MSGBOX_DEFAULT
	release
	end

DewfordRoute_Text_NotReady:
	.string "Hello young one. I'm sorry,\n"
	.string "but you aren't ready for\p"
	.string "what lies ahead.\n"
	.string "Come back when you are\p"
	.string "stronger.$"

DewfordRoute_EventScript_LadyMovesFromCave::
	playse SE_PIN
	msgbox DewfordRoute_Text_PleaseGoAhead, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, DewfordRoute_Movement_MoveOutOfWay
	waitmovement 0
	applymovement LOCALID_LADY, DewfordRoute_Movement_LadyMoves
	waitmovement 0
	removeobject LOCALID_LADY
	setflag FLAG_BLOCKER_DEWFORD
	release
	end

DewfordRoute_Text_PleaseGoAhead:
	.string "Oh! You recently beat the\n"
	.string "champion.\p"
	.string "You have what it takes to\n"
	.string "face the Pokemon in this cave.\p"
	.string "Please, go ahead.$"

DewfordRoute_Movement_MoveOutOfWay:
	walk_left
	face_right
	step_end

DewfordRoute_Movement_LadyMoves:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

DewfordRoute_EventScript_Scientist1::
	lock
	faceplayer
	msgbox DewfordRoute_Text_Scientist1, MSGBOX_DEFAULT
	release
	end

DewfordRoute_EventScript_Scientist2::
	lock
	faceplayer
	msgbox DewfordRoute_Text_Scientist2, MSGBOX_DEFAULT
	release
	end

DewfordRoute_Text_Scientist1:
	.string "Hello, I'm examining energy\n"
	.string "left behind by legendary\p"
	.string "Pokemon.$"

DewfordRoute_Text_Scientist2:
	.string "I'm getting energy readings\n"
	.string "from fighting types.$"


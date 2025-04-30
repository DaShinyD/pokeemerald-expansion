.set LOCALID_ATTENDANT, 1
.set LOCALID_ATTENDANT_2, 2

PWT_MapScripts::
	.byte 0

PWT_EventScript_Level200::
	lock
	msgbox PWT_Text_Hello200, MSGBOX_DEFAULT
	specialvar VAR_RESULT, AreAllPartyMonsLevel200
	goto_if_eq VAR_RESULT, FALSE, PWT_EventScript_Hmm
	specialvar VAR_RESULT, CheckIfPartySizeIsExactly4
	goto_if_eq VAR_RESULT, FALSE, PWT_EventScript_Hmm
	msgbox PWT_Text_AlrightReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PWT_EventScript_GuessNot
	msgbox PWT_Text_FollowMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ATTENDANT_2, PWT_Movement_GoToElevator
	applymovement OBJ_EVENT_ID_PLAYER, PWT_Movement_FollowAttendant
	waitmovement 0
	opendoor 24, 1
	waitdooranim
	applymovement LOCALID_ATTENDANT_2, PWT_Movement_EnterElevator
	applymovement OBJ_EVENT_ID_PLAYER, PWT_Movement_EnterElevator
	waitmovement 0
	removeobject LOCALID_ATTENDANT_2
	setflag FLAG_ATTENDANTS
	applymovement OBJ_EVENT_ID_PLAYER, PWT_Movement_PlayerEnterElevator
	waitmovement 0
	clearflag FLAG_ATTENDANTS
	setvar VAR_PWT_TYPE, 2
	setflag FLAG_HIDE_PWT_A
	goto_if_set FLAG_PWT_BEAT_200, PWT_EventScript_Warp2
	warp MAP_PWT_ARENA, 4, 24
	release
	end

PWT_Text_Hello200:
	.string "Hello!\p"
	.string "Welcome to the level 200 doubles\n"
	.string "battle tournament.\p"
	.string "In order to enter, you must have\n"
	.string "{COLOR RED}four Pokemon {COLOR DARK_GRAY}in your party all\l"
	.string "{COLOR RED}level 200{COLOR DARK_GRAY}.\p"
	.string "Please allow me to check your party.$"

PWT_Text_AlrightReady:
	.string "Congratulations, you meet all of\n"
	.string "the requirements.\p"
	.string "Are you ready to enter?$"

PWT_Text_FollowMe:
	.string "Great, follow me.$"

PWT_Movement_GoToElevator:
	walk_up
	walk_up
	walk_up
	step_end

PWT_Movement_FollowAttendant:
	walk_up
	walk_up
	walk_up
	step_end

PWT_Movement_EnterElevator:
	walk_up
	step_end

PWT_Movement_PlayerEnterElevator:
	walk_up
	step_end

PWT_EventScript_Hmm::
	msgbox PWT_Text_DontMeet, MSGBOX_DEFAULT
	release
	end

PWT_Text_DontMeet:
	.string "It looks like you don't meet the\n"
	.string "requirements.\p"
	.string "Please feel free to return when\n"
	.string "you do.$"

PWT_EventScript_GuessNot::
	msgbox PWT_Text_GuessNot, MSGBOX_DEFAULT
	release
	end

PWT_Text_GuessNot:
	.string "Oh, guess not.\p"
	.string "Feel free to return if you change\n"
	.string "your mind.$"

PWT_EventScript_Warp2::
	warp MAP_PWT_ARENA2, 4, 24
	release
	end


Champion_Club_MapScripts::
	.byte 0

Champion_Club_EventScript_FirstEntryL::
	lock
	msgbox Champion_Club_Text_HeyYou, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Movement_ApproachDeskL
	waitmovement 0
	msgbox Champion_Club_Text_ClubExplained, MSGBOX_DEFAULT\
	setvar VAR_CHAMPION_CLUB_ENTER, 1
	release
	end

Champion_Club_EventScript_FirstEntryR::
	lock
	msgbox Champion_Club_Text_HeyYou, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Movement_ApproachDeskR
	waitmovement 0
	msgbox Champion_Club_Text_ClubExplained, MSGBOX_DEFAULT
	setvar VAR_CHAMPION_CLUB_ENTER, 1
	release
	end

Champion_Club_Movement_ApproachDeskL:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	face_up
	step_end

Champion_Club_Movement_ApproachDeskR:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Champion_Club_Text_HeyYou:
	.string "Hey newbie, over here.\p"
	.string "Come to the desk.$"

Champion_Club_Text_ClubExplained:
	.string "Welcome to the Champion Club!\p"
	.string "Here, Master Trainers and other\n"
	.string "Champions gather to train and\l"
	.string "battle each other.\p"
	.string "There are people from all over the\n"
	.string "world!\p"
	.string "This facility is for free battles\n"
	.string "in the singles format.\p"
	.string "Eventually, a tournament will be\n"
	.string "held in doubles to determine the\l"
	.string "best trainer.\p"
	.string "If you would like to challenge\n"
	.string "anyone, simply step on the red\l"
	.string "area opposite of them.\p"
	.string "Enjoy your time here.$"

Champion_Club_EventScript_DeskClerk::
	msgbox Champion_Club_Text_HowIsIt, MSGBOX_NPC
	end

Champion_Club_Text_HowIsIt:
	.string "Are you enjoying your time here?$"

@ Begin trainer stuff



@ Generic Scripts

Champion_Club_EventScript_DenyBattle::
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Movement_WalkDown
	waitmovement 0
	release
	end

Champion_Club_Movement_MoveBattleCamera:
	walk_slow_left
	walk_slow_left
	walk_slow_left
	walk_slow_left
	step_end

Champion_Club_Movement_MoveBattleCameraBack:
	walk_slow_right
	walk_slow_right
	walk_slow_right
	walk_slow_right
	step_end

Champion_Club_Movement_WalkDown:
	walk_down
	step_end



@ Jake

Champion_Club_EventScript_JakeChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeJake, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_JakeBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeJake:
	.string "Would you like to challenge Master\n"
	.string "Trainer Jake?$"

Champion_Club_EventScript_JakeBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_JakeLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_JakeBattle
	end

Champion_Club_Text_JakeLetsFight:
	.string "Announcer: The battle between Jake\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Jake: My Umbreon and I will take\n"
	.string "you down.$"

Champion_Club_EventScript_JakeBattle::
	trainerbattle_no_intro TRAINER_JAKE, Champion_Club_Text_JakeDefeat
	goto Champion_Club_EventScript_JakeBeat
	end

Champion_Club_Text_JakeDefeat:
	.string "It ain't easy being cheesy.$"

Champion_Club_EventScript_JakeBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	release
	end

Champion_Club_Text_TheWinnerPlayer:
	.string "The winner is {PLAYER}!$"

